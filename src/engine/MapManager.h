#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "raylib.h"
#include <vector>
#include <memory>
#include <string>

class Player;

class MapManager {
public:
    MapManager(int tile_size, int map_width, int map_height);

    bool load_from_file(const std::string& file_path);

    bool save_to_file(const std::string& file_path) const;

    void render(const std::shared_ptr<Player>& player) const;

    int get_map_width() const;
    int get_map_height() const;
    int get_tile(int x, int y) const;
    void set_tile(int x, int y, int value);

private:
    int tile_size;
    int map_width;
    int map_height;
    std::vector<int> tiles;

    int get_index(int x, int y) const;
};

#endif // MAP_MANAGER_H
