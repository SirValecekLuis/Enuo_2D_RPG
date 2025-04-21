//
// Created by Tobias on 19.04.2025.
//

#ifndef RENDEREDOBJECT_H
#define RENDEREDOBJECT_H

#include "raylib.h"

class RenderedObject {
public:
    RenderedObject(float x, float y, float width, float height, float movement_speed);
    virtual ~RenderedObject() = default;

    [[nodiscard]] virtual unsigned int get_id() const;

    virtual void render() const;
    void move_x(float add_x);
    void move_y(float add_y);

    float get_movement_speed() const;

protected:
    Rectangle rectangle;
    float movement_speed;
    unsigned int id;
};



#endif //RENDEREDOBJECT_H
