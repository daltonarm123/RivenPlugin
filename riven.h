#pragma once

#include "../plugin_sdk/plugin_sdk.hpp"
#include "RivenDraw.hpp" // drawing functions
#include "RivenHelpers.hpp" // helper functions
#include "RivenVisuals.hpp"
namespace riven
{
    TreeTab* main_tab = nullptr;

    // Declare spell variables
    script_spell* q = nullptr;
    script_spell* w = nullptr;
    script_spell* e = nullptr;
    script_spell* r = nullptr;

    // Mode Hotkeys (with user-defined keybindings)
    TreeHotkey* combo_hotkey = nullptr;
    TreeHotkey* flee_hotkey = nullptr;
    TreeHotkey* farm_hotkey = nullptr;
    TreeHotkey* burst_combo_hotkey = nullptr;

    // Farm Mode Toggles
    TreeCheckbox* farm_use_abilities = nullptr;

    // Event callback function declarations
    void on_update();
    void on_draw();
    void on_process_spell_cast(game_object_script sender, spell_instance_script spell);

    // Helper function to calculate Riven's current damage 
    float calculate_current_damage(game_object_script target)
    {
        float base_damage = myhero->get_total_attack_damage(); // Get Riven's base damage

        // Calculate additional damage based on abilities and items
        float damage = base_damage;

        // Add Q, W, E, R specific damage calculation
        if (q->is_ready())
        {
            damage += 10 + (35 * q->level()) + 0.65f * myhero->get_total_attack_damage(); // Example Q damage calculation
        }

        if (w->is_ready())
        {
            damage += 40 + (30 * w->level()) + 1.0f * myhero->get_total_attack_damage(); // Example W damage calculation
        }

        if (e->is_ready())
        {
            damage += 20 + (30 * e->level()) + 0.4f * myhero->get_total_attack_damage(); // Example E damage calculation
        }

        if (r->is_ready())
        {
            damage += 80 + (40 * r->level()) + 1.5f * myhero->get_total_attack_damage(); // Example R damage calculation
        }

        return damage - (target->get_armor() / (100 + myhero->get_flat_armor_penetration()));
    }

    inline void load()
    {
        console->print("Riven script loaded.");

        // Initialize the main menu tab 
        main_tab = menu->create_tab("riven_plugin", "Riven Plugin");

        // Mode Hotkeys (with user-defined keybindings)
        combo_hotkey = main_tab->add_hotkey("combo_hotkey", "Combo Mode Hotkey", TreeHotkeyMode::Hold, 0x20 /*Default: Space key*/, false);
        flee_hotkey = main_tab->add_hotkey("flee_hotkey", "Flee Mode Hotkey", TreeHotkeyMode::Hold, 0x5A /*Default: Z key*/, false);
        farm_hotkey = main_tab->add_hotkey("farm_hotkey", "Farm Mode Hotkey", TreeHotkeyMode::Hold, 0x58 /*Default: X key*/, false);
        burst_combo_hotkey = main_tab->add_hotkey("burst_combo_hotkey", "Burst Combo Mode Hotkey", TreeHotkeyMode::Hold, 0x43 /*Default: C key*/, false);

        // Ability usage checkboxes and sliders for Combo Mode
        main_tab->add_checkbox("combo_use_q", "Use Q in Combo", true);
        main_tab->add_checkbox("combo_use_w", "Use W in Combo", true);
        main_tab->add_checkbox("combo_use_e", "Use E in Combo", true);
        main_tab->add_checkbox("combo_use_r", "Use R in Combo", true);

        // Farm Mode Toggles
        farm_use_abilities = main_tab->add_checkbox("farm_use_abilities", "Use Abilities to Farm", true);

        // Ability usage checkboxes and sliders for mana management
        main_tab->add_slider("min_mana_q", "Min Mana to Use Q", 40, 0, 100);
        main_tab->add_slider("min_mana_w", "Min Mana to Use W", 30, 0, 100);
        main_tab->add_slider("min_mana_e", "Min Mana to Use E", 20, 0, 100);
        main_tab->add_slider("min_mana_r", "Min Mana to Use R", 50, 0, 100);

        // Register spells
        q = plugin_sdk->register_spell(spellslot::q, 260.f);
        w = plugin_sdk->register_spell(spellslot::w, 125.f);
        e = plugin_sdk->register_spell(spellslot::e, 300.f);
        r = plugin_sdk->register_spell(spellslot::r, 900.f);

        // Setting Q as a skillshot
        q->set_skillshot(0.25f, 60.0f, FLT_MAX, { }, skillshot_type::skillshot_line);

        // Setting W as a point-blank AOE
        w->set_skillshot(0.25f, w->range(), FLT_MAX, {}, skillshot_type::skillshot_circle);

        // Setting R as a skillshot
        r->set_skillshot(0.25f, 200.0f, 1200.0f, { collisionable_objects::heroes, collisionable_objects::minions }, skillshot_type::skillshot_cone);

        // Register events 
        event_handler<events::on_update>::add_callback(on_update);
        event_handler<events::on_draw>::add_callback(on_draw); // Drawing logic from Permashow.cpp
        event_handler<events::on_process_spell_cast>::add_callback(on_process_spell_cast);
    }

