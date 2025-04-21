//
// Created by Tobias on 19.04.2025.
//

#include "KeyEventManager.h"

#include <iostream>
#include <raylib.h>

using std::cout, std::endl;

Engine& KeyEventManager::engine = Engine::get_instance();

void KeyEventManager::check_all_inputs(const std::shared_ptr<Player>& player) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        cout << "Menu Opened" << endl;
    }

    const float move_sp = player->get_movement_speed();
    const float dt = GetFrameTime();
    Rectangle player_rect = player->get_rectangle();

    Rectangle test_rect = player_rect;

    bool moved = false;

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        test_rect.y = player_rect.y - move_sp * dt;
        if (!engine.map_manager.check_collision(test_rect)) {
            player_rect.y = test_rect.y;
            moved = true;
        }
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        test_rect.y = player_rect.y + move_sp * dt;
        if (!engine.map_manager.check_collision(test_rect)) {
            player_rect.y = test_rect.y;
            moved = true;
        }
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        test_rect.x = player_rect.x - move_sp * dt;
        if (!engine.map_manager.check_collision(test_rect)) {
            player_rect.x = test_rect.x;
            moved = true;
        }
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        test_rect.x = player_rect.x + move_sp * dt;
        if (!engine.map_manager.check_collision(test_rect)) {
            player_rect.x = test_rect.x;
            moved = true;
        }
    }

    if (moved) {
        player->set_rectangle(player_rect);
        player->camera_refresh();
    }
}

