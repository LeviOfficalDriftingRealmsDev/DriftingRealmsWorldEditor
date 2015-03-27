#include "FileHelp.hpp"

using namespace TPath;
namespace FileHelper{


    std::string get_str(clan::File &file_read){
        std::string str = "";
        int str_len = file_read.read_int32();
        for(int i = 0;i < str_len; i++){
            str.append(1,file_read.read_int8() );
        }
        return str;
    };


    void write_str(std::string str ,clan::File &file_write){
        //! int8 = char
        file_write.write_int32(str.size());
        for(unsigned int i = 0;i < str.size();i++)
            file_write.write_int8(str[i]);
    }
    bool read_bool(clan::File &file_read){
        if(file_read.read_int8() == 0)
            return true;
        else
            return false;
    };
    void write_bool(bool b,clan::File &file_write){
        //! int8 = char - use because write bool doesn't seem to exist...
        if(b == true)
            file_write.write_int8(0);
        else
            file_write.write_int8(1);
    }

    clan::Vec2f read_Vec2f(clan::File &file_read){
        const float x = file_read.read_float();
        const float y = file_read.read_float();
        return clan::Vec2f(x,y);
    };
    void write_Vec2f(clan::Vec2f f,clan::File &file_write){
        file_write.write_float(f.x);
        file_write.write_float(f.y);
    }

    TMath::Point read_TPoint(clan::File &file_read){
        const float x = file_read.read_float();
        const float y = file_read.read_float();
        return TMath::Point(x,y);
    };
    void write_TPoint(TMath::Point f,clan::File &file_write){
        file_write.write_float(f.x);
        file_write.write_float(f.y);
    }

    TPath::NavSaveLoadData read_nav_map(clan::File &file){
        TPath::NavSaveLoadData return_me;

        const unsigned int triangle_count = file.read_uint32();
        const unsigned int edge_count  = file.read_uint32();
        const unsigned int point_count = file.read_uint32();

        for(unsigned int i = 0;i < triangle_count; i++){
            return_me.list_of_triangles.push_back(NavSaveLoadTriangle());
            return_me.list_of_triangles[i].connected_edge_index_list[0] = file.read_int32();
            return_me.list_of_triangles[i].connected_edge_index_list[1] = file.read_int32();
            return_me.list_of_triangles[i].connected_edge_index_list[2] = file.read_int32();
        }
        for(unsigned int i = 0;i < edge_count; i++){
            int a_i = file.read_int32();
            int b_i = file.read_int32();
            return_me.list_of_edges.push_back(NavSaveLoadEdge(a_i,b_i));
        }
        std::vector<int> temp;
        for(unsigned int i = 0;i < point_count; i++){
            return_me.list_of_points.push_back(FileHelper::read_TPoint(file));


            const bool is_chain_point = FileHelper::read_bool(file);
            //! if this is true then we need a new chain
            const bool is_new_chain = FileHelper::read_bool(file);

            if(is_chain_point){
                if(is_new_chain && i != 0){
                    return_me.list_of_chain_points.push_back(temp);
                    temp.clear();
                }
                temp.push_back(i);
            }
        }
        return_me.list_of_chain_points.push_back(temp);

        const unsigned int open_points_list_size = file.read_uint32();
        for(unsigned int i = 0;i < open_points_list_size;i++){
            return_me.list_of_open_points.push_back(file.read_uint32());
        }

        return return_me;
    }



    WorldData *read_world_data(std::string name){
        File file(name,File::OpenMode::open_existing,File::AccessFlags::access_read);
        WorldData *data = new WorldData;

        std::string check_if_DRM_file = FileHelper::get_str(file);
        if(check_if_DRM_file == "Drifting Realms Map File"){
            const unsigned int version = file.read_uint32();
            if(version == VERSION){

                data->mesh_load = read_nav_map(file);


                //! later put these in one for loop, currently too lazy... ugh
                //! I would put them in there own function, but its such a small one
                for(unsigned int i = 0;i < data->mesh_load.list_of_points.size();i++)
                    data->mesh_load.angles.push_back(file.read_float());
                for(unsigned int i = 0;i < data->mesh_load.list_of_points.size();i++)
                    data->mesh_load.angle_sizes.push_back(file.read_float());


                //! determine the amount of elements
                const unsigned int elem_size = file.read_uint32();
                for(unsigned int i = 0;i< elem_size;i++){
                    const unsigned int type = file.read_uint32();
                    if(type == WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_STATIC_OBSTACLE){
                        //! get template_name
                        LoadSaveStaticMesh *mesh = new LoadSaveStaticMesh;
                        mesh->texture_name = FileHelper::get_str(file);

                        mesh->position = FileHelper::read_Vec2f(file);

                        mesh->is_flipped_horizontal = FileHelper::read_bool(file);
                        mesh->is_flipped_vertical   = FileHelper::read_bool(file);

                        data->save_load_static_mesh.push_back(mesh);
                    }
                    else if(type == WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_GAME){
                        const unsigned int type = file.read_uint32();
                        const Vec2f pos = FileHelper::read_Vec2f(file);
                        data->game_element_locations.push_back(pos);
                        data->game_element_types.push_back(GAME_TYPES(type));
                    }
                    else if(type == WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_REGION){
                        const Vec2f pos = FileHelper::read_Vec2f(file);
                        const Vec2f size = FileHelper::read_Vec2f(file);
                        data->areas.push_back(Rectf(pos,Sizef(size.x,size.y)));
                    }
                }



            }else{
                //! wrong version number!
                //! actual dialog later
                std::cout << "WRONG VERSION MAN!\n";
                std::cout << "AKA: THE MAP WAS MADE WITH A DIFFERENT VERSION OF THE DRIFTING REALMS WORLDS EDITOR\n";
            }
        }else{
            //! invalid file type!
            //! actual dialog later
            std::cout << "WRONG FILE FORMAT MAN!\n";
            std::cout << "AKA: THIS ISN'T EVEN A MAP FILE... MAN!\n";
        }
        return data;
    }

}