    inline void unload()
    {
        console->print("Riven script unloaded.");

        // Remove spells
        if (q) plugin_sdk->remove_spell(q);
        if (w) plugin_sdk->remove_spell(w);
        if (e) plugin_sdk->remove_spell(e);
        if (r) plugin_sdk->remove_spell(r);

        // Remove events
        event_handler<events::on_update>::remove_handler(on_update);
        event_handler<events::on_draw>::remove_handler(on_draw);
        event_handler<events::on_process_spell_cast>::remove_handler(on_process_spell_cast);

        // Delete the main menu tab on unload
        if (main_tab)
        {
            menu->delete_tab(main_tab);
            main_tab = nullptr;
        }
    }

    void on_update()
    {
        // Your update logic here, integrating prediction
        if (combo_hotkey->get_bool())
        {
            for (auto& enemy : entitylist->get_enemy_heroes())
            {
                if (!enemy->is_valid() || enemy->is_dead() || !enemy->is_visible())
                    continue;

                // Prediction for Q
                auto q_pred = q->get_prediction(enemy, true);
                if (q_pred.hitchance >= get_hitchance(hit_chance::high) && q->is_ready() && q->is_in_range(q_pred.position) && main_tab->get_checkbox("combo_use_q")->get_bool())
                {
                    q->cast(q_pred.position);
                }

                // Prediction for W
                if (w->is_ready() && main_tab->get_checkbox("combo_use_w")->get_bool())
                {
                    auto w_pred = w->get_prediction(enemy, true);
                    if (w_pred.hitchance >= get_hitchance(hit_chance::high) && w->is_in_range(w_pred.position))
                    {
                        w->cast();
                    }
                }

                // Prediction for E (used to shield at the right time)
                if (e->is_ready() && main_tab->get_checkbox("combo_use_e")->get_bool())
                {
                    auto e_pred = e->get_prediction(enemy, true);
                    if (e_pred.hitchance >= get_hitchance(hit_chance::high))
                    {
                        e->cast(e_pred.position);
                    }
                }

                // Prediction for R
                auto r_pred = r->get_prediction(enemy, true);
                if (r_pred.hitchance >= get_hitchance(hit_chance::high) && r->is_ready() && r->is_in_range(r_pred.position) && main_tab->get_checkbox("combo_use_r")->get_bool())
                {
                    r->cast(r_pred.position);
                }
            }
        }

        // Implement similar logic for Burst Combo, Flee, and Farm modes
    }

    void on_draw()
    {
        if (myhero->is_dead())
        {
            return;
        }

        // Draw Riven's ability ranges
        if (q->is_ready())
            draw_manager->add_circle(myhero->get_position(), q->range(), MAKE_COLOR(255, 0, 0, 255)); // Q Range

        if (w->is_ready())
            draw_manager->add_circle(myhero->get_position(), w->range(), MAKE_COLOR(0, 255, 0, 255)); // W Range

        if (e->is_ready())
            draw_manager->add_circle(myhero->get_position(), e->range(), MAKE_COLOR(0, 0, 255, 255)); // E Range

        if (r->is_ready())
            draw_manager->add_circle(myhero->get_position(), r->range(), MAKE_COLOR(255, 255, 0, 255)); // R Range
    }

    void on_process_spell_cast(game_object_script sender, spell_instance_script spell)
    {
        // Implement any specific logic when a spell is cast
    }
}


