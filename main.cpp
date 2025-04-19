#include <iostream>

#include "raylib.h"

using std::cout, std::endl;

int main() {
    InitWindow(800, 600, "Raylib Test");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from Raylib!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}