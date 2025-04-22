//
// Created by Tobias on 22.04.2025.
//

#include "TextManager.h"

TextManager::TextManager() {
    font = LoadFont("../../../assets/fonts/PixelifySans-Bold.ttf");
}

TextManager::~TextManager() {
    UnloadFont(font);
}


void TextManager::render_text(const TextToRender &text) const {
    DrawTextEx(font, text.text.c_str(), text.pos, text.size, text.spacing, text.color);
}

void TextManager::add_text(const TextToRender &text) {
    texts.emplace_back(text);
}

void TextManager::update() {
    for (auto it = texts.begin(); it != texts.end();) {
        if (it->time_rendered < it->time_to_live) {
            render_text(*it);
            it->time_rendered += GetFrameTime();
            it->pos.y += -4 * GetFrameTime();
            ++it;
        } else {
            it = texts.erase(it);
        }
    }
}
