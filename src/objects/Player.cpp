//
// Created by Tobias on 19.04.2025.
//

#include "Player.h"

#include <raylib.h>

Player::Player(const int x, const int y, const int width, const int height): RenderedObject(x, y, width, height) {

}

void Player::render() const {
    DrawRectangle(x, y, width, height, BLUE);
}
