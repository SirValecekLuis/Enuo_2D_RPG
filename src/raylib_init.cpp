//
// Created by Tobias on 19.04.2025.
//

#include "raylib.h"

void raylib_init() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Enuo 2D RPG");
}
