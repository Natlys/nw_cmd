#include "nwcmd_engine.h"

#define NWCMD_INPUT_FLAGS ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT

namespace NWCMD
{
    cmd_engine::cmd_engine() :
        m_fmbuf(*this),
        m_wnd_info(cmd_window_info()),
        m_pxl_info(cmd_pixel_info()),
        m_cout(nullptr), m_cin(nullptr)
    {
        this->set_wnd_size(120, 40);
        this->set_pxl_size(8, 16);
        m_fmbuf.set_size(get_wnd_size_x(), get_wnd_size_y());
        m_cout = GetStdHandle(STD_OUTPUT_HANDLE);
        m_cin = GetStdHandle(STD_INPUT_HANDLE);
        m_wnd_info.title = "console_window";
    }
    cmd_engine::~cmd_engine()
    {
        quit();
    }
    // --setters
    void cmd_engine::stop_running() { m_is_running = false; }
    void cmd_engine::set_title(cstr title) { m_wnd_info.title = title; ::SetConsoleTitleA(&title[0]); }
    void cmd_engine::set_wnd_size(v1u16 uwidth, v1u16 uheight) {
        m_wnd_info.wnd_rect.Left = m_wnd_info.wnd_rect.Top = 0;
        m_wnd_info.wnd_rect.Right = uwidth; m_wnd_info.wnd_rect.Bottom = uheight;
    }
    void cmd_engine::set_pxl_size(v1u16 width, v1u16 height){
        m_pxl_info.dwFontSize.X = width;
        m_pxl_info.dwFontSize.Y = height;
        m_pxl_info.FontFamily = FF_DONTCARE;
        m_pxl_info.FontWeight = FW_NORMAL;
        m_pxl_info.nFont = 0;
        swprintf_s(m_pxl_info.FaceName, L"Consolas");
        m_pxl_info.cbSize = sizeof(m_pxl_info);
    }

