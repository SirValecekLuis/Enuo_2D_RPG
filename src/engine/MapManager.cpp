#include "MapManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

using namespace tinyxml2;
using std::cout, std::endl;
namespace fs = std::filesystem;

MapManager::MapManager()
    : map_width(0), map_height(0), tile_width(0), tile_height(0), has_collectibles(false) {
}

MapManager::~MapManager() {
    UnloadMap();
}

bool MapManager::load_map(const std::string &file_path) {
    UnloadMap();

    XMLDocument doc;
    if (const XMLError result = doc.LoadFile(file_path.c_str()); result != XML_SUCCESS) {
        std::cerr << "Map was not loaded successfully. " << doc.ErrorStr() << std::endl;
        return false;
    }

    XMLElement *map_element = doc.FirstChildElement("map");
    if (!map_element) {
        std::cerr << "Map element not found" << std::endl;
        return false;
    }

    map_element->QueryIntAttribute("width", &map_width);
    map_element->QueryIntAttribute("height", &map_height);
    map_element->QueryIntAttribute("tilewidth", &tile_width);
    map_element->QueryIntAttribute("tileheight", &tile_height);

    if (!parse_tilesets(map_element)) {
        std::cerr << "Failed to parse tilesets" << std::endl;
        UnloadMap();
        return false;
    }
    if (!parse_layers(map_element)) {
        std::cerr << "Failed to parse map layers" << std::endl;
        UnloadMap();
        return false;
    }

    if (!parse_object_groups(map_element)) {
        std::cerr << "Failed to parse object groups" << std::endl;
        UnloadMap();
        return false;
    }

    return true;
}

bool MapManager::parse_tilesets(XMLElement *map_element) {
    if (!map_element) return false;

    for (XMLElement *tileset_element = map_element->FirstChildElement("tileset");
         tileset_element;
         tileset_element = tileset_element->NextSiblingElement("tileset")) {
        int firstgid = 0;
        tileset_element->QueryIntAttribute("firstgid", &firstgid);

        if (!parse_tileset(tileset_element, firstgid)) {
            std::cerr << "Failed to parse tileset with firstgid " << firstgid << std::endl;
            return false;
        }
    }

    return true;
}

bool MapManager::parse_tileset(XMLElement *tileset_element, int firstgid) {
    if (!tileset_element) return false;

    auto tileset = std::make_shared<Tileset>();
    tileset->firstgid = firstgid;

    const char *name = tileset_element->Attribute("name");
    tileset->name = name ? name : "unnamed";

    tileset_element->QueryIntAttribute("tilewidth", &tileset->tile_width);
    tileset_element->QueryIntAttribute("tileheight", &tileset->tile_height);

    bool is_collection = false;
    const char *element_orientation = tileset_element->Attribute("orientation");
    const char *element_columns = tileset_element->Attribute("columns");

    if (element_columns && std::string(element_columns) == "0") {
        is_collection = true;
    }

    if (element_orientation && std::string(element_orientation) == "orthogonal") {
        XMLElement *grid = tileset_element->FirstChildElement("grid");
        if (grid) {
            const char *orientation = grid->Attribute("orientation");
            if (orientation && std::string(orientation) == "orthogonal") {
                is_collection = true;
            }
        }
    }

    XMLElement *image_element = tileset_element->FirstChildElement("image");
    if (image_element && !is_collection) {
        const char *source = image_element->Attribute("source");
        if (source) {
            const std::string image_path = "../../assets/maps" + std::string(source);

            if (!load_tileset_texture(tileset, image_path)) {
                return false;
            }
        }
    }

    for (XMLElement *tile_element = tileset_element->FirstChildElement("tile");
         tile_element;
         tile_element = tile_element->NextSiblingElement("tile")) {
        int tile_id = 0;
        tile_element->QueryIntAttribute("id", &tile_id);

        if (XMLElement *objectgroup = tile_element->FirstChildElement("objectgroup")) {
            collidable_tiles.insert(tile_id + firstgid);
        }

        if (XMLElement *animation_element = tile_element->FirstChildElement("animation")) {
            load_animations(tile_element, tileset, tile_id);
        }

        if (is_collection) {
            XMLElement *img_element = tile_element->FirstChildElement("image");
            if (img_element) {
                const char *source = img_element->Attribute("source");
                if (source) {
                    std::string image_path = "../../assets/maps" + std::string(source);
                    Texture2D tile_texture = LoadTexture(image_path.c_str());
                    if (tile_texture.id != 0) {
                        tileset->individual_textures[tile_id] = tile_texture;
                    }
                }
            }
        }

        XMLElement *properties_element = tile_element->FirstChildElement("properties");
        if (properties_element) {
            int global_id = tile_id + firstgid;

            for (XMLElement *property = properties_element->FirstChildElement("property");
                 property;
                 property = property->NextSiblingElement("property")) {
                const char *prop_name = property->Attribute("name");
                const char *prop_value = property->Attribute("value");

                if (prop_name && prop_value) {
                    std::string name_str = prop_name;

                    if (name_str == "coin") {
                        collectible_names[global_id] = "coin " + std::string(prop_value);
                        collectible_values[global_id] = 10;
                    } else if (name_str == "name") {
                        collectible_names[global_id] = prop_value;
                    } else if (name_str == "value") {
                        try {
                            collectible_values[global_id] = std::stof(prop_value);
                        } catch ([[maybe_unused]] const std::exception &e) {
                            collectible_values[global_id] = 1;
                        }
                    }
                }
            }
        }
    }

    tilesets.push_back(tileset);
    return true;
}

