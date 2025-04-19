#include <iostream>

#include "raylib.h"
#include "engine/Engine.h"
#include "objects/RenderedObject.h"

using std::cout, std::endl;

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Enuo 2D RPG");

    Engine& engine = Engine::get_instance();
    auto player = std::make_unique<RenderedObject>(GetMonitorWidth(0) / 2, GetMonitorHeight(0) / 2, 32, 32);

    engine.object_manager.add_object(std::move(player));
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        engine.object_manager.render_objects();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
