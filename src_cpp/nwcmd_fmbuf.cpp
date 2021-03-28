#include "nwcmd_fmbuf.h"
#include <nwcmd_engine.h>
namespace NWCMD
{
	cmd_fmbuf::cmd_fmbuf(cmd_engine& engine) :
		m_engine(&engine), m_cout(nullptr),
		m_pxl_count(0), m_pxl_data(nullptr),
		m_pxl_clear(cmd_pixel{ PXT_SOLID, COLOR_BG_1 | COLOR_FG_1 }),
		m_info(cmd_fmbuf_info())
	{
		m_cout = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	cmd_fmbuf::~cmd_fmbuf() { if (m_cout != nullptr) { CloseHandle(m_cout); m_cout = nullptr; } }
    // --setters
	void cmd_fmbuf::set_size(v1u16 width, v1u16 height) {
		m_info.dwSize = { static_cast<v1s16>(width), static_cast<v1s16>(height) };
	}
	void cmd_fmbuf::set_clear_value(cmd_colors clear_value, cmd_pixel_types pxl_type) {
		m_pxl_clear.Char.UnicodeChar = pxl_type;
		m_pxl_clear.Attributes = clear_value;
	}
	void cmd_fmbuf::set_pixel(size x, cmd_pixel pxl_draw) {
		if (x + 1 >= m_pxl_count) { return; }
		v1u16 width = get_size_x();
		m_pxl_data[x++] = pxl_draw;
		if ((x % width) == 0) { return; }
		m_pxl_data[x] = pxl_draw;
	}
	void cmd_fmbuf::set_pixel(size x, cmd_colors color_val, cmd_pixel_types pxl_type) {
		if (x + 1 >= m_pxl_count) { return; }
		v1u16 width = get_size_y();
		cmd_pixel cpx;
		cpx.Attributes = color_val;
		cpx.Char.UnicodeChar = pxl_type;
		m_pxl_data[x++] = cpx;
		if ((x % width) == 0) { return; }
		m_pxl_data[x] = cpx;
	}
	void cmd_fmbuf::set_pixel(v1s16 x, v1s16 y, cmd_pixel pxl_draw) {
		if (x < 0 || y < 0) { return; }
		v1u16 width = get_size_x();
		if (x > width) { return; }
		set_pixel(NW_XY_TO_X(x, y, width), pxl_draw);
	}
	void cmd_fmbuf::set_pixel(v1s16 x, v1s16 y, cmd_colors color_val, cmd_pixel_types pxl_type) {
		if (x < 0) { return; }
		v1u16 width = get_size_x();
		if (x > width) { return; }
		set_pixel(NW_XY_TO_X(x, y, width), color_val, pxl_type);
	}
	void cmd_fmbuf::set_byte(v1s16 x, v1s16 y, cmd_colors color_val, sbyte bt)
	{
		if (x < 0 || y < 0) { return; }
		v1u16 width = get_size_x();
		if (x > width) { return; }
		size crd = NW_XY_TO_X(x, y, width);
		if (crd >= m_pxl_count) { return; }
		m_pxl_data[crd].Attributes = color_val;
		m_pxl_data[crd].Char.UnicodeChar = static_cast<schar>(bt);
	}
	void cmd_fmbuf::set_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, sbyte* str, size length) {
		if (x0 > x1) { x0 = x0 - x1; x1 = x0 + x1; x1 = x0 - x1; }
		if (y0 > y1) { y0 = y0 - y1; y1 = y0 + y1; y1 = y0 - y1; }
		for (v1s ci = 0, ix = x0; ci < length && y0 != y1; ci++, ix++) {
			if (ix > x1 || str[ci] == '\n') { ix = 0; y0 += 1; }
			else { set_byte(ix, y0, color_val, str[ci]); }
		}
	}
    // --==<core_methods>==--
    void cmd_fmbuf::remake() {
		if (m_pxl_data != nullptr) {
			mem_sys::get().del_arr<cmd_pixel>(m_pxl_data, m_pxl_count);
			m_pxl_data = nullptr;
			m_pxl_count = 0;
		}

		v2xy size_wh = { m_engine->get_wnd_size_x() + 1, m_engine->get_wnd_size_y() + 1 };

		if (m_info.dwSize.X < size_wh.X) { m_info.dwSize.X = size_wh.X; }
		if (m_info.dwSize.Y < size_wh.Y) { m_info.dwSize.Y = size_wh.Y; }
		m_info.srWindow = { 0, 0, get_size_x(), get_size_y() };
		m_info.bFullscreenSupported = true;
		m_info.wAttributes = COLOR_BG_BLACK | COLOR_FG_GREEN;
		m_info.dwCursorPosition = v2xy{ 0, 0 };
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

		m_pxl_count = static_cast<size>(get_size_x()) * static_cast<size>(get_size_y());
		m_pxl_data = mem_sys::get().new_arr<cmd_pixel>(m_pxl_count);
		memset(m_pxl_data, 0, m_pxl_count * sizeof(cmd_pixel));
	
		if (!SetConsoleScreenBufferInfoEx(m_cout, &m_info)) { return; }
		if (!SetConsoleScreenBufferSize(m_cout, m_info.dwSize)) { return; }
	}
	void cmd_fmbuf::clear()
	{
		for (size ipxl = 0; ipxl < m_pxl_count; ipxl++) { m_pxl_data[ipxl] = m_pxl_clear; }
	}
    // --==</core_methods>==--
}