    void cmd_engine::set_wnd_info(const cmd_window_info& info) { m_wnd_info = info; ::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect); }
    void cmd_engine::set_pxl_info(const cmd_pixel_info& rcpxInfo) { m_pxl_info = rcpxInfo; ::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info); }
    void cmd_engine::set_crs_info(const cmd_cursor_info& rcrsInfo) { m_mouse_info = rcrsInfo; ::SetConsoleCursorInfo(m_fmbuf.get_handle(), &m_mouse_info); }
    
    // --==<core_methods>==--
    void cmd_engine::run() {
        auto run_loop = [this]()->void {
            if (!init()) { return; }
            while (m_is_running) { update(); }
            quit();
        };
        m_thrRun = thread(run_loop);
    }
    bool cmd_engine::init()
    {
        if (m_is_running) { return false; }
        else { m_is_running = true; }

        set_title(&m_wnd_info.title[0]);
        if (!::SetConsoleMode(m_cin, NWCMD_INPUT_FLAGS)) { quit(); return false; }
     
        rect min_rect = { 0, 0, 1, 1 };
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &min_rect)) { quit(); return false; }
        if (!::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info)) { quit(); return false; }

        m_wnd_info.max_size = ::GetLargestConsoleWindowSize(m_cout);
        if (m_wnd_info.get_width() > m_wnd_info.max_size.X) { m_wnd_info.wnd_rect.Left = 0; m_wnd_info.wnd_rect.Right = m_wnd_info.max_size.X; }
        if (m_wnd_info.get_height() > m_wnd_info.max_size.Y) { m_wnd_info.wnd_rect.Top = 0; m_wnd_info.wnd_rect.Bottom = m_wnd_info.max_size.Y; }

        m_fmbuf.remake();
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect)) { quit(); return false; }
        if (!::SetConsoleActiveScreenBuffer(m_fmbuf.get_handle())) { quit(); return false; }

        return true;
    }
    void cmd_engine::quit()
    {
        if (!m_is_running) { return; }
        else { m_is_running = false; }

        ::SetConsoleActiveScreenBuffer(m_cout);
    }

    void cmd_engine::update()
    {
        swap_buffers();
        m_fmbuf.clear();
        poll_events();
    }

    void cmd_engine::event_proc(a_event& evt)
    {
        if (evt.is_in_category(EVC_MOUSE)) {
            ms_event& ms_evt = static_cast<ms_event&>(evt);
            m_mouse.event_proc(ms_evt);
            if (ms_evt.is_handled) { return; }
        }
        else if (evt.is_in_category(EVC_KEYBOARD)) {
            kbd_event& kbd_evt = static_cast<kbd_event&>(evt);
            m_kbd.event_proc(kbd_evt);
            if (kbd_evt.is_handled) { return; }
            if (m_kbd.is_held(KBC_LCTRL)) {
                if (m_kbd.is_held(KBC_M)) {
                    m_mouse.set_cursor_enabled(!m_mouse.is_cursor_enabled());
                }
                else if (m_kbd.is_held(KBC_ESCAPE)) {
                    stop_running();
                    evt.is_handled = true;
                }
            }
        }
        else if (evt.is_in_category(EVC_WINDOW)) {
            wnd_event& wnd_evt = static_cast<wnd_event&>(evt);
            if (wnd_evt.is_handled) { return; }
            if (wnd_evt.type == EVT_WND_CLOSE) {
                stop_running();
                evt.is_handled = true;
            }
        }
    }
    // --==</core_methods>==--

    // --==<drawing_methods>==--
    void cmd_engine::draw_line(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
        v1s16 delta_x = x1 - x0;
        v1s16 delta_y = y1 - y0;
        v1s16 dir_x = delta_x < 0 ? -1 : 1;
        v1s16 dir_y = delta_y < 0 ? -1 : 1;
        v1s16 err_val = 0;
        v1s16 delta_err = delta_y;
        for (v1s16 x = x0, y = y0; x != y1; x += dir_x) {
            m_fmbuf.set_pixel(x, y, pxl_draw);
            err_val += delta_err;
            if (err_val > delta_x) {
                y += dir_y;
                err_val -= (delta_x);
            }
        }
    }
    void cmd_engine::draw_rect(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
        v1s16 delta_x = x0 > x1 ? -1 : +1;
        v1s16 delta_y= y0 > y1 ? -1 : +1;
        y1 += delta_y ;
        for (v1s16 iy = y0; iy != y1; iy += delta_y) {
            for (v1s16 ix = x0; ix != x1; ix += delta_x) {
                m_fmbuf.set_pixel(ix, iy, pxl_draw);
            }
        }
    }
    void cmd_engine::draw_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, sbyte* str, size length) {
        m_fmbuf.set_bytes(x0, y0, x1, y1, color_val, str, length);
    }
    // --==</drawing_methods>==--
    
    // --==<implementation_methods>==--
    inline void cmd_engine::swap_buffers() {
        if (!WriteConsoleOutputW(m_fmbuf.get_handle(), &m_fmbuf.get_pxl_data()[0],
            { m_fmbuf.get_size_x(), m_fmbuf.get_size_y() }, { 0, 0 }, &m_fmbuf.get_info().srWindow)) { stop_running(); return; }
    }
    inline void cmd_engine::poll_events() {
        GetNumberOfConsoleInputEvents(m_cin, &m_evt_info.get_count);
        if (m_evt_info.get_count > 0) {
            ReadConsoleInputA(m_cin, m_evt_info.in_rec, m_evt_info.get_count, &m_evt_info.read_count);
            for (v1u16 evi = 0; evi < m_evt_info.get_count; evi++) {
                v1u evt_type_id = m_evt_info.in_rec[evi].EventType;
                if (evt_type_id == MOUSE_EVENT) {
                    MOUSE_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MouseEvent;
                    if (evt.dwEventFlags == MOUSE_MOVED) {
                        ms_event ms_evt(EVT_MS_MOVE, evt.dwMousePosition.X, evt.dwMousePosition.Y);
                        event_proc(ms_evt);
                    }
                    else {
                        for (v1u mi = 0; mi < MSC_COUNT; mi++) {
                            auto& btn = m_mouse[static_cast<mouse_codes>(mi)];
                            button_states old_state = btn.state;
                            btn.state = (((1 << mi) & evt.dwButtonState) > 0) ? BS_HELD : BS_FREE;
                            if (btn.state != old_state) {
                                ms_event ms_evt(btn.state == BS_HELD ?
                                    EVT_MS_PRESSED : EVT_MS_RELEASED, static_cast<mouse_codes>(mi));
                                event_proc(ms_evt);
                            }
                        }
                    }
                }
                else if (evt_type_id == KEY_EVENT) {
                    KEY_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.KeyEvent;
                    if (evt.bKeyDown) {
                        if (evt.wRepeatCount == 1) {
                            kbd_event kbd_evt(EVT_KBD_PRESSED, static_cast<keyboard_codes>(evt.wVirtualKeyCode));
                            event_proc(kbd_evt);
                        }
                    }
                    else {
                        kbd_event kbd_evt(EVT_KBD_RELEASED, static_cast<keyboard_codes>(evt.wVirtualKeyCode));
                        event_proc(kbd_evt);
                        if (evt.uChar.AsciiChar >= ' ' && evt.uChar.AsciiChar <= 'z') {
                            kbd_event kbEvt(EVT_KBD_CHAR, static_cast<keyboard_codes>(evt.uChar.AsciiChar));
                            event_proc(kbd_evt);
                        }
                        else {
                        }
                    }
                }
                else if (evt_type_id == FOCUS_EVENT) {
                    wnd_event wnd_evt(m_evt_info.in_rec[evi].Event.FocusEvent.bSetFocus ? EVT_WND_FOCUS : EVT_WND_DEFOCUS);
                    event_proc(wnd_evt);
                }
                else if (evt_type_id == WINDOW_BUFFER_SIZE_EVENT) {
                    wnd_event wnd_evt(EVT_WND_RESIZE, m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.X, m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.Y);
                    event_proc(wnd_evt);
                }
                else if (evt_type_id == MENU_EVENT) {
                    MENU_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MenuEvent;
                }
            }
        }
    }
    // --==</implementation_methods>==--
}