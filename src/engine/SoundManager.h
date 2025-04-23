//
// Created by Tobias on 23.04.2025.
//

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <raylib.h>
#include <vector>

#include "../Constants.h"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void play_sound(const std::string &name);
    void update();

private:
    std::vector<Sound> sounds = {};
};


#endif //SOUND_MANAGER_H
