#pragma once
#include "Precomp.hpp"
#include "../TrollGameEngine/TrollFileSystem/FileHelp.hpp"

using namespace clan;

CollisionOutline CreateOutlineFromPoints(std::vector<Vec2f> chain_points);







class ElementTemplate{
public:
    static int cnt;
    int id = 0;
    std::vector<Vec2f> chain_points;

    void Draw(Canvas &c, float x,float y, float zoom,Vec2f flip,bool selected){
        if(flip.x == -1){
            x+=texture->get_width();
        }
//        if(flip.y == -1){
//            y+=texture->get_width();
//        }
        texture->draw(c, Rectf(x/zoom,y/zoom,
                                (x+texture->get_width()*flip.x)/zoom,
                                (y+texture->get_height()*flip.y)/zoom));
    }

    ElementTemplate(){
        id = cnt++;
    }

    std::string texture_name;
    std::string template_name;
//    clan::Image *texture=0;
    clan::Sprite *texture=0;
    TMath::Point *points_list = 0;
    unsigned int points_list_cnt;
    unsigned int amount_of_points_list_cnts;

};

class WorldElement{
public:
    WorldElement(){
        ;
    }
    WORLD_EDITOR_ELEMENT_TYPES type;
    bool selected = false;
    virtual void SetPosition(Vec2f f) = 0;
    virtual void Move(Vec2f f) = 0;
    float GetX(){
        return element_position.x;
    }
    float GetY(){
        return element_position.y;
    }
    Vec2f GetPos(){
        return element_position;
    }
    void StartGrab(Vec2f mouse){
        last_position = GetPos() - mouse;
    }
    void Grabbing(Vec2f mouse){
        SetPosition(mouse + last_position);
    }
    void StartScale(Vec2f mouse){
        last_scale = scale - Sizef(mouse.x,mouse.y);
    }
    virtual void Scaling(Vec2f mouse){
        scale = Sizef(mouse.x,mouse.y) + last_scale;
    }
    virtual void Save(clan::File &file) = 0;
    virtual void Delete() = 0;
    virtual void Draw(Canvas &c, Vec2f cam, float zoom)=0;
    virtual void MoveOutline(float x, float y)=0;
    virtual bool IsPointInside(Pointf pnt)=0;
    virtual void ScaleCollision(float zoom,Vec2f cam)=0;

    virtual WorldElement *Clone()=0;
    virtual WorldElement *MirrorX(Rectf map_rect)=0;
protected:
    clan::Vec2f element_position;
    clan::Vec2f last_position;
    clan::Sizef scale;
    clan::Sizef last_scale;
};

class StaticElement : public WorldElement {
public:
    ElementTemplate *element_template_data=0;
    std::vector<Vec2f> chain_points;
    CollisionOutline outline;
    bool is_flipped_horizontal = false;
    bool is_flipped_vertical = false;
    StaticElement() : WorldElement(){
        type = WORLD_EDITOR_STATIC_OBSTACLE;
    }
    void SetPosition(Vec2f f){
        element_position = f;
    }
    void Move(Vec2f f){
        element_position += f;
    }


