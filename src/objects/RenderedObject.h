//
// Created by Tobias on 19.04.2025.
//

#ifndef RENDEREDOBJECT_H
#define RENDEREDOBJECT_H

class RenderedObject {
public:
    RenderedObject(int x, int y, int width, int height);
    virtual ~RenderedObject() = default;

    [[nodiscard]] virtual unsigned int get_id() const;

    virtual void render() const;
    void move_x(int add_x);
    void move_y(int add_y);

protected:
    int x, y, width, height;
    unsigned int id;
};



#endif //RENDEREDOBJECT_H
