//
// Created by Tobias on 19.04.2025.
//

#include "Player.h"

#include <algorithm>
#include <raylib.h>

Player::Player(const float x, const float y, const float width, const float height,
               const float movement_speed): RenderedObject(x, y, width, height, movement_speed) {

}

void Player::render() const {
    DrawRectangleV(position, size, BLUE);
}
