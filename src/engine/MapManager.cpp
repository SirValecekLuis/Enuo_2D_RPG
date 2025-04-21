#include "MapManager.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace tinyxml2;
using std::cout, std::endl;

MapManager::MapManager()
    : map_width(0), map_height(0), tile_width(0), tile_height(0), tileset_texture{} {
}

MapManager::~MapManager() {
    UnloadMap();
}

bool MapManager::load_map(const std::string& file_path, const std::string& tileset_path) {
    UnloadMap();

    XMLDocument doc;
    XMLError result = doc.LoadFile(file_path.c_str());
    if (result != XML_SUCCESS) {
        std::cerr << "Map was not loaded successfully. " << doc.ErrorStr() << std::endl;
        return false;
    }

    XMLElement* map_element = doc.FirstChildElement("map");
    if (!map_element) {
        std::cerr << "Map element not found" << std::endl;
        return false;
    }

    map_element->QueryIntAttribute("width", &map_width);
    map_element->QueryIntAttribute("height", &map_height);
    map_element->QueryIntAttribute("tilewidth", &tile_width);
    map_element->QueryIntAttribute("tileheight", &tile_height);

    tileset_texture = LoadTexture(tileset_path.c_str());

    // Parse tileset for collision information
    XMLElement* tileset_element = map_element->FirstChildElement("tileset");
    if (!parse_tileset(tileset_element)) {
        std::cerr << "Failed to parse tileset data" << std::endl;
        UnloadMap();
        return false;
    }

    // Parse layers (map and decorations)
    if (!parse_layers(map_element)) {
        std::cerr << "Failed to parse map layers" << std::endl;
        UnloadMap();
        return false;
    }

    // Parse object groups (spawn points)
    if (!parse_object_groups(map_element)) {
        std::cerr << "Failed to parse object groups" << std::endl;
        UnloadMap();
        return false;
    }

    return true;
}

bool MapManager::parse_tileset(XMLElement* tileset_element) {
    if (!tileset_element) return false;

    // Iterate through all tile elements to find those with collision data
    for (XMLElement* tile_element = tileset_element->FirstChildElement("tile");
         tile_element;
         tile_element = tile_element->NextSiblingElement("tile")) {
        int tile_id = 0;
        tile_element->QueryIntAttribute("id", &tile_id);

        // Check if this tile has objectgroup (collision data)
        if (XMLElement* objectgroup = tile_element->FirstChildElement("objectgroup")) {
            // Mark this tile as collidable (add GID which is tile_id + 1)
            collidable_tiles.insert(tile_id + 1);
        }
    }

    return true;
}

bool MapManager::parse_layers(XMLElement* map_element) {
    if (!map_element) return false;

    // Look for a group that contains the map layers
    XMLElement* map_parts = nullptr;
    for (XMLElement* group = map_element->FirstChildElement("group");
         group;
         group = group->NextSiblingElement("group")) {
        const char* name = group->Attribute("name");
        if (name && std::string(name) == "map_parts") {
            map_parts = group;
            break;
        }
    }

    if (!map_parts) {
        // If no map_parts group is found, try to find layers directly under map
        map_parts = map_element;
    }

    // Parse all layers
    for (XMLElement* layer_element = map_parts->FirstChildElement("layer");
         layer_element;
         layer_element = layer_element->NextSiblingElement("layer")) {
        Layer layer;
        const char* name = layer_element->Attribute("name");
        layer.name = name ? name : "unnamed";

        if (!parse_layer_data(layer_element, layer)) {
            return false;
        }

        layers.push_back(layer);
    }

    return true;
}