    void FlipCollisionHorizontal(){
        int farthest_x = chain_points[0].x;
        for(unsigned int i = 0;i < chain_points.size();i++){
            if(chain_points[i].x > farthest_x) farthest_x = chain_points[i].x;
        }
        for(unsigned int i = 0;i < chain_points.size();i++){
            chain_points[i].x = farthest_x-chain_points[i].x;
        }
        CreateOutline();
    }
    void FlipCollisionVertical(){
        int farthest_y = chain_points[0].y;
        for(unsigned int i = 0;i < chain_points.size();i++){
            if(chain_points[i].y > farthest_y) farthest_y = chain_points[i].y;
        }
        int offset = farthest_y;
        if(is_flipped_vertical == false){
            offset *= -1;
        }
        for(unsigned int i = 0;i < chain_points.size();i++){
            chain_points[i].y = farthest_y-chain_points[i].y-offset;
        }
        CreateOutline();
    }
    void Delete(){
    }
    void Save(clan::File &file){
        file.write_uint32(WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_STATIC_OBSTACLE);

        FileHelper::write_str(element_template_data->texture_name, file);
//        FileHelper::write_Vec2f(GetPos() + Vec2f(element_template_data->texture->get_width()/2,element_template_data->texture->get_height()/2),file);
        FileHelper::write_Vec2f(GetPos(),file);

        FileHelper::write_bool(is_flipped_horizontal,file);
        FileHelper::write_bool(is_flipped_vertical  ,file);

//        file.write_uint32(element_template_data->chain_points.size());
//        for(unsigned int i = 0;i < element_template_data->chain_points.size();i++){
//            FileHelper::write_Vec2f(element_template_data->chain_points[i],file);
//        }
    }
    float GetPointX(int i){
        return element_position.x + chain_points[i].x;
    }
    float GetPointY(int i){
        return element_position.y + chain_points[i].y;
    }
    void Draw(Canvas &c, Vec2f cam, float zoom){
        Vec2f flip(1,1);
        if(is_flipped_horizontal){
            flip.x = -1;
        }
        if(is_flipped_vertical){
            flip.y = -1;
        }
        const int x = GetX() - cam.x;
        const int y = GetY() - cam.y;
        element_template_data->Draw(c,x,y,zoom,flip,selected);


        if(selected){
            outline.set_translation(x/zoom,y/zoom);
            outline.draw(0,0,Colorf::white,c);
        }
    }
    void CreateOutline(){
        outline = CreateOutlineFromPoints(chain_points);
    }
    void MoveOutline(float x, float y){
        outline.set_translation(x,y);
    }
    bool IsPointInside(Pointf pnt){
        return outline.point_inside(pnt);
    }
    void ScaleCollision(float zoom,Vec2f cam){
//        Vec2f flip(1,1);
//        if(is_flipped_horizontal){
//            flip.x = -1;
//        }
//        if(is_flipped_vertical){
//            flip.y = -1;
//        }

        outline.set_scale(1/zoom,1/zoom);
        outline.set_translation((GetX() - cam.x)/zoom,(GetY() - cam.y)/zoom);
    }
    WorldElement *Clone(){
        StaticElement *new_clone = new StaticElement();
        new_clone->outline = outline;
        new_clone->chain_points = chain_points;
        new_clone->is_flipped_horizontal = is_flipped_horizontal;
        new_clone->is_flipped_vertical   = is_flipped_vertical;
        new_clone->SetPosition(GetPos());
        new_clone->element_template_data = element_template_data;
        return new_clone;
    }
    WorldElement *MirrorX(Rectf map_rect){
        //! don't make a clone of a clone, so we use a pointer
        StaticElement *new_clone = static_cast<StaticElement *>(Clone());
        float map_right = map_rect.get_top_right().x;
        new_clone->SetPosition(Vec2f(map_right - new_clone->GetX() + map_right - element_template_data->texture->get_width(),new_clone->GetY()));

        new_clone->FlipCollisionHorizontal();

        new_clone->is_flipped_horizontal = !new_clone->is_flipped_horizontal;

        return new_clone;
    }
};


class AreaElement : public WorldElement {
public:

    std::vector<Vec2f> chain_points;
    CollisionOutline outline;


    void Draw(Canvas &c, Vec2f cam, float zoom){
        if(selected){
            c.fill_rect(Rectf((-cam.x + GetX())/zoom,(-cam.y + GetY())/zoom,scale/zoom),Colorf(255, 255, 0,200));
        }else{
            c.fill_rect(Rectf((-cam.x + GetX())/zoom,(-cam.y + GetY())/zoom,scale/zoom),Colorf(255, 165, 0,165));
        }
    }

    void CreateOutline(){
        outline = CreateOutlineFromPoints(chain_points);
    }