bool MapManager::load_animations(XMLElement *tile_element, std::shared_ptr<Tileset> &tileset, const int tile_id) {
    XMLElement *animation_element = tile_element->FirstChildElement("animation");
    if (!animation_element) return false;

    Animation animation;
    animation.current_time = 0.0f;
    animation.current_frame = 0;

    for (XMLElement *frame_element = animation_element->FirstChildElement("frame");
         frame_element;
         frame_element = frame_element->NextSiblingElement("frame")) {
        AnimationFrame frame;
        frame_element->QueryIntAttribute("tileid", &frame.tileid);
        frame_element->QueryIntAttribute("duration", &frame.duration);

        animation.frames.push_back(frame);
    }

    if (!animation.frames.empty()) {
        tileset->animations[tile_id] = animation;
        return true;
    }

    return false;
}

bool MapManager::load_tileset_texture(std::shared_ptr<Tileset> &tileset, const std::string &image_path) {
    tileset->texture = LoadTexture(image_path.c_str());
    return tileset->texture.id != 0;
}

std::shared_ptr<Tileset> MapManager::get_tileset_for_gid(const int gid) const {
    for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; i--) {
        if (gid >= tilesets[i]->firstgid) {
            return tilesets[i];
        }
    }
    return nullptr;
}

bool MapManager::parse_layers(XMLElement *map_element) {
    if (!map_element) return false;

    XMLElement *map_parts = nullptr;
    for (XMLElement *group = map_element->FirstChildElement("group");
         group;
         group = group->NextSiblingElement("group")) {
        const char *name = group->Attribute("name");
        if (name && std::string(name) == "map_parts") {
            map_parts = group;
            break;
        }
    }

    if (!map_parts) {
        map_parts = map_element;
    }

    for (XMLElement *layer_element = map_parts->FirstChildElement("layer");
         layer_element;
         layer_element = layer_element->NextSiblingElement("layer")) {
        auto layer = std::make_unique<Layer>();
        const char *name = layer_element->Attribute("name");
        layer->name = name ? name : "unnamed";

        if (!parse_layer_data(layer_element, layer)) {
            return false;
        }

        if (layer->name == "collectibles") {
            has_collectibles = true;
        }

        layers.push_back(std::move(layer));
    }

    return true;
}

