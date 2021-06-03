#ifndef NC_CMD_GUI_H
#	define NC_CMD_GUI_H
#	include "nc_cmd_core.hpp"
#	if (defined NC_WAPI)
enum cmd_widget_states : v1u_t {
	WGS_DEFAULT = 0,
	WGS_DISABLE, WGS_ENABLE,
	WGS_ACT, WGS_DEACT,
	WGS_FOCUS, WGS_DEFOCUS,
	WGS_NEXT, WGS_BACK,
	WGS_MOVE_LT, WGS_MOVE_RT, WGS_MOVE_UP, WGS_MOVE_DN
};
/// cmd_widget class
class cmd_widget : public nc_name_owner_t
{
public:
	using action = std::function<v1u_t()>;
public:
	cmd_pixel pxl_default;
	cmd_pixel pxl_disable;
	cmd_pixel pxl_focus;
	cmd_pixel pxl_action;
	cmd_pixel pxl_draw;
	v2s coord;
	v2s size;
	v4s padding;
public:
	cmd_widget();
	virtual ~cmd_widget();
	/* getters */
	inline cmd_widget_states get_state() const { return m_state; }
	/* setters */
	v1nil_t set_action(const action& act);
	/* commands */
	virtual v1nil_t on_draw() = 0;
	virtual v1nil_t on_state(cmd_widget_states state) = 0;
protected:
	cmd_widget_states m_state;
	action m_action;
};
/// cmd_menu_item class for nwc graphical user interface
class cmd_menu_item : public cmd_widget
{
public:
	cmd_menu_item();
	virtual ~cmd_menu_item();
	/* commands */
	virtual v1nil_t on_draw() override;
	virtual v1nil_t on_state(cmd_widget_states state) override;
};
#	endif	/* NC_WAPI */
/* end_of_file */
#endif	/* NC_CMD_GUI_H */