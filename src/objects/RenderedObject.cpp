//
// Created by Tobias on 19.04.2025.
//

#include <iostream>
#include <raylib.h>

#include "RenderedObject.h"
#include "../engine/IdManager.h"


using std::cout, std::endl;

RenderedObject::RenderedObject(const float x, const float y, const float width, const float height,
                               const float movement_speed): movement_speed(movement_speed), id(IdManager::gen_id()) {
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = width;
    rectangle.height = height;
}

void RenderedObject::render() const {
    DrawRectangleRec(rectangle, RED);
}

void RenderedObject::move_x(const float add_x) {
    rectangle.x += add_x;
}

void RenderedObject::move_y(const float add_y) {
    rectangle.y += add_y;
}

unsigned int RenderedObject::get_id() const {
    return id;
}

float RenderedObject::get_movement_speed() const {
    return movement_speed;
}
