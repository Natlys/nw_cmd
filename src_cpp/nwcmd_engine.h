#ifndef NWCMD_CONSOLE_ENGINE_H
#define NWCMD_CONSOLE_ENGINE_H
#include <nwcmd_core.hpp>
#include <nwcmd_fmbuf.h>
namespace NWCMD
{
	struct NW_API cmd_window_info
	{
	public:
		dstr title = dstr("\0", 256);
		rect wnd_rect = { 0, 0, 1, 1 };
		v2xy max_size = { 1, 1 };
		v1b is_focused = false;
	public:
		inline v1s16 get_width() const  { return get_abs(wnd_rect.Right - wnd_rect.Left); }
		inline v1s16 get_height() const { return get_abs(wnd_rect.Bottom - wnd_rect.Top); }
		inline v2xy get_size() const    { return { get_width(), get_height() }; }
	};
	struct NW_API cmd_events_info
	{
		DWORD get_count = 0;
		DWORD read_count = 0;
		INPUT_RECORD in_rec[64];
	};
}
namespace NWCMD
{
	/// cmd_engine class
	class NW_API cmd_engine : public a_mem_user
	{
	public:
		cmd_engine();
		virtual ~cmd_engine();
		// --getters
		inline cmd_fmbuf* get_framebuf()                 { return &m_fmbuf; }
		inline const keyboard_state* get_keyboard() const   { return &m_kbd; }
		inline const mouse_state* get_cursor() const        { return &m_mouse; }
		inline const cmd_window_info& get_wnd_info() const  { return m_wnd_info; }
		inline const cmd_pixel_info& get_pxl_info() const   { return m_pxl_info; }
		inline const cmd_events_info& get_evt_info() const  { return m_evt_info; }
		inline const v1u16 get_wnd_size_x() const          { return m_wnd_info.get_width(); }
		inline const v1u16 get_wnd_size_y() const          { return m_wnd_info.get_height(); }
		// --setters
		void stop_running();
		void set_wnd_size(v1u16 width, v1u16 height);
		void set_title(cstr title);
		void set_pxl_size(v1u16 width, v1u16 height);
		void set_wnd_info(const cmd_window_info& info);
		void set_pxl_info(const cmd_pixel_info& info);
		void set_crs_info(const cmd_cursor_info& info);
		// --predicates
		inline v1b is_running() const { return m_is_running; }
		// --core_methods
		void run();
		bool init();
		void quit();
		void update();
		void event_proc(a_event& evt);
		// --drawing_methods
		void draw_line(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw);
		void draw_rect(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw);
		void draw_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, sbyte* str, size length);
	private:
		inline void poll_events();
		inline void swap_buffers();
	private:
		thread m_thrRun;
		v1b m_is_running;
		cmd_fmbuf m_fmbuf;
		keyboard_state m_kbd;
		mouse_state m_mouse;
		ptr m_cout, m_cin;
		cmd_window_info m_wnd_info;
		cmd_pixel_info m_pxl_info;
		cmd_events_info m_evt_info;
		cmd_cursor_info m_mouse_info;
	};
}
#endif	// NWCMD_CONSOLE_ENGINE_H
