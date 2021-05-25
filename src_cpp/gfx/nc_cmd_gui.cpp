#include "nc_cmd_pch.hpp"
#include "nc_cmd_gui.hxx"
#if (defined NC_WAPI)
#	include "core/nc_cmd_eng.hxx"

cmd_widget::cmd_widget() :
	nc_name_owner_t(),
	coord{ 0, 0 }, size{ 10, 5 }, padding{ 1, 1, 1, 1 },
	pxl_default{ PXT_SOLID, COLOR_BG_4 | COLOR_FG_8 }, pxl_disable{ PXT_SOLID, COLOR_BG_1 | COLOR_FG_4 },
	pxl_focus{ PXT_SOLID, COLOR_BG_8 | COLOR_FG_12 }, pxl_action{ PXT_SOLID, COLOR_BG_8 | COLOR_FG_12 },
	pxl_draw{ pxl_default },
	m_state(WGS_DEFAULT),
	m_action{ []()->v1u_t { return WGS_DEFAULT; } }
{
	m_name = m_name + "_" + std::to_string(get_rand(0u, 2u << 8u));
}
cmd_widget::~cmd_widget() { }
/* setters */
v1nil_t cmd_widget::set_action(const action& act) { m_action = act; }
cmd_menu_item::cmd_menu_item() :
	cmd_widget() { }
cmd_menu_item::~cmd_menu_item() { }
/* setters */
/* commands */
v1nil_t cmd_menu_item::on_draw()
{
	v4s_t draw_rect;
	draw_rect[0] = coord[0] + padding[0];
	draw_rect[1] = coord[1] + padding[1];
	draw_rect[2] = coord[0] - padding[2] + size[0];
	draw_rect[3] = coord[1] - padding[3] + size[1];
	cmd_engine::get().draw_rect(draw_rect[0], draw_rect[1],
		draw_rect[2], draw_rect[3], pxl_draw);
	cmd_engine::get().draw_bytes(
		draw_rect[0], draw_rect[1], draw_rect[2], draw_rect[3],
		static_cast<cmd_colors>(pxl_draw.Attributes & 0xf0), &m_name[0], strlen(&m_name[0]));
}
v1nil_t cmd_menu_item::on_state(cmd_widget_states state)
{
	if (m_state == WGS_DISABLE && state != WGS_ENABLE) { return; }
	switch (state) {
	case WGS_ENABLE:
		pxl_draw = pxl_default;
		break;
	case WGS_DISABLE:
		pxl_draw = pxl_disable;
		break;
	case WGS_ACT:
		pxl_draw = pxl_action;
		m_action();
		break;
	case WGS_DEACT:
		pxl_draw = pxl_default;
		break;
	case WGS_FOCUS:
		pxl_draw = pxl_focus;
		break;
	case WGS_DEFOCUS:
		pxl_draw = pxl_default;
		break;
	default: break;
	}
	m_state = state;
}

#endif	// NC_WAPI //
/* end_of_file */