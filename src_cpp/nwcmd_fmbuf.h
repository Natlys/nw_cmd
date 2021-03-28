#ifndef NWCMD_FRAMEBUFFER_H
#define NWCMD_FRAMEBUFFER_H
#include <nwcmd_core.hpp>
namespace NWCMD
{
	/// cmd_fmbuf class
	class NW_API cmd_fmbuf
	{
	public:
		cmd_fmbuf(cmd_engine& engine);
		~cmd_fmbuf();
		// --getters
		inline v1s16 get_size_x() const { return static_cast<v1s16>(m_info.dwSize.X); }
		inline v1s16 get_size_y() const { return static_cast<v1s16>(m_info.dwSize.Y); }
		inline cmd_pixel* get_pxl_data() { return m_pxl_data; }
		inline size get_pxl_count() { return m_pxl_count; }
		inline ptr get_handle() { return m_cout; }
		inline cmd_fmbuf_info& get_info() { return m_info; }
		// --setters
		void set_size(v1u16 width, v1u16 height);
		void set_clear_value(cmd_colors clear_value, cmd_pixel_types pxl_type);
		void set_pixel(size x, cmd_pixel pxl);
		void set_pixel(size x, cmd_colors color_val, cmd_pixel_types pxType);
		void set_pixel(v1s16 x, v1s16 y, cmd_pixel pxl);
		void set_pixel(v1s16 x, v1s16 y, cmd_colors color_val, cmd_pixel_types pxl_type = PXT_SOLID);
		void set_byte(v1s16 x, v1s16 y, cmd_colors color_val, sbyte bt);
		void set_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, sbyte* source, size length = 1);
		// --core_methods
		void remake();
		void clear();
	private:
		cmd_engine* m_engine;
		ptr m_cout;
		size m_pxl_count;
		cmd_pixel* m_pxl_data;
		cmd_pixel m_pxl_clear;
		cmd_fmbuf_info m_info;
	};
}
#endif	// NWCMD_FRAMEBUFFER_H
