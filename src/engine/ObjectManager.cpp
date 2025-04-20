//
// Created by Tobias on 19.04.2025.
//

#include "ObjectManager.h"

#include <ranges>

void ObjectManager::add_object(const std::shared_ptr<RenderedObject>& object) {
    objects[object->get_id()] = object;
}

void ObjectManager::set_player(const std::shared_ptr<Player>& pl) {
    objects[pl->get_id()] = pl;
    player = pl;
}


void ObjectManager::remove_object(const unsigned int id) {
    objects.erase(id);
}

void ObjectManager::render_objects() {
    for (const auto& val : objects | std::views::values) {
        val->render();
    }
}

std::shared_ptr<Player> ObjectManager::get_player() {
    return player;
}