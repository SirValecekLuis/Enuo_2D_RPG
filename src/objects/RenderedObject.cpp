//
// Created by Tobias on 19.04.2025.
//

#include <iostream>
#include <raylib.h>

#include "RenderedObject.h"
#include "../engine/IdManager.h"


using std::cout, std::endl;

RenderedObject::RenderedObject(const int x, const int y, const int width,
                               const int height) : x(x), y(y), width(width), height(height), id(IdManager::gen_id()) {
}

void RenderedObject::render() const {
    DrawRectangle(x, y, width, height, RED);
}

void RenderedObject::move_x(const int add_x) {
    cout << x << " " << y << " " << add_x << endl;
    x += add_x;
    cout << x << " " << y << " " << add_x << endl;
    cout << "end";
}

void RenderedObject::move_y(const int add_y) {
    y += add_y;
}

unsigned int RenderedObject::get_id() const {
    return id;
}
