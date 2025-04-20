//
// Created by Tobias on 19.04.2025.
//

#include "Player.h"

#include <raylib.h>

Player::Player(const float x, const float y, const float width, const float height,
               const float movement_speed): RenderedObject(x, y, width, height, movement_speed) {
    camera = {0};
    camera.target = position;
    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Player::render() const {
    DrawRectangleV(position, size, BLUE);
}

void Player::camera_refresh() {
    camera.target = position;
}


Camera2D& Player::get_camera() {
    return camera;
}
