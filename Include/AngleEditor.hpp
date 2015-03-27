#pragma once
#include "../TrollGameEngine/TrollFileSystem/FileHelp.hpp"
#include "NavMapLoadData.hpp"

class AngleEditor{
public:
    void Draw(Canvas &c, const int cam_x,const int cam_y,const float zoom);
    void Input(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom);
    void Save(clan::File &file);
    void LoadFrom(TPath::NavSaveLoadData &data);
    void Generate();
    void GenerateShape();
    std::vector<float> angles;
    std::vector<float> angle_sizes;
    //! should be renamed. Does this point contain an angle?
    std::vector<bool> is_viable_point;
    int GetPointRangeForElement(int &start, int &last, int loc);
    void Flipped_x(int loc);
    void Flipped_y(int loc);
};
