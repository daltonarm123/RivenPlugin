// RivenVisuals

#include "RivenVisuals.hpp"

namespace riven_visuals
{
	TreeTab* visualsMenu = nullptr;

	TreeColorPicker* q_color = nullptr;
	TreeColorPicker* w_color = nullptr;
	TreeColorPicker* e_color = nullptr;
	TreeColorPicker* r_color = nullptr;
	TreeColorPicker* enable_gradient = nullptr;
	TreeSlider* thickness = nullptr;

	void init()
	{
		visualsMenu = menu->create_tab("riven_visuals", "Riven Visuals")

		float default_q_color[] = { 0.98f, 1.f, 0.69f, 0.8f };
		float default_w_color[] = { 0.33f, 0.59f, 0.72f, 0.8f };
		float default_e_color[] = { 0.42f, 0.76f, 0.63f, 0.8f };
		float default_r_color[] = { 1.f, 0.67f, 0.67f, 0.8f }

		// add visual customizations
		thickness = visualsMenu->add_slider("thickness", "Line Thickness", 1, 1, 10);
		enable_gradient = visualsMenu->add_checkbox("enable_gradient", "Enable Gradient", false)

		q_color = visualsMenu->add_colorpick("q_color", "Q color", default_q_color);
		w_color = visualsMenu->add_colorpick("w_color", "W color", default_w_color);
		e_color = visualsMenu->add_colorpick("e_color", "E color", default_e_color);
		r_color = visualsMenu->add_colorpick("r_color", "R color", default_r_color);

		// Add additional customization options if needed
	}

	void destroy()
	{
		if (visualsMenu)
		{
			menu->delete_tab(visualsMenu);
			visualsMenu = nullptr;
		}
	}

	//Methods for external use
	TreeColorPicker* get_q_color() { return q_color; }
	TreeColorPicker* get_w_color() { return w_color; }
	TreeColorPicker* get_e_color() { return e_color; }
	TreeColorPicker* get_r_color() { return r_color; }
	TreeCheckbox* get_enable_gradient() { return enable_gradient; }
	TreeSlider* get_thickness() { return thickness; }
}