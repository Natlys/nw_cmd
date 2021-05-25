#ifndef NC_CMD_CORE_H
#	define NC_CMD_CORE_H
/* config */
#	if (defined NC_BUILD)
#		undef NC_BUILD
#	endif	/* NC_BUILD */
#	define NC_BUILD NC_BUILD_LIB
/* includes */
#	include "nc_cmd_pch.hxx"
// linkage //
#   if (defined NC_API)
#       undef NC_API
#   endif   /* NC_API */
#   if !(defined NC_BUILD)
#   elif (NC_BUILD & NC_BUILD_EXE)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_LIB)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_DLL)
#       define NC_API NC_DLL_EXPORT
#   else
#       define NC_API NC_DLL_IMPORT
#   endif	/* NC_BUILD */
// types //
typedef CHAR_INFO             nc_cmd_pxl_t;
typedef const nc_cmd_pxl_t   nc_cmd_pxl_tc;
typedef SHORT                 nc_cmd_v1u_t;
typedef const nc_cmd_v1u_t   nc_cmd_v1u_tc;
typedef COORD                 nc_cmd_v2u_t;
typedef const nc_cmd_v2u_t   nc_cmd_v2u_tc;
typedef SMALL_RECT            nc_cmd_v4u_t;
typedef const nc_cmd_v2u_t   nc_cmd_v4u_tc;
typedef CONSOLE_FONT_INFOEX            nc_cmd_pxl_info_t;
typedef const nc_cmd_pxl_info_t       nc_cmd_pxl_info_tc;
typedef CONSOLE_SCREEN_BUFFER_INFOEX   nc_cmd_buf_info_t;
typedef const nc_cmd_buf_info_t       nc_cmd_buf_info_tc;
typedef CONSOLE_SELECTION_INFO         nc_cmd_sel_info_t;
typedef const nc_cmd_sel_info_t       nc_cmd_sel_info_tc;
/* defines */
/// 0xFF'FF'FF'FF -> 32 bits
/// counting from left:
/// 4 bits - red; 4 bits - blue; 4 bits - green; 4 bits - alpha
#define NC_CMD_PXT_1_4 0x2591
#define NC_CMD_PXT_2_4 0x2592
#define NC_CMD_PXT_3_4 0x2593
#define NC_CMD_PXT_MIX 0x2489
#define NC_CMD_PXT_BOX 0x2588
/// 0b0000'0000 -> 8 bits; 0x00 -> 8 bits
/// right 4 bits - background -> 16 variations; right digit - back
/// left 4 bits - foreground -> 16 variations; left digit - char
// console_colors //
// // titled // //
// // foreground // //
#define NC_CMD_COLOR_FG_BLACK        0x00
#define NC_CMD_COLOR_FG_DARK_BLUE    0x01
#define NC_CMD_COLOR_FG_DARK_GREEN   0x02
#define NC_CMD_COLOR_FG_DARK_CYAN    0x03
#define NC_CMD_COLOR_FG_DARK_RED     0x04
#define NC_CMD_COLOR_FG_DARK_MAGENTA 0x05
#define NC_CMD_COLOR_FG_DARK_YELLOW  0x06
#define NC_CMD_COLOR_FG_GREY         0x07
#define NC_CMD_COLOR_FG_DARK_GREY    0x08
#define NC_CMD_COLOR_FG_BLUE         0x09
#define NC_CMD_COLOR_FG_GREEN        0x0A
#define NC_CMD_COLOR_FG_CYAN         0x0B
#define NC_CMD_COLOR_FG_RED          0x0C
#define NC_CMD_COLOR_FG_MAGENTA      0x0D
#define NC_CMD_COLOR_FG_YELLOW       0x0E
#define NC_CMD_COLOR_FG_WHITE        0x0F
// // background // //
#define NC_CMD_COLOR_BG_BLACK        0x00
#define NC_CMD_COLOR_BG_DARK_BLUE    0x10
#define NC_CMD_COLOR_BG_DARK_GREEN   0x20
#define NC_CMD_COLOR_BG_DARK_CYAN    0x30
#define NC_CMD_COLOR_BG_DARK_RED     0x40
#define NC_CMD_COLOR_BG_DARK_MAGENTA 0x50
#define NC_CMD_COLOR_BG_DARK_YELLOW  0x60
#define NC_CMD_COLOR_BG_GREY         0x70
#define NC_CMD_COLOR_BG_DARK_GREY    0x80
#define NC_CMD_COLOR_BG_BLUE         0x90
#define NC_CMD_COLOR_BG_GREEN        0xA0
#define NC_CMD_COLOR_BG_CYAN         0xB0
#define NC_CMD_COLOR_BG_RED          0xC0
#define NC_CMD_COLOR_BG_MAGENTA      0xD0
#define NC_CMD_COLOR_BG_YELLOW       0xE0
#define NC_CMD_COLOR_BG_WHITE        0xF0
// // indexed // //
// // // foreground // // //
#define NC_CMD_COLOR_FG_01 0x00
#define NC_CMD_COLOR_FG_02 0x01
#define NC_CMD_COLOR_FG_03 0x02
#define NC_CMD_COLOR_FG_04 0x03
#define NC_CMD_COLOR_FG_05 0x04
#define NC_CMD_COLOR_FG_06 0x05
#define NC_CMD_COLOR_FG_07 0x06
#define NC_CMD_COLOR_FG_08 0x07
#define NC_CMD_COLOR_FG_09 0x08
#define NC_CMD_COLOR_FG_10 0x09
#define NC_CMD_COLOR_FG_11 0x0A
#define NC_CMD_COLOR_FG_12 0x0B
#define NC_CMD_COLOR_FG_13 0x0C
#define NC_CMD_COLOR_FG_14 0x0D
#define NC_CMD_COLOR_FG_15 0x0E
#define NC_CMD_COLOR_FG_16 0x0F
// // // background // // //
#define NC_CMD_COLOR_BG_1 = 0x00,	COLOR_BG_2 = 0x10,	COLOR_BG_3 = 0x20,	COLOR_BG_4 = 0x30,
#define NC_CMD_COLOR_BG_5 = 0x40,	COLOR_BG_6 = 0x50,	COLOR_BG_7 = 0x60,	COLOR_BG_8 = 0x70,
#define NC_CMD_COLOR_BG_9 = 0x80,	COLOR_BG_10 = 0x90,	COLOR_BG_11 = 0xA0,	COLOR_BG_12 = 0xB0,
#define NC_CMD_COLOR_BG_13 = 0xC0,	COLOR_BG_14 = 0xD0,	COLOR_BG_15 = 0xE0,	COLOR_BG_16 = 0xF0,
/* end_of_file */
#endif	// NC_CMD_CORE_H //