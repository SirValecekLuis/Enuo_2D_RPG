//
// Created by Tobias on 19.04.2025.
//

#include "ObjectManager.h"

#include <ranges>

void ObjectManager::add_object(std::unique_ptr<RenderedObject> object) {
    objects[object->get_id()] = std::move(object);
}

void ObjectManager::remove_object(const unsigned int id) {
    objects.erase(id);
}

void ObjectManager::render_objects() {
    for (const auto &val: objects | std::views::values) {
        val->render();
    }
}
