//
// Created by Tobias on 19.04.2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "RenderedObject.h"


class Player final : public RenderedObject {
public:
    Player(float x, float y, float width, float height, float movement_speed);

    void render() const override;
};


#endif //PLAYER_H
