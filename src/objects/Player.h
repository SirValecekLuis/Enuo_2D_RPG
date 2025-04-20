//
// Created by Tobias on 19.04.2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "RenderedObject.h"


class Player final : public RenderedObject {
public:
    Player(int x, int y, int width, int height);

    void render() const override;
};


#endif //PLAYER_H
