//
// Created by Tobias on 19.04.2025.
//

#include "raylib.h"

void raylib_init() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Enuo 2D RPG");
    SetWindowPosition(0, 0);
}
