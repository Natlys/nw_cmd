#ifndef NWCMD_GUI_H
#define NWCMD_GUI_H

#include <nwcmd_core.hpp>

namespace NWCMD
{
	enum cmd_widget_states : v1u {
		WGS_DEFAULT = 0,
		WGS_DISABLE, WGS_ENABLE,
		WGS_ACT, WGS_DEACT,
		WGS_FOCUS, WGS_DEFOCUS,
		WGS_NEXT, WGS_BACK,
		WGS_MOVE_LT, WGS_MOVE_RT, WGS_MOVE_UP, WGS_MOVE_DN
	};
	/// cmd_widget class
	class NW_API cmd_widget
	{
	public:
		using action = std::function<v1u()>;
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
		cmd_widget(cmd_engine& rEngine);
		virtual ~cmd_widget();
		// --getters
		inline cstr get_name() const { return &m_name[0]; }
		inline cmd_widget_states get_state() const { return m_state; }
		// --setters
		void set_name(cstr name);
		void set_action(const action& act);
		// --core_methods
		virtual void on_draw() = 0;
		virtual void on_state(cmd_widget_states state) = 0;
	protected:
		cmd_engine* m_engine;
		dstr m_name;
		cmd_widget_states m_state;
		action m_action;
	};
}
namespace NWCMD
{
	/// cmd_menu_item class for nwc graphical user interface
	class NW_API cmd_menu_item : public cmd_widget
	{
	public:
		cmd_menu_item(cmd_engine& engine);
		virtual ~cmd_menu_item();
		// --core_methods
		virtual void on_draw() override;
		virtual void on_state(cmd_widget_states state) override;
	};
}

#endif	// NWCMD_GUI_H