    AreaElement() : WorldElement(){
        type = WORLD_EDITOR_REGION;
        chain_points.push_back(Vec2f(0,0));
        chain_points.push_back(Vec2f(100,0));
        chain_points.push_back(Vec2f(100,100));
        chain_points.push_back(Vec2f(0,100));
        scale = Sizef(100,100);
        CreateOutline();
    }
    AreaElement(Vec2f pos) : WorldElement(){
        type = WORLD_EDITOR_REGION;
        chain_points.push_back(Vec2f(0,0));
        chain_points.push_back(Vec2f(100,0));
        chain_points.push_back(Vec2f(100,100));
        chain_points.push_back(Vec2f(0,100));
        scale = Sizef(100,100);
        CreateOutline();
        SetPosition(pos);
    }
    void SetRect(Rectf f){
        SetPosition(f.get_top_left());
        SetScale(Vec2f(f.get_width(),f.get_height()));
    }
    void SetPosition(Vec2f f){
        element_position = f;
    }
    void Move(Vec2f f){
        element_position += f;
    }
    void Delete(){
    }
    void Save(clan::File &file){
        file.write_uint32(WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_REGION);
        FileHelper::write_Vec2f(GetPos(),file);
        FileHelper::write_Vec2f(Vec2f(scale.width,scale.height),file);
    }
    void MoveOutline(float x, float y){
        outline.set_translation(x,y);
    }
    bool IsPointInside(Pointf pnt){return outline.point_inside(pnt);}
    void ScaleCollision(float zoom,Vec2f cam){
        outline.set_translation((GetX() - cam.x)/zoom,(GetY() - cam.y)/zoom);
        outline.set_scale(1/zoom,1/zoom);
    }
    void SetScale(Vec2f scale){
        this->scale = scale;
        chain_points.clear();
        chain_points.push_back(Vec2f(0,0));
        chain_points.push_back(Vec2f(scale.x,0));
        chain_points.push_back(Vec2f(scale.x,scale.y));
        chain_points.push_back(Vec2f(0,scale.y));
        CreateOutline();
    }
    void Scaling(Vec2f mouse){
        SetScale(Sizef(mouse.x,mouse.y) + last_scale);
    }
    WorldElement *Clone(){
        AreaElement *new_clone = new AreaElement();
        new_clone->chain_points = chain_points;
        new_clone->outline = outline;
        new_clone->scale = scale;
        new_clone->SetPosition(GetPos());
        return new_clone;
    }
    WorldElement *MirrorX(Rectf map_rect){
        //! don't make a clone of a clone
        AreaElement *new_clone = static_cast<AreaElement *>(Clone());
        float map_right = map_rect.get_top_right().x;

        new_clone->SetPosition(Vec2f(map_right-new_clone->GetPos().x+scale.width,new_clone->GetPos().y));
//        new_clone->SetPosition(Vec2f(map_right,map_right));

        return new_clone;
    }
};


class GameElement : public WorldElement {
public:
    int colour = 0;//! 0 = red, 1 = purple, 2 = blue
    std::vector<Vec2f> chain_points;
    CollisionOutline outline;
    clan::Image *texture=0;

    bool first_render = true;
    void Draw(Canvas &c, Vec2f cam, float zoom){
        //! we need access to canvas so we use this "hack"
        if(first_render){
            if(colour == 0){
                texture = new Image(c,"./GameResources/Resources/red_diamond.png");
            }else if(colour == 1){
                texture = new Image(c,"./GameResources/Resources/purple_diamond.png");
            }else if(colour == 2){
                texture = new Image(c,"./GameResources/Resources/blue_diamond.png");
            }
            first_render = false;
        }

        texture->draw(c, Rectf((element_position.x-cam.x)/zoom,(element_position.y-cam.y)/zoom,
                                ((element_position.x-cam.x)+texture->get_width())/zoom,
                                ((element_position.y-cam.y)+texture->get_height())/zoom));
        if(selected){
            outline.set_translation(element_position.x/zoom,element_position.y/zoom);
            outline.draw(-cam.x/zoom,-cam.y/zoom,Colorf::white,c);
        }
    }

    void CreateOutline(){
        outline = CreateOutlineFromPoints(chain_points);
    }

    GameElement(Vec2f pos,int _colour) : WorldElement(){
        type = WORLD_EDITOR_GAME;
        chain_points.push_back(Vec2f(0,0));
        chain_points.push_back(Vec2f(200,0));
        chain_points.push_back(Vec2f(200,320));
        chain_points.push_back(Vec2f(0,320));
        CreateOutline();
        SetPosition(pos);
        colour = _colour;
    }
    void SetPosition(Vec2f f){
        element_position = f;
    }
    void Move(Vec2f f){
        element_position += f;
    }
    void Delete(){
    }
    void Save(clan::File &file){
        file.write_uint32(WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_GAME);
        file.write_uint32(colour);
        FileHelper::write_Vec2f(GetPos(),file);
    }
    void MoveOutline(float x, float y){
        outline.set_translation(x,y);
    }
    bool IsPointInside(Pointf pnt){return outline.point_inside(pnt);}
    void ScaleCollision(float zoom,Vec2f cam){
        outline.set_translation((GetX() - cam.x)/zoom,(GetY() - cam.y)/zoom);
        outline.set_scale(1/zoom,1/zoom);
    }
    WorldElement *Clone(){
//        GameElement *elem = new GameElement();
    }
    WorldElement *MirrorX(Rectf map_rect){
        //! don't make a clone of a clone
        GameElement *new_clone = static_cast<GameElement *>(Clone());
        float map_right = map_rect.get_top_right().x;

        return new_clone;
    }
};
