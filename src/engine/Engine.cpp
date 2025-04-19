//
// Created by Tobias on 19.04.2025.
//

#include "Engine.h"

Engine &Engine::get_instance() {
    static Engine instance;
    return instance;
}

