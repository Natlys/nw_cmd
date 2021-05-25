#include "nc_cmd_pch.hpp"
#include "nc_cmd_eng.hxx"
#if (defined NC_WAPI)
#   define NC_CMD_INPUT_FLAGS ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT
cmd_engine::cmd_engine() :
    m_fmbuf(fmbuf_t()),
    m_wnd_info(cmd_window_info()),
    m_pxl_info(cmd_pixel_info()),
    m_cout(NC_NULL), m_cin(NC_NULL)
{
    this->set_wnd_size(120, 40);
    this->set_pxl_size(8, 16);
    m_fmbuf.set_size(get_wnd_size_x(), get_wnd_size_y());
    m_cout = GetStdHandle(STD_OUTPUT_HANDLE);
    m_cin = GetStdHandle(STD_INPUT_HANDLE);
    m_wnd_info.title = "console_window";
}
cmd_engine::~cmd_engine() { }
/* setters */
v1nil_t cmd_engine::set_title(cstr_t title) { m_wnd_info.title = title; ::SetConsoleTitleA(&title[0]); }
v1nil_t cmd_engine::set_wnd_size(v1s16_t uwidth, v1s16_t uheight) {
    m_wnd_info.wnd_rect.Left = m_wnd_info.wnd_rect.Top = 0;
    m_wnd_info.wnd_rect.Right = uwidth; m_wnd_info.wnd_rect.Bottom = uheight;
}
v1nil_t cmd_engine::set_pxl_size(v1s16_t width, v1s16_t height) {
    /* init */
    /* work */
    m_pxl_info.dwFontSize.X = width;
    m_pxl_info.dwFontSize.Y = height;
    m_pxl_info.FontFamily = FF_DONTCARE;
    m_pxl_info.FontWeight = FW_NORMAL;
    m_pxl_info.nFont = 0;
    swprintf_s(m_pxl_info.FaceName, L"Consolas");
    m_pxl_info.cbSize = sizeof(m_pxl_info);
    /* quit */
}
v1nil_t cmd_engine::set_wnd_info(const cmd_window_info& info) { m_wnd_info = info; ::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect); }
v1nil_t cmd_engine::set_pxl_info(const cmd_pixel_info& rcpxInfo) { m_pxl_info = rcpxInfo; ::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info); }
v1nil_t cmd_engine::set_crs_info(const cmd_cursor_info& rcrsInfo) { m_mouse_info = rcrsInfo; ::SetConsoleCursorInfo(m_fmbuf.get_handle(), &m_mouse_info); }
/* commands */
v1bit_t cmd_engine::init() {
    NC_PCALL({ /* init */
    }, "init error!", return NC_FALSE);
    NC_PCALL({ /* work */
        set_title(&m_wnd_info.title[0]);
        if (!::SetConsoleMode(m_cin, NC_CMD_INPUT_FLAGS)) { return NC_TRUTH; }
        rect_t min_rect = { 0, 0, 1, 1 };
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &min_rect)) { return NC_TRUTH; }
        if (!::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info)) { return NC_TRUTH; }
        m_wnd_info.max_size = ::GetLargestConsoleWindowSize(m_cout);
        if (m_wnd_info.get_width() > m_wnd_info.max_size.X) { m_wnd_info.wnd_rect.Left = 0; m_wnd_info.wnd_rect.Right = m_wnd_info.max_size.X; }
        if (m_wnd_info.get_height() > m_wnd_info.max_size.Y) { m_wnd_info.wnd_rect.Top = 0; m_wnd_info.wnd_rect.Bottom = m_wnd_info.max_size.Y; }
        m_fmbuf.remake();
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect)) { return NC_TRUTH; }
        if (!::SetConsoleActiveScreenBuffer(m_fmbuf.get_handle())) { return NC_TRUTH; }
        NC_CHECK(std_engine_t::init(), "init error!", return NC_FALSE);
    }, "init error!", return NC_FALSE);
    /* quit */
    return NC_TRUTH;
}
v1bit_t cmd_engine::quit()
{
    NC_PCALL({ /* init */
        NC_CHECK(std_engine_t::quit(), "quit error!", return NC_FALSE);
    }, "quit error!", return NC_FALSE);
    NC_PCALL({ /* work */
        ::SetConsoleActiveScreenBuffer(m_cout);
    }, "quit error!", return NC_FALSE);
    /* quit */
    return NC_TRUTH;
}
v1bit_t cmd_engine::work()
{
    NC_PCALL({ /* init */
    }, "work error!", return NC_FALSE);
    NC_PCALL({ /* init */
        swap_buffers();
        m_fmbuf.clear();
        poll_events();
        NC_CHECK(std_engine_t::work(), "work error!", return NC_FALSE);
    }, "work error!", return NC_FALSE);
    /* quit */
    return NC_TRUTH;
}
v1bit_t cmd_engine::event_proc(nc_event_t& evt)
{
    if (evt.has_sort(NC_EVSORT_APPLIC)) {
        cursor_t::nc_event_t& crs_evt = static_cast<cursor_t::nc_event_t&>(evt);
        m_cursor.event_proc(crs_evt);
        if (crs_evt.is_handled()) { return NC_FALSE; }
    }
    else if (evt.has_sort(NC_EVSORT_KEYBOD)) {
        nc_keybod_t::nc_event_t& kbd_evt = static_cast<nc_keybod_t::nc_event_t&>(evt);
        m_keybod.event_proc(kbd_evt);
        if (kbd_evt.is_handled()) { return NC_FALSE; }
        if (m_keybod.is_held(NC_KEYCODE_CONTR)) {
            if (m_keybod.is_held(NC_KEYCODE_M)) {
                m_cursor.set_enabled(!m_cursor.is_enabled());
            }
            else if (m_keybod.is_held(NC_KEYCODE_ESCP)) {
                work_quit();
                evt.set_handled(NC_TRUTH);
            }
        }
    }
    else if (evt.has_sort(NC_EVSORT_WINDOW)) {
        nc_event_window_t& wnd_evt = static_cast<nc_event_window_t&>(evt);
        if (wnd_evt.is_handled()) { return NC_FALSE; }
        if (wnd_evt.has_type(NC_EVTYPE_WINDOW_CLOSE)) {
            work_quit();
            evt.set_handled(NC_TRUTH);
        }
    }
    return NC_TRUTH;
}
// drawing //
cmd_engine::engine_t& cmd_engine::draw_line(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
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
    return *this;
}
cmd_engine::engine_t& cmd_engine::draw_rect(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
    v1s16 delta_x = x0 > x1 ? -1 : +1;
    v1s16 delta_y= y0 > y1 ? -1 : +1;
    y1 += delta_y ;
    for (v1s16 iy = y0; iy != y1; iy += delta_y) {
        for (v1s16 ix = x0; ix != x1; ix += delta_x) {
            m_fmbuf.set_pixel(ix, iy, pxl_draw);
        }
    }
    return *this;
}
cmd_engine::engine_t& cmd_engine::draw_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, byte_t* str, size_t length) {
    m_fmbuf.set_bytes(x0, y0, x1, y1, color_val, str, length);
    return *this;
}
// --==<implementation_methods>==--
inline v1nil_t cmd_engine::swap_buffers() {
    if (!WriteConsoleOutputW(m_fmbuf.get_handle(), &m_fmbuf.get_pxl_data()[0],
        { m_fmbuf.get_size_x(), m_fmbuf.get_size_y() }, { 0, 0 }, &m_fmbuf.get_info().srWindow)) { work_quit(); return; }
}
inline v1nil_t cmd_engine::poll_events() {
    GetNumberOfConsoleInputEvents(m_cin, &m_evt_info.get_count);
    if (m_evt_info.get_count > 0) {
        ReadConsoleInputA(m_cin, m_evt_info.in_rec, m_evt_info.get_count, &m_evt_info.read_count);
        for (v1s16_t evi = 0; evi < m_evt_info.get_count; evi++) {
            v1u_t evt_type_id = m_evt_info.in_rec[evi].EventType;
            if (evt_type_id == MOUSE_EVENT) {
                MOUSE_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MouseEvent;
                if (evt.dwEventFlags == MOUSE_MOVED) {
                    cursor_t::nc_event_t crs_evt(NC_EVTYPE_CURSOR_COORD, evt.dwMousePosition.X, evt.dwMousePosition.Y);
                    event_proc(crs_evt);
                }
                else {
                    for (v1u_t itr = 0; itr < NC_CURCODE_COUNT; itr++) {
                        auto& ibtn = m_cursor[itr];
                        auto& old_state = ibtn.m_state;
                        ibtn.m_state = ( ((1 << itr) & evt.dwButtonState) > 0 ) ? NC_BUTTON_HELD : NC_BUTTON_FREE;
                        if (ibtn.m_state != old_state) {
                            cursor_t::nc_event_t crs_evt(ibtn.m_state == NC_BUTTON_HELD ? NC_BUTTON_PRESS : NC_BUTTON_RAISE, itr);
                            event_proc(crs_evt);
                        }
                    }
                }
            }
            else if (evt_type_id == KEY_EVENT) {
                KEY_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.KeyEvent;
                if (evt.bKeyDown) {
                    if (evt.wRepeatCount == 1) {
                        nc_keybod_t::nc_event_t kbd_evt(NC_EVTYPE_KEYBOD_PRESS, evt.wVirtualKeyCode);
                        event_proc(kbd_evt);
                    }
                }
                else {
                    nc_event_keybod_t kbd_evt(NC_EVTYPE_KEYBOD_RAISE, evt.wVirtualKeyCode);
                    event_proc(kbd_evt);
                    if (evt.uChar.AsciiChar >= ' ' && evt.uChar.AsciiChar <= 'z') {
                        nc_keybod_t::nc_event_t kbEvt(NC_EVTYPE_KEYBOD_CHART, evt.uChar.AsciiChar);
                        event_proc(kbd_evt);
                    }
                    else {
                        nc_keybod_t::nc_event_t kbEvt(NC_EVTYPE_KEYBOD_CHART, evt.uChar.UnicodeChar);
                        event_proc(kbd_evt);
                    }
                }
            }
            else if (evt_type_id == FOCUS_EVENT) {
                nc_event_window_t wnd_evt(NC_EVTYPE_WINDOW_FOCUS, m_evt_info.in_rec[evi].Event.FocusEvent.bSetFocus);
                event_proc(wnd_evt);
            }
            else if (evt_type_id == WINDOW_BUFFER_SIZE_EVENT) {
                nc_event_window_t wnd_evt(NC_EVTYPE_WINDOW_SIZED, m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.X, m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.Y);
                event_proc(wnd_evt);
            }
            else if (evt_type_id == MENU_EVENT) {
                MENU_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MenuEvent;
            }
        }
    }
}
#endif	/* NC_WAPI */
/* end_of_file */