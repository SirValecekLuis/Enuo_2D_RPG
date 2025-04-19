//
// Created by Tobias on 19.04.2025.
//

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <memory>
#include <unordered_map>

#include "../objects/RenderedObject.h"

class ObjectManager {
    public:
    ObjectManager() = default;

    void add_object(std::unique_ptr<RenderedObject> object);
    void remove_object(unsigned int id);

    void render_objects();

    private:
    std::unordered_map<unsigned int, std::unique_ptr<RenderedObject>> objects = std::unordered_map<unsigned int, std::unique_ptr<RenderedObject>>();
};



#endif //OBJECTMANAGER_H
