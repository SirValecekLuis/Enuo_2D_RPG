#include <filesystem>
#include <iostream>

#include "raylib.h"
#include "raylib_init.h"
#include "engine/Engine.h"
#include "engine/KeyEventManager.h"
#include "engine/TextManager.h"
#include "objects/Player.h"

using std::cout, std::endl;

bool should_close() {
    if (WindowShouldClose() && IsKeyDown(KEY_ESCAPE)) {
        return false;
    } else {
        return WindowShouldClose();
    }
}

void game_loop() {
    Engine &engine = Engine::get_instance();
    cout << "Aktuální pracovní adresář: " << std::filesystem::current_path() << std::endl;

    if (!engine.map_manager.load_map("../../../assets/maps/map.tmx")) {
        cout << "Failed to load map or tileset." << endl;
        exit(-1);
    }

    Vector2 player_pos = engine.map_manager.get_spawn_point("player_spawn");
    auto player = std::make_shared<Player>(player_pos.x, player_pos.y, 8.f, 8.f, 200.f);
    auto enemy = std::make_shared<RenderedObject>(0.f + 32, 0.f + 32, 32.f,
                                                  32.f, 200.f);

    engine.object_manager.set_player(player);
    engine.object_manager.add_object(enemy);

    while (!should_close()) {
        BeginDrawing();
        ClearBackground(BLACK);

        KeyEventManager::check_all_inputs(player);
        engine.map_manager.check_collectible_collision(player->get_rectangle());

        BeginMode2D(player->get_camera());
        engine.map_manager.draw_map();
        engine.map_manager.update_animations();
        engine.object_manager.render_objects();
        engine.text_manager.update();
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
