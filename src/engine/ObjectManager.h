//
// Created by Tobias on 19.04.2025.
//

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <memory>
#include <unordered_map>

#include "../objects/RenderedObject.h"
#include "../objects/Player.h"


class ObjectManager {
public:
    ObjectManager() = default;

    void add_object(const std::shared_ptr<RenderedObject>& object);
    void set_player(const std::shared_ptr<Player>& pl);

    void remove_object(unsigned int id);

    void render_objects();

    std::shared_ptr<Player> get_player();

private:
    std::unordered_map<unsigned int, std::shared_ptr<RenderedObject>> objects = std::unordered_map<
        unsigned int, std::shared_ptr<RenderedObject>>();

    std::shared_ptr<Player> player;
};


#endif //OBJECTMANAGER_H
