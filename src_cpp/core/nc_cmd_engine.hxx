#ifndef NC_CMD_CONSOLE_ENGINE_H
#	define NC_CMD_CONSOLE_ENGINE_H
#	include "nc_cmd_core.hpp"
#	if (defined NC_API)
#		include "std/nc_std_engine.hxx"
#		include "gfx/nc_cmd_buf.hxx"
struct NC_API cmd_window_info
{
public:
	dstr_t title;
	rect_t wnd_rect;
	xy_t max_size;
	v1bit_t is_focused;
public:
	inline v1s16 get_width() const  { return NC_NUM_ABS(wnd_rect.Right - wnd_rect.Left); }
	inline v1s16 get_height() const { return NC_NUM_ABS(wnd_rect.Bottom - wnd_rect.Top); }
	inline xy_t get_size() const    { return { get_width(), get_height() }; }
};
struct NC_API cmd_events_info
{
	DWORD get_count = 0;
	DWORD read_count = 0;
	INPUT_RECORD in_rec[64];
};
/// cmd_engine class
class cmd_engine : public std_engine_t<cmd_engine>
{
public:
	using nc_keybod_t = nc_keybod_t;
	using keybod_tc = const nc_keybod_t;
	using cursor_t = nc_cursor_t;
	using cursor_tc = const cursor_t;
	using fmbuf_t = cmd_fmbuf;
	using fmbuf_tc = const fmbuf_t;
public:
	cmd_engine();
	~cmd_engine();
	/* getters */
	inline fmbuf_tc* get_fmbuf() const   { return &m_fmbuf; }
	inline keybod_tc* get_keybod() const { return &m_keybod; }
	inline cursor_tc* get_cursor() const { return &m_cursor; }
	inline const cmd_window_info& get_wnd_info() const  { return m_wnd_info; }
	inline const cmd_pixel_info& get_pxl_info() const   { return m_pxl_info; }
	inline const cmd_events_info& get_evt_info() const  { return m_evt_info; }
	inline const cv1u16 get_wnd_size_x() const { return m_wnd_info.get_width(); }
	inline const cv1u16 get_wnd_size_y() const { return m_wnd_info.get_height(); }
	/* setters */
	v1nil_t set_wnd_size(v1s16_t width, v1s16_t height);
	v1nil_t set_title(cstr_t title);
	v1nil_t set_pxl_size(v1s16_t width, v1s16_t height);
	v1nil_t set_wnd_info(const cmd_window_info& info);
	v1nil_t set_pxl_info(const cmd_pixel_info& info);
	v1nil_t set_crs_info(const cmd_cursor_info& info);
	/* vetters */
	/* commands */
	virtual v1bit_t init() override;
	virtual v1bit_t quit() override;
	virtual v1bit_t work() override;
	v1bit_t event_proc(nc_event_t& evt);
	// --drawing_methods
	engine_t& draw_line(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw);
	engine_t& draw_rect(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw);
	engine_t& draw_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, byte_t* str, size_t length);
private:
	inline v1nil_t poll_events();
	inline v1nil_t swap_buffers();
private:
	cmd_fmbuf m_fmbuf;
	nc_keybod_t m_keybod;
	cursor_t m_cursor;
	ptr_t m_cout, m_cin;
	cmd_window_info m_wnd_info;
	cmd_pixel_info m_pxl_info;
	cmd_events_info m_evt_info;
	cmd_cursor_info m_mouse_info;
};
#	endif	/* NC_API */
#endif	/* NC_CMD_CONSOLE_ENGINE_H */