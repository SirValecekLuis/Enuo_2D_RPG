//
// Created by Tobias on 22.04.2025.
//

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

class Engine;

#include "MapManager.h"

struct TextToRender {
    std::string text;
    Vector2 pos;
    float size;
    float spacing;
    Color color;
    float time_to_live;
    float time_rendered = 0;
};

class TextManager {
public:
    TextManager();
    ~TextManager();

    void add_text(const TextToRender& text);
    void update();

private:
    std::vector<TextToRender> texts = {};
    Font font = {};

    void render_text(const TextToRender& text) const;
};



#endif //TEXT_MANAGER_H
