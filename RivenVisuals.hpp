// RivenVisuals.hpp

#pragma once

#include "../Plugin_sdk/plugin_sdk.hpp"

namespace riven_visuals
{
	void init();
	void destroy();

	// accessor methods for color settings
	TreeColorPicker* get_q_color();

	TreeColorPicker* get_w_color();

	TreeColorPicker* get_e_color();

	TreeColorPicker* get_r_color();

	TreeCheckbox* get_enable_gradient();

	TreeSlider* get_thickness();

	// Add more functions as needed for your visual effects

}