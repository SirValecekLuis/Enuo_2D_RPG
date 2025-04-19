//
// Created by Tobias on 19.04.2025.
//

#ifndef RENDEREDOBJECT_H
#define RENDEREDOBJECT_H

class RenderedObject {
public:
    RenderedObject(int x, int y, int width, int height);

    [[nodiscard]] unsigned int get_id() const;

    void render() const;
    void move(int add_x, int add_y);

private:
    int x, y, width, height;
    unsigned int id;
};



#endif //RENDEREDOBJECT_H