bool MapManager::parse_layer_data(XMLElement *layer_element, std::unique_ptr<Layer> &layer) const {
    const XMLElement *data = layer_element->FirstChildElement("data");
    if (!data) return false;

    const char *csv_data = data->GetText();
    if (!csv_data) return false;

    const std::string csv_string(csv_data);
    std::stringstream ss(csv_string);
    std::string item;
    int x = 0;
    int y = 0;

    while (std::getline(ss, item, ',')) {
        if (item.find_first_not_of("0123456789") != std::string::npos) {
            std::erase_if(item, [](const char c) {
                return !std::isdigit(c);
            });
        }

        if (item.empty()) continue;

        if (const int gid = std::stoi(item); gid > 0) {
            const auto tileset = get_tileset_for_gid(gid);

            if (!tileset) {
                continue;
            }

            const int local_id = gid - tileset->firstgid;
            int tileset_columns = 1;

            if (tileset->texture.id > 0) {
                tileset_columns = tileset->texture.width / tileset->tile_width;
            }

            const int ts_x = tileset_columns > 0 ? local_id % tileset_columns : 0;
            const int ts_y = tileset_columns > 0 ? local_id / tileset_columns : 0;

            auto t = std::make_unique<Tile>();
            t->source_rect = {
                static_cast<float>(ts_x * tileset->tile_width),
                static_cast<float>(ts_y * tileset->tile_height),
                static_cast<float>(tileset->tile_width),
                static_cast<float>(tileset->tile_height)
            };
            t->position = {
                static_cast<float>(x * tile_width),
                static_cast<float>(y * tile_height)
            };
            t->gid = gid;
            t->animation = nullptr;
            t->current_animation_tileid = local_id;
            t->collected = false;

            if (auto anim_it = tileset->animations.find(local_id); anim_it != tileset->animations.end()) {
                t->animation = &anim_it->second;
            }

            layer->tiles.push_back(std::move(t));
        }

        x++;
        if (x >= map_width) {
            x = 0;
            y++;
        }
    }

    return true;
}

bool MapManager::parse_object_groups(XMLElement *map_element) {
    XMLElement *object_parts = nullptr;
    for (XMLElement *group = map_element->FirstChildElement("group");
         group;
         group = group->NextSiblingElement("group")) {
        if (const char *name = group->Attribute("name"); name && std::string(name) == "object_parts") {
            object_parts = group;
            break;
        }
    }

    if (!object_parts) return true;

    bool success = true;

    for (XMLElement *objectgroup = object_parts->FirstChildElement("objectgroup");
         objectgroup;
         objectgroup = objectgroup->NextSiblingElement("objectgroup")) {
        const char *layer_name = objectgroup->Attribute("name");
        std::string group_name = layer_name ? layer_name : "unnamed";

        for (XMLElement *object = objectgroup->FirstChildElement("object");
             object;
             object = object->NextSiblingElement("object")) {
            const char *obj_name = object->Attribute("name");
            if (!obj_name) {
                std::cerr << "Object without name in group '" << group_name << "'" << std::endl;
                success = false;
                continue;
            }

            float x = 0.0f, y = 0.0f;
            const XMLError x_result = object->QueryFloatAttribute("x", &x);
            const XMLError y_result = object->QueryFloatAttribute("y", &y);

            if (x_result != XML_SUCCESS || y_result != XML_SUCCESS) {
                std::cerr << "Failed to parse position of object '" << obj_name
                        << "' in group '" << group_name << "'" << std::endl;
                success = false;
                continue;
            }

            spawn_points[obj_name] = Vector2{x, y};
        }
    }

    return success;
}

void MapManager::update_animations() {
    for (auto &layer_ptr: layers) {
        for (auto &tile_ptr: layer_ptr->tiles) {
            if (tile_ptr->animation) {
                tile_ptr->animation->current_time += GetFrameTime() * 1000.0f;

                if (tile_ptr->animation->current_time >=
                    tile_ptr->animation->frames[tile_ptr->animation->current_frame].duration) {
                    tile_ptr->animation->current_time = 0.0f;
                    tile_ptr->animation->current_frame =
                            (tile_ptr->animation->current_frame + 1) % tile_ptr->animation->frames.size();
                }

                int current_tileid = tile_ptr->animation->frames[tile_ptr->animation->current_frame].tileid;
                auto tileset = get_tileset_for_gid(tile_ptr->gid);

                if (tileset) {
                    if (auto it = tileset->individual_textures.find(current_tileid);
                        it != tileset->individual_textures.end()) {
                        tile_ptr->source_rect = {
                            0, 0,
                            static_cast<float>(it->second.width),
                            static_cast<float>(it->second.height)
                        };
                        tile_ptr->current_animation_tileid = current_tileid;
                    } else if (tileset->texture.id > 0) {
                        int tileset_columns = tileset->texture.width / tileset->tile_width;
                        if (tileset_columns > 0) {
                            int ts_x = current_tileid % tileset_columns;
                            int ts_y = current_tileid / tileset_columns;

                            tile_ptr->source_rect = {
                                static_cast<float>(ts_x * tileset->tile_width),
                                static_cast<float>(ts_y * tileset->tile_height),
                                static_cast<float>(tileset->tile_width),
                                static_cast<float>(tileset->tile_height)
                            };
                        }
                    }
                }
            }
        }
    }
}

