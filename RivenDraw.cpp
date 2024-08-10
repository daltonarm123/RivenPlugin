// RivenDraw.cpp

#include "RivenDraw.hpp"

namespace riven_draw
{
    // Function to draw Riven's ability ranges
    void draw_ability_ranges()
    {
        if (myhero->is_dead())
        {
            return;
        }

        // Draw Q range
        if (riven::q->is_ready())
        {
            draw_manager->add_circle(myhero->get_position(), riven::q->range(), MAKE_COLOR(62, 129, 237, 255)); // Blue for Q
        }

        // Draw W range
        if (riven::w->is_ready())
        {
            draw_manager->add_circle(myhero->get_position(), riven::w->range(), MAKE_COLOR(227, 203, 20, 255)); // Yellow for W
        }

        // Draw E range
        if (riven::e->is_ready())
        {
            draw_manager->add_circle(myhero->get_position(), riven::e->range(), MAKE_COLOR(235, 12, 223, 255)); // Purple for E
        }

        // Draw R range
        if (riven::r->is_ready())
        {
            draw_manager->add_circle(myhero->get_position(), riven::r->range(), MAKE_COLOR(224, 77, 13, 255)); // Red for R
        }
    }

    // Optional: Function to draw a damage indicator on enemies
    void draw_damage_indicator()
    {
        for (auto& enemy : entitylist->get_enemy_heroes())
        {
            if (!enemy->is_valid() || enemy->is_dead() || !enemy->is_visible())
                continue;

            float damage = riven::calculate_current_damage(enemy);

            vector hpbar_pos = enemy->get_hpbar_pos();
            float health = enemy->get_health();

            float start = hpbar_pos.x + (damage / health) * 100; // Scale appropriately
            float end = hpbar_pos.x + 100;

            draw_manager->add_filled_rect(vector(start, hpbar_pos.y), vector(end, hpbar_pos.y + 10), MAKE_COLOR(255, 0, 0, 180)); // Red bar for damage indicator
        }
    }
}