bool MapManager::parse_layer_data(XMLElement* layer_element, Layer& layer) const {
    const XMLElement* data = layer_element->FirstChildElement("data");
    if (!data) return false;

    const char* csv_data = data->GetText();
    if (!csv_data) return false;

    const std::string csv_string(csv_data);
    std::stringstream ss(csv_string);
    std::string item;
    int x = 0;
    int y = 0;

    while (std::getline(ss, item, ',')) {
        // Skip newlines and other non-numerical characters
        if (item.find_first_not_of("0123456789") != std::string::npos) {
            std::erase_if(item, [](const char c) {
                return !std::isdigit(c);
            });
        }

        if (item.empty()) continue;

        int gid = std::stoi(item);
        if (gid > 0) {
            int tileset_columns = static_cast<int>(tileset_texture.width / tile_width);

            int ts_x = (gid - 1) % tileset_columns;
            int ts_y = (gid - 1) / tileset_columns;

            Tile t;
            t.source_rect = {
                static_cast<float>(ts_x * tile_width),
                static_cast<float>(ts_y * tile_height),
                static_cast<float>(tile_width),
                static_cast<float>(tile_height)
            };
            t.position = {
                static_cast<float>(x * tile_width),
                static_cast<float>(y * tile_height)
            };
            t.gid = gid;

            layer.tiles.push_back(t);
        }

        x++;
        if (x >= map_width) {
            x = 0;
            y++;
        }
    }

    return true;
}

bool MapManager::parse_object_groups(XMLElement* map_element) {
    // Look for the object_parts group
    XMLElement* object_parts = nullptr;
    for (XMLElement* group = map_element->FirstChildElement("group");
         group;
         group = group->NextSiblingElement("group")) {
        const char* name = group->Attribute("name");
        if (name && std::string(name) == "object_parts") {
            object_parts = group;
            break;
        }
    }

    if (!object_parts) return true; // Not an error if no object_parts

    // Parse all object groups
    for (XMLElement* objectgroup = object_parts->FirstChildElement("objectgroup");
         objectgroup;
         objectgroup = objectgroup->NextSiblingElement("objectgroup")) {
        const char* layer_name = objectgroup->Attribute("name");
        std::string group_name = layer_name ? layer_name : "unnamed";

        // Parse spawn points and other objects
        for (XMLElement* object = objectgroup->FirstChildElement("object");
             object;
             object = object->NextSiblingElement("object")) {
            const char* obj_name = object->Attribute("name");
            if (!obj_name) continue;

            float x = 0.0f, y = 0.0f;
            object->QueryFloatAttribute("x", &x);
            object->QueryFloatAttribute("y", &y);

            // Store the spawn point
            spawn_points[obj_name] = Vector2{x, y};
        }
    }

    return true;
}

void MapManager::draw_map() const {
    // Draw all layers in order
    for (const auto& layer : layers) {
        for (const auto& tile : layer.tiles) {
            DrawTextureRec(tileset_texture, tile.source_rect, tile.position, WHITE);
        }
    }
}

void MapManager::UnloadMap() {
    if (tileset_texture.id > 0) {
        UnloadTexture(tileset_texture);
        tileset_texture = {};
    }
    layers.clear();
    collidable_tiles.clear();
    spawn_points.clear();
}

Vector2 MapManager::get_spawn_point(const std::string& name) const {
    if (const auto it = spawn_points.find(name); it != spawn_points.end()) {
        return it->second;
    }
    return Vector2{0, 0}; // Return origin if spawn point not found
}

bool MapManager::is_tile_collidable(const int x, const int y) const {
    if (x < 0 || y < 0 || x >= map_width || y >= map_height) {
        return true; // Out of bounds is considered collidable
    }

    // Check all layers in reverse (top to bottom)
    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        for (const auto& tile : it->tiles) {
            const int tile_x = static_cast<int>(tile.position.x / tile_width);

            if (int tile_y = static_cast<int>(tile.position.y / tile_height); tile_x == x && tile_y == y) {
                // Check if this tile's gid is in the collidable set
                if (collidable_tiles.find(tile.gid) != collidable_tiles.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool MapManager::check_collision(const Rectangle rect) const {
    // Check collision with map tiles
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
