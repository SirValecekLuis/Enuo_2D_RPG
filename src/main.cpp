#include <iostream>

#include "raylib.h"
#include "raylib_init.h"
#include "engine/Engine.h"
#include "engine/KeyEventManager.h"
#include "objects/Player.h"

using std::cout, std::endl;

bool should_close() {
    if (WindowShouldClose() && IsKeyDown(KEY_ESCAPE)) {
        return false;
    }
    else {
        return WindowShouldClose();
    }
}

void game_loop() {
    Engine& engine = Engine::get_instance();
    auto player = std::make_shared<Player>(GetMonitorWidth(0) / 2.f, GetMonitorHeight(0) / 2.f, 32.f, 32.f, 200.f);
    auto enemy = std::make_shared<RenderedObject>(GetMonitorWidth(0) / 2.f + 32, GetMonitorHeight(0) / 2.f + 32, 32.f,
                                                  32.f, 200.f);

    engine.object_manager.set_player(player);
    engine.object_manager.add_object(enemy);

    while (!should_close()) {
        BeginDrawing();
        ClearBackground(BLACK);

        KeyEventManager::check_all_inputs(player);

        BeginMode2D(player->get_camera());
        engine.map_manager.render(player);
        engine.object_manager.render_objects();
        EndMode2D();

        EndDrawing();
    }
}

int main() {
    raylib_init();
    game_loop();
    CloseWindow();

    return 0;
}
