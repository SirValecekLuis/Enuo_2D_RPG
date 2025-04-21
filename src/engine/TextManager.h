//
// Created by Tobias on 22.04.2025.
//

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include "MapManager.h"

class TextManager {
public:
    TextManager();

    void render_collectible_info(CollectibleInfo& collectible);
private:
    Font font;
};



#endif //TEXT_MANAGER_H
