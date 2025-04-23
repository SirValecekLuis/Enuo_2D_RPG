//
// Created by Tobias on 19.04.2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "MapManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "TextManager.h"


class Engine {
public:
    static Engine &get_instance();

    Engine(const Engine &) = delete;

    Engine(Engine &&) = delete;

    Engine &operator=(const Engine &) = delete;

    Engine &operator=(Engine &&) = delete;

    SoundManager &get_sound_manager() { return sound_manager; }
    TextManager &get_text_manager() { return text_manager; }
    MapManager &get_map_manager() { return map_manager; }
    ObjectManager &get_object_manager() { return object_manager; }

    ObjectManager object_manager;
    MapManager map_manager;
    TextManager text_manager;
    SoundManager sound_manager;

private:
    Engine() = default;
};


#endif //ENGINE_H
