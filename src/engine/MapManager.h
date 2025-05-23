#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "raylib.h"
#include "tinyxml2.h"
#include "../Constants.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

struct AnimationFrame {
    int tileid;
    int duration;
};

struct Animation {
    std::vector<AnimationFrame> frames;
    float current_time;
    int current_frame;
};

struct Tileset {
    int firstgid;
    std::string name;
    int tile_width;
    int tile_height;
    Texture2D texture;
    std::unordered_map<int, Animation> animations;
    std::unordered_map<int, Texture2D> individual_textures;
};


struct CollectibleInfo {
    std::string name;
    float value;
    Vector2 position;
};


struct Tile {
    Rectangle source_rect;
    Vector2 position;
    int gid;
    Animation *animation;
    int current_animation_tileid;
    bool collected;
};


struct Layer {
    std::string name;
    std::vector<std::unique_ptr<Tile> > tiles;
};

class MapManager {
public:
    MapManager();

    ~MapManager();

    bool load_map(const std::string &file_path);

    void draw_map() const;

    void update_animations();

    [[nodiscard]] Vector2 get_spawn_point(const std::string &name) const;

    [[nodiscard]] bool is_tile_collidable(int x, int y) const;

    [[nodiscard]] bool check_boundary_collision(Rectangle rect) const;

    void check_collectible_collision(Rectangle rect);

private:
    int map_width;
    int map_height;
    int tile_width;
    int tile_height;
    bool has_collectibles;

    std::vector<std::shared_ptr<Tileset> > tilesets;
    std::vector<std::unique_ptr<Layer> > layers;
    std::unordered_set<int> collidable_tiles;
    std::unordered_map<std::string, Vector2> spawn_points;
    std::unordered_map<int, std::string> collectible_names;
    std::unordered_map<int, float> collectible_values;


    void UnloadMap();

    bool parse_tilesets(tinyxml2::XMLElement *map_element);

    bool parse_tileset(tinyxml2::XMLElement *tileset_element, int firstgid);

    bool parse_layers(tinyxml2::XMLElement *map_element);

    bool parse_object_groups(tinyxml2::XMLElement *map_element);

    bool parse_layer_data(tinyxml2::XMLElement *layer_element, std::unique_ptr<Layer> &layer) const;

    bool load_animations(tinyxml2::XMLElement *tile_element, std::shared_ptr<Tileset> &tileset, int tile_id);

    [[nodiscard]] std::shared_ptr<Tileset> get_tileset_for_gid(int gid) const;

    bool load_tileset_texture(std::shared_ptr<Tileset> &tileset, const std::string &image_path);
};

#endif // MAP_MANAGER_H
