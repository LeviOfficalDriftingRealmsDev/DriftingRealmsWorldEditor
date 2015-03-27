#pragma once
#include "Precomp.hpp"
//#include "../TrollGame/TrollMath.hpp"
#include "../TrollPath/SaveLoadMeshData.hpp"

#include "../TrollMath/TrollMath.hpp"
#include "../TrollPath/TrollPath.hpp"

enum WORLD_EDITOR_ELEMENT_TYPES{
    WORLD_EDITOR_UNIT,
    WORLD_EDITOR_EFFECTOR,
    WORLD_EDITOR_WAYPOINT,
    WORLD_EDITOR_REGION,
    WORLD_EDITOR_STATIC_OBSTACLE,
    WORLD_EDITOR_DYNAMIC_OBSTACLE,
    WORLD_EDITOR_WEAPON,
    WORLD_EDITOR_ENGINE,
    WORLD_EDITOR_PROJECTILE,
    WORLD_EDITOR_WORLD,
    WORLD_EDITOR_GAME,//! resources, spawn points, module placement etc...
    WORLD_EDITOR_UNDEFINED
};

enum GAME_TYPES{
    RESOURCE_RED = 0,
    RESOURCE_PURPLE,
    RESOURCE_BLUE
};


using namespace clan;

//! used for the file. increase when compatability changes.
//! placing it in the cpp file prevents using an old FileHelp with a new one
#define VERSION 0

struct LoadSaveStaticMesh{
    Vec2f position;
    std::string texture_name;
    bool is_flipped_horizontal = false;
    bool is_flipped_vertical = false;
};

struct WorldData{
    std::vector<LoadSaveStaticMesh *> save_load_static_mesh;
    TPath::NavSaveLoadData mesh_load;

    std::vector<Rectf> areas;

    std::vector<Vec2f> game_element_locations;
    std::vector<GAME_TYPES> game_element_types;
};

namespace FileHelper{
    std::string get_str(clan::File &file_read);
    void write_str(std::string str ,clan::File &file_write);
    bool read_bool(clan::File &file_read);
    void write_bool(bool b,clan::File &file_write);
    clan::Vec2f read_Vec2f(clan::File &file_read);
    void write_Vec2f(clan::Vec2f f,clan::File &file_write);
    TMath::Point read_TPoint(clan::File &file_read);
    void write_TPoint(TMath::Point f,clan::File &file_write);

    TPath::NavSaveLoadData read_nav_map(clan::File &file);

    WorldData *read_world_data(std::string name);

};

