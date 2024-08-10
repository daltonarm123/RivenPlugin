// RivenHelpers.hpp

#pragma once

#include "../plugin_sdk/plugin_sdk.hpp"

namespace riven_helpers
{
	//function to determine hitchance for abilities
	float get_hitchance(get_hitchance::type hitchance);

	// utility function to check if a minion is blaid for targeting
	bool is_valid_minon(game_object_script minion);

	//utility function to count the number of balid minions in certain range
	int count_minons_in_range(float range, vector from);

	//Add extra functions here for future updates

}
