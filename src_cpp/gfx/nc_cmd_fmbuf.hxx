#ifndef NC_CMD_FRAMEBUFFER_H
#define NC_CMD_FRAMEBUFFER_H
#include "nc_cmd_core.hpp"
#if (defined NC_WAPI)
namespace NC
{
	/// cmd_fmbuf class
	class cmd_fmbuf
	{
	public:
		using handle_t = HANDLE;
		using handle_tc = const handle_t;
		using info_t = cmd_fmbuf_info;
		using info_tc = const info_t;
	public:
		cmd_fmbuf();
		~cmd_fmbuf();
		/* getters */
		inline v1s16 get_size_x() const  { return static_cast<v1s16>(m_info.dwSize.X); }
		inline v1s16 get_size_y() const  { return static_cast<v1s16>(m_info.dwSize.Y); }
		inline size_t get_pxl_count()    { return m_pxl_count; }
		inline cmd_pixel* get_pxl_data() { return m_pxl_data; }
		inline handle_t get_handle()        { return m_cout; }
		inline handle_tc get_handle() const { return m_cout; }
		inline info_t& get_info()       { return m_info; }
		inline info_tc& get_info() const{ return m_info; }
		/* setters */
		v1nil_t set_size(v1s16_t width, v1s16_t height);
		v1nil_t set_clear_value(cmd_colors clear_value, cmd_pixel_types pxl_type);
		v1nil_t set_pixel(size_t x, cmd_pixel pxl);
		v1nil_t set_pixel(size_t x, cmd_colors color_val, cmd_pixel_types pxType);
		v1nil_t set_pixel(v1s16 x, v1s16 y, cmd_pixel pxl);
		v1nil_t set_pixel(v1s16 x, v1s16 y, cmd_colors color_val, cmd_pixel_types pxl_type = PXT_SOLID);
		v1nil_t set_byte(v1s16 x, v1s16 y, cmd_colors color_val, sbyte_t bt);
		v1nil_t set_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, byte_t* source, size_t length = 1);
		/* commands */
		v1nil_t remake();
		v1nil_t clear();
	private:
		handle_t m_cout;
		size_t m_pxl_count;
		cmd_pixel* m_pxl_data;
		cmd_pixel m_pxl_clear;
		info_t m_info;
	};
}
#endif	// NC_WAPI
#endif	// NC_CMD_FRAMEBUFFER_H