void MapManager::draw_map() const {
    for (const auto &layer_ptr: layers) {
        for (const auto &tile_ptr: layer_ptr->tiles) {
            if (tile_ptr->collected) {
                continue;
            }

            auto tileset = get_tileset_for_gid(tile_ptr->gid);

            if (tileset) {
                if (tile_ptr->animation) {
                    int current_tileid = tile_ptr->current_animation_tileid;
                    if (auto it = tileset->individual_textures.find(current_tileid);
                        it != tileset->individual_textures.end()) {
                        DrawTextureRec(it->second, tile_ptr->source_rect, tile_ptr->position, WHITE);
                        continue;
                    }
                }

                if (tileset->texture.id > 0) {
                    DrawTextureRec(tileset->texture, tile_ptr->source_rect, tile_ptr->position, WHITE);
                }
            }
        }
    }
}

void MapManager::UnloadMap() {
    for (auto &tileset_ptr: tilesets) {
        if (tileset_ptr->texture.id > 0) {
            UnloadTexture(tileset_ptr->texture);
            tileset_ptr->texture = {0};
        }

        for (auto &[id, texture]: tileset_ptr->individual_textures) {
            if (texture.id > 0) {
                UnloadTexture(texture);
            }
        }
    }

    tilesets.clear();
    layers.clear();
    collidable_tiles.clear();
    spawn_points.clear();
    has_collectibles = false;
}

Vector2 MapManager::get_spawn_point(const std::string &name) const {
    if (const auto it = spawn_points.find(name); it != spawn_points.end()) {
        return it->second;
    }
    return Vector2{0, 0};
}

bool MapManager::is_tile_collidable(const int x, const int y) const {
    if (x < 0 || y < 0 || x >= map_width || y >= map_height) {
        return true;
    }

    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        for (const auto &tile_ptr: (*it)->tiles) {
            const int tile_x = static_cast<int>(tile_ptr->position.x / tile_width);

            if (const int tile_y = static_cast<int>(tile_ptr->position.y / tile_height); tile_x == x && tile_y == y) {
                if (collidable_tiles.contains(tile_ptr->gid)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool MapManager::check_boundary_collision(const Rectangle rect) const {
    const int start_x = static_cast<int>(rect.x / tile_width);
    const int start_y = static_cast<int>(rect.y / tile_height);
    const int end_x = static_cast<int>((rect.x + rect.width) / tile_width);
    const int end_y = static_cast<int>((rect.y + rect.height) / tile_height);

    for (int y = start_y; y <= end_y; y++) {
        for (int x = start_x; x <= end_x; x++) {
            if (is_tile_collidable(x, y)) {
                return true;
            }
        }
    }

    return false;
}

std::optional<CollectibleInfo> MapManager::check_collectible_collision(const Rectangle rect) {
    CollectibleInfo info = {"", 0.0f, {0, 0}};

    if (!has_collectibles) {
        return std::nullopt;
    }

    for (const auto &layer: layers) {
        if (layer->name != "collectibles") {
            continue;
        }

        for (auto &tile: layer->tiles) {
            if (tile->collected) {
                continue;
            }

            Rectangle tile_rect = {
                tile->position.x,
                tile->position.y,
                static_cast<float>(tile_width),
                static_cast<float>(tile_height)
            };

            if (CheckCollisionRecs(rect, tile_rect)) {
                int gid = tile->gid;
                auto tileset = get_tileset_for_gid(gid);

                if (tileset) {
                    auto property_iter = collectible_names.find(gid);
                    if (property_iter != collectible_names.end()) {
                        info.name = property_iter->second;

                        auto value_iter = collectible_values.find(gid);
                        if (value_iter != collectible_values.end()) {
                            info.value = value_iter->second;
                        }
                    }
                }

                info.position = tile->position;
                tile->collected = true;

                return info;
            }
        }
    }

    return std::nullopt;
}
