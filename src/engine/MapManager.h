#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "raylib.h"
#include "tinyxml2.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class MapManager {
public:
    MapManager();
    ~MapManager();

    bool load_map(const std::string& file_path, const std::string& tileset_path);
    void draw_map() const;

    Vector2 get_spawn_point(const std::string& name) const;

    bool is_tile_collidable(int x, int y) const;
    bool check_collision(Rectangle rect) const;

private:
    struct Tile {
        Rectangle source_rect;
        Vector2 position;
        int gid;
    };

    struct Layer {
        std::string name;
        std::vector<Tile> tiles;
    };

    struct SpawnPoint {
        std::string name;
        Vector2 position;
    };

    int map_width;
    int map_height;
    int tile_width;
    int tile_height;

    Texture2D tileset_texture;
    std::vector<Layer> layers;
    std::unordered_set<int> collidable_tiles;
    std::unordered_map<std::string, Vector2> spawn_points;

    void UnloadMap();
    bool parse_tileset(tinyxml2::XMLElement* tileset_element);
    bool parse_layers(tinyxml2::XMLElement* map_element);
    bool parse_object_groups(tinyxml2::XMLElement* map_element);
    bool parse_layer_data(tinyxml2::XMLElement* layer_element, Layer& layer) const;
};

#endif // MAP_MANAGER_H
