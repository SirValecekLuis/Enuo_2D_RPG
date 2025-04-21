//
// Created by Tobias on 19.04.2025.
//

#include "Player.h"

#include <raylib.h>

Player::Player(const float x, const float y, const float width, const float height,
               const float movement_speed): RenderedObject(x, y, width, height, movement_speed) {
    camera = {0};
    camera.target = {rectangle.x, rectangle.y};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 4.f;
}

void Player::render() const {
    DrawRectangleRec(rectangle, BLUE);
}

void Player::camera_refresh() {
    camera.target = {rectangle.x, rectangle.y};
}


Camera2D& Player::get_camera() {
    return camera;
}

Rectangle Player::get_rectangle() const {
    return rectangle;
}

void Player::set_rectangle(const Rectangle& rec) {
    rectangle = rec;
}