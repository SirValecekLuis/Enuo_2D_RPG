//
// Created by Tobias on 19.04.2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "IdManager.h"
#include "KeyEventManager.h"
#include "ObjectManager.h"


class Engine {
public:
    static Engine& get_instance();

    Engine(const Engine&) = delete;

    Engine(Engine&&) = delete;

    Engine& operator=(const Engine&) = delete;

    Engine& operator=(Engine&&) = delete;


    ObjectManager object_manager;

private:
    Engine() = default;
};


#endif //ENGINE_H
