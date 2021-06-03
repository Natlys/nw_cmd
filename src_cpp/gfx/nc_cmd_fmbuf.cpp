#include "nc_cmd_pch.hpp"
#include "nc_cmd_buf.hxx"
#if (defined NC_WAPI)
#include "core/nc_cmd_eng.hxx"
namespace NC
{
	cmd_fmbuf::cmd_fmbuf() :
		m_cout(NC_NULL),
		m_pxl_count(0), m_pxl_data(NC_NULL),
		m_pxl_clear(cmd_pixel{ PXT_SOLID, COLOR_BG_1 | COLOR_FG_1 }),
		m_info(info_t())
	{
		m_cout = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	cmd_fmbuf::~cmd_fmbuf() { if (m_cout != NC_NULL) { CloseHandle(m_cout); m_cout = NC_NULL; } }
    /* setters */
	v1nil_t cmd_fmbuf::set_size(v1s16_t width, v1s16_t height) {
		m_info.dwSize = { static_cast<v1s16>(width), static_cast<v1s16>(height) };
	}
	v1nil_t cmd_fmbuf::set_clear_value(cmd_colors clear_value, cmd_pixel_types pxl_type) {
		m_pxl_clear.Char.UnicodeChar = pxl_type;
		m_pxl_clear.Attributes = clear_value;
	}
	v1nil_t cmd_fmbuf::set_pixel(size_t x, cmd_pixel pxl_draw) {
		if (x + 1 >= m_pxl_count) { return; }
		v1s16_t width = get_size_x();
		m_pxl_data[x++] = pxl_draw;
		if ((x % width) == 0) { return; }
		m_pxl_data[x] = pxl_draw;
	}
	v1nil_t cmd_fmbuf::set_pixel(size_t x, cmd_colors color_val, cmd_pixel_types pxl_type) {
		if (x + 1 >= m_pxl_count) { return; }
		v1s16_t width = get_size_y();
		cmd_pixel cpx;
		cpx.Attributes = color_val;
		cpx.Char.UnicodeChar = pxl_type;
		m_pxl_data[x++] = cpx;
		if ((x % width) == 0) { return; }
		m_pxl_data[x] = cpx;
	}
	v1nil_t cmd_fmbuf::set_pixel(v1s16 x, v1s16 y, cmd_pixel pxl_draw) {
		if (x < 0 || y < 0) { return; }
		v1s16_t width = get_size_x();
		if (x > width) { return; }
		set_pixel(NC_NUM_2D_1D(x, y, width), pxl_draw);
	}
	v1nil_t cmd_fmbuf::set_pixel(v1s16 x, v1s16 y, cmd_colors color_val, cmd_pixel_types pxl_type) {
		if (x < 0) { return; }
		v1s16_t width = get_size_x();
		if (x > width) { return; }
		set_pixel(NC_NUM_2D_1D(x, y, width), color_val, pxl_type);
	}
	v1nil_t cmd_fmbuf::set_byte(v1s16 x, v1s16 y, cmd_colors color_val, sbyte_t bt)
	{
		if (x < 0 || y < 0) { return; }
		v1s16_t width = get_size_x();
		if (x > width) { return; }
		size_t crd = NC_NUM_2D_1D(x, y, width);
		if (crd >= m_pxl_count) { return; }
		m_pxl_data[crd].Attributes = color_val;
		m_pxl_data[crd].Char.AsciiChar = bt;
		m_pxl_data[crd].Char.UnicodeChar = bt;
	}
	v1nil_t cmd_fmbuf::set_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, byte_t* str, size_t length) {
		if (x0 > x1) { x0 = x0 - x1; x1 = x0 + x1; x1 = x0 - x1; }
		if (y0 > y1) { y0 = y0 - y1; y1 = y0 + y1; y1 = y0 - y1; }
		for (v1s_t ci = 0, ix = x0; ci < length && y0 != y1; ci++, ix++) {
			if (ix > x1 || str[ci] == '\n') { ix = 0; y0 += 1; }
			else { set_byte(ix, y0, color_val, str[ci]); }
		}
	}
    /* commands */
    v1nil_t cmd_fmbuf::remake() {
		if (m_pxl_data != NC_NULL) {
			mem_sys::get().del_arr<cmd_pixel>(m_pxl_data, m_pxl_count);
			m_pxl_data = NC_NULL;
			m_pxl_count = 0;
		}

		xy_t size_wh = { cmd_engine::get().get_wnd_size_x() + 1, cmd_engine::get().get_wnd_size_y() + 1 };

		if (m_info.dwSize.X < size_wh.X) { m_info.dwSize.X = size_wh.X; }
		if (m_info.dwSize.Y < size_wh.Y) { m_info.dwSize.Y = size_wh.Y; }
		m_info.srWindow = { 0, 0, get_size_x(), get_size_y() };
		m_info.bFullscreenSupported = true;
		m_info.wAttributes = COLOR_BG_BLACK | COLOR_FG_GREEN;
		m_info.dwCursorPosition = xy_t{ 0, 0 };
		m_info.dwMaximumWindowSize = GetLargestConsoleWindowSize(m_cout);
		//for (ui8 cli = 0; cli < 16; cli++) { m_Info.ColorTable[cli] = 0x11'11'11 * cli; }
		m_info.ColorTable[0] = 0x00'00'00;
		m_info.ColorTable[1] = 0x00'00'7F;
		m_info.ColorTable[2] = 0x00'7F'00;
		m_info.ColorTable[3] = 0x00'7F'7F;
		m_info.ColorTable[4] = 0x7F'00'00;
		m_info.ColorTable[5] = 0x7F'00'7F;
		m_info.ColorTable[6] = 0x7F'7F'00;
		m_info.ColorTable[7] = 0x7F'7F'7F;

		m_info.ColorTable[8] = 0x00'00'00;
		m_info.ColorTable[9] = 0x00'00'FF;
		m_info.ColorTable[10] = 0x00'FF'00;
		m_info.ColorTable[11] = 0x00'FF'FF;
		m_info.ColorTable[12] = 0xFF'00'00;
		m_info.ColorTable[13] = 0xFF'00'FF;
		m_info.ColorTable[14] = 0xFF'FF'00;
		m_info.ColorTable[15] = 0xFF'FF'FF;

		m_info.cbSize = sizeof(cmd_fmbuf_info);

		m_pxl_count = static_cast<size_t>(get_size_x()) * static_cast<size_t>(get_size_y());
		m_pxl_data = mem_sys::get().new_arr<cmd_pixel>(m_pxl_count);
		memset(m_pxl_data, 0, m_pxl_count * sizeof(cmd_pixel));
	
		if (!SetConsoleScreenBufferInfoEx(m_cout, &m_info)) { return; }
		if (!SetConsoleScreenBufferSize(m_cout, m_info.dwSize)) { return; }
	}
	v1nil_t cmd_fmbuf::clear()
	{
		for (size_t ipxl = 0; ipxl < m_pxl_count; ipxl++) { m_pxl_data[ipxl] = m_pxl_clear; }
	}
    // --==</core_methods>==--
}
#endif	// NC_WAPI