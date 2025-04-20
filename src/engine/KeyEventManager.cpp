//
// Created by Tobias on 19.04.2025.
//

#include "KeyEventManager.h"

#include <iostream>
#include <raylib.h>

using std::cout, std::endl;

void KeyEventManager::check_all_inputs(const std::shared_ptr<Player>& player) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        cout << "Menu Opened" << endl;
    }

    const float move_sp = player->get_movement_speed();
    const float dt = GetFrameTime();
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player->move_y(-move_sp * dt);
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player->move_y(move_sp * dt);
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player->move_x(-move_sp * dt);
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player->move_x(move_sp * dt);
    }
}
