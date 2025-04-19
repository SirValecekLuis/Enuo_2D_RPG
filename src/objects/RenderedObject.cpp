//
// Created by Tobias on 19.04.2025.
//

#include <raylib.h>

#include "RenderedObject.h"

#include "../engine/IdManager.h"

RenderedObject::RenderedObject(const int x, const int y, const int width,
                               const int height) : x(x), y(y), width(width), height(height), id(IdManager::gen_id()) {
}

void RenderedObject::render() const {
    DrawRectangle(x, y, width, height, RED);
}

void RenderedObject::move(const int add_x, const int add_y) {
    x += add_x;
    y += add_y;
}

unsigned int RenderedObject::get_id() const {
    return id;
}
