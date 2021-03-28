#ifndef NWCMD_CORE_HPP
#define NWCMD_CORE_HPP
// --==<configuration>==--
// --==</configuration>==--
#include <nwlib.hpp>
namespace NWCMD { using namespace NWLIB; }
// --<type_alias>--
namespace NWCMD
{
	class NW_API cmd_engine;
	class NW_API cmd_fmbuf;

	using cmd_history_info = CONSOLE_HISTORY_INFO;
	using cmd_selection_info = CONSOLE_SELECTION_INFO;
	using cmd_fmbuf_info = CONSOLE_SCREEN_BUFFER_INFOEX;
	using cmd_pixel_info = CONSOLE_FONT_INFOEX;
	using cmd_cursor_info = CONSOLE_CURSOR_INFO;
	using cmd_pixel = CHAR_INFO;
	using v2xy = COORD;
	using rect = SMALL_RECT;
}
// --</type_alias>--

namespace NWCMD
{
	/// 0xFF'FF'FF'FF -> 32 bits
	/// counting from left:
	/// 4 bits - red; 4 bits - blue; 4 bits - green; 4 bits - alpha
	enum cmd_pixel_types : wchar {
		PXT_DEFAULT = 0x00'00,
		PXT_DITHER = 0x24'89,
		PXT_1_4 = 0x25'91,
		PXT_2_4 = 0x25'92,
		PXT_3_4 = 0x25'93,
		PXT_SOLID = 0x25'88,
	};
	/// 0b0000'0000 -> 8 bits; 0x00 -> 8 bits
	/// right 4 bits - background -> 16 variations; right digit - back
	/// left 4 bits - foreground -> 16 variations; left digit - char
	enum cmd_colors_: v1u16 {
		COLOR_FG_BLACK =        0x00,
		COLOR_FG_DARK_BLUE =    0x01,
		COLOR_FG_DARK_GREEN =   0x02,
		COLOR_FG_DARK_CYAN =    0x03,
		COLOR_FG_DARK_RED =     0x04,
		COLOR_FG_DARK_MAGENTA = 0x05,
		COLOR_FG_DARK_YELLOW =  0x06,
		COLOR_FG_GREY =         0x07,
		COLOR_FG_DARK_GREY =    0x08,
		COLOR_FG_BLUE =         0x09,
		COLOR_FG_GREEN =        0x0A,
		COLOR_FG_CYAN =         0x0B,
		COLOR_FG_RED =          0x0C,
		COLOR_FG_MAGENTA =      0x0D,
		COLOR_FG_YELLOW =       0x0E,
		COLOR_FG_WHITE =        0x0F,
		COLOR_BG_BLACK =        0x00,
		COLOR_BG_DARK_BLUE =    0x10,
		COLOR_BG_DARK_GREEN =   0x20,
		COLOR_BG_DARK_CYAN =    0x30,
		COLOR_BG_DARK_RED =     0x40,
		COLOR_BG_DARK_MAGENTA = 0x50,
		COLOR_BG_DARK_YELLOW =  0x60,
		COLOR_BG_GREY =         0x70,
		COLOR_BG_DARK_GREY =    0x80,
		COLOR_BG_BLUE =         0x90,
		COLOR_BG_GREEN =        0xA0,
		COLOR_BG_CYAN =         0xB0,
		COLOR_BG_RED =          0xC0,
		COLOR_BG_MAGENTA =      0xD0,
		COLOR_BG_YELLOW =       0xE0,
		COLOR_BG_WHITE =        0xF0,

		COLOR_FG_1 = 0x00,	COLOR_FG_2 = 0x01,	COLOR_FG_3 = 0x02,	COLOR_FG_4 = 0x03,
		COLOR_FG_5 = 0x04,	COLOR_FG_6 = 0x05,	COLOR_FG_7 = 0x06,	COLOR_FG_8 = 0x07,
		COLOR_FG_9 = 0x08,	COLOR_FG_10 = 0x09,	COLOR_FG_11 = 0x0A,	COLOR_FG_12 = 0x0B,
		COLOR_FG_13 = 0x0C,	COLOR_FG_14 = 0x0D,	COLOR_FG_15 = 0x0E,	COLOR_FG_16 = 0x0F,

		COLOR_BG_1 = 0x00,	COLOR_BG_2 = 0x10,	COLOR_BG_3 = 0x20,	COLOR_BG_4 = 0x30,
		COLOR_BG_5 = 0x40,	COLOR_BG_6 = 0x50,	COLOR_BG_7 = 0x60,	COLOR_BG_8 = 0x70,
		COLOR_BG_9 = 0x80,	COLOR_BG_10 = 0x90,	COLOR_BG_11 = 0xA0,	COLOR_BG_12 = 0xB0,
		COLOR_BG_13 = 0xC0,	COLOR_BG_14 = 0xD0,	COLOR_BG_15 = 0xE0,	COLOR_BG_16 = 0xF0,
	};
	typedef v1u16 cmd_colors;
}
#endif	// NWCMD_CORE_HPP