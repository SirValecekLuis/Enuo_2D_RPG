//
// Created by Tobias on 23.04.2025.
//

#include "SoundManager.h"

SoundManager::SoundManager() {
    InitAudioDevice();
    SetMasterVolume(0.3f);
}

SoundManager::~SoundManager() {
    for (const auto& sound : sounds) {
        UnloadSound(sound);
    }
    sounds.clear();


    CloseAudioDevice();
}

void SoundManager::play_sound(const std::string &name) {
    const std::string name_path = PATH_TO_ASSETS + "/sounds/" + name;
    const Sound sound = LoadSound(name_path.c_str());

    PlaySound(sound);
    sounds.emplace_back(sound);
}

void SoundManager::update() {
    for (auto it = sounds.begin(); it != sounds.end();) {
        if (!IsSoundPlaying(*it)) {
            UnloadSound(*it);
            it = sounds.erase(it);
        } else {
            ++it;
        }
    }
}
