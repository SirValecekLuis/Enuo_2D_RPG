//
// Created by Tobias on 19.04.2025.
//

#ifndef KEYEVENTMANAGER_H
#define KEYEVENTMANAGER_H

#include <memory>

#include "Engine.h"
#include "../objects/Player.h"


class KeyEventManager {
public:
    KeyEventManager() = default;

    static void check_all_inputs(const std::shared_ptr<Player>& player);

    static Engine& engine;
};


#endif //KEYEVENTMANAGER_H
