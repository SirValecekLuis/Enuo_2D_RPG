#include "MapManager.h"
#include "../objects/Player.h"
#include <fstream>
#include <iostream>

MapManager::MapManager(const int tile_size, const int map_width, const int map_height)
    : tile_size(tile_size), map_width(map_width), map_height(map_height),
      tiles(map_width * map_height, 0) // initialize all tiles as 0
{
}

bool MapManager::load_from_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) return false;

    file.read(reinterpret_cast<char*>(&map_width), sizeof(map_width));
    file.read(reinterpret_cast<char*>(&map_height), sizeof(map_height));

    tiles.resize(map_width * map_height);
    file.read(reinterpret_cast<char*>(tiles.data()), tiles.size() * sizeof(int));

    return true;
}

bool MapManager::save_to_file(const std::string& file_path) const {
    std::ofstream file(file_path, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(&map_width), sizeof(map_width));
    file.write(reinterpret_cast<const char*>(&map_height), sizeof(map_height));
    file.write(reinterpret_cast<const char*>(tiles.data()), tiles.size() * sizeof(int));

    return true;
}

void MapManager::render(const std::shared_ptr<Player>& player) const {
    const Camera2D& camera = player->get_camera();

    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    const float left = camera.target.x - screen_width / 2.0f;
    const float top = camera.target.y - screen_height / 2.0f;

    int first_tile_x = static_cast<int>(left) / tile_size;
    int first_tile_y = static_cast<int>(top) / tile_size;

    int last_tile_x = static_cast<int>(left + screen_width) / tile_size + 1;
    int last_tile_y = static_cast<int>(top + screen_height) / tile_size + 1;

    first_tile_x = std::max(0, first_tile_x);
    first_tile_y = std::max(0, first_tile_y);
    last_tile_x = std::min(map_width, last_tile_x);
    last_tile_y = std::min(map_height, last_tile_y);

    for (int y = first_tile_y; y < last_tile_y; ++y) {
        for (int x = first_tile_x; x < last_tile_x; ++x) {
            const int tile_value = get_tile(x, y);
            Color color;

            switch (tile_value) {
            case 0: color = GREEN;
                break; // grass
            case 1: color = GRAY;
                break; // stone
            case 2: color = BLUE;
                break; // water
            default: color = BLACK;
                break;
            }

            const Rectangle tile_rect = {
                static_cast<float>(x * tile_size),
                static_cast<float>(y * tile_size),
                static_cast<float>(tile_size),
                static_cast<float>(tile_size)
            };

            DrawRectangleRec(tile_rect, color);
        }
    }
}

int MapManager::get_tile(const int x, const int y) const {
    if (x < 0 || x >= map_width || y < 0 || y >= map_height) return -1;
    return tiles[get_index(x, y)];
}

void MapManager::set_tile(const int x, const int y, const int value) {
    if (x < 0 || x >= map_width || y < 0 || y >= map_height) return;
    tiles[get_index(x, y)] = value;
}

int MapManager::get_index(const int x, const int y) const {
    return y * map_width + x;
}

int MapManager::get_map_width() const { return map_width; }
int MapManager::get_map_height() const { return map_height; }
