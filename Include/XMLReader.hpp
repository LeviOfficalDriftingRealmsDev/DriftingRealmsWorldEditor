#pragma once

#include "World.hpp"
#include "pugixml.hpp"

using namespace pugi;

struct DriftingRealmsXMLReader{
    static void reload_element_templates(std::vector<ElementTemplate*> &element_template_list, clan::Canvas &canvas){

        xml_document doc;

        xml_parse_result result = doc.load_file("templates.xml");

        if(result){
            ElementTemplate::cnt = 0;
            xml_node element_section = doc.child("elements"); //.append_child();

//            std::vector<int> ids;
            std::vector<std::string> names;
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->type == WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_STATIC_OBSTACLE){
                    std::cout << static_cast<StaticElement *>(elem)->element_template_data->template_name << "\n";
//                    std::cout << ((StaticElement *)elem)->element_template_data->id << "\n";
                    names.push_back(static_cast<StaticElement *>(elem)->element_template_data->template_name);
//                    ids.push_back(((StaticElement *)elem)->element_template_data->id);
                }
            }
//            std::cout << ids.size() << "\n";
            if(element_section){
                element_template_list.clear();

                for (xml_node_iterator it = element_section.begin(); it != element_section.end(); ++it){

                    ElementTemplate *new_element = new ElementTemplate;

                    new_element->template_name = it->attribute("template_name").value();

//                    int enum_type_value = it->attribute("static").as_int();

//                    new_element->type = static_cast<WORLD_EDITOR_ELEMENT_TYPES>(enum_type_value);


                    for (xml_node chain_section = it->child("chain_shape"); chain_section; chain_section = chain_section.next_sibling("chain_shape")){
//                        xml_node chain_section = it->child("chain_shape");
                        for (xml_node_iterator c_it = chain_section.begin(); c_it != chain_section.end(); ++c_it){
                            new_element->chain_points.push_back(
                                clan::Vec2f(
                                    c_it->attribute("x").as_int(),
                                    c_it->attribute("y").as_int()
                                )
                            );
                        }
                    }




                    /// set this to an itterator later when multiple textures per
                    xml_node texture_section = it->child("textures");
                    new_element->texture_name = texture_section.child("texture").attribute("texture_name").value();

                    new_element->texture = new clan::Sprite(canvas,new_element->texture_name);

                    element_template_list.push_back(new_element);
                }

            }
            for(auto &new_element : element_template_list){
                new_element->amount_of_points_list_cnts = new_element->chain_points.size();
                new_element->points_list = new TMath::Point[new_element->chain_points.size()];
                new_element->points_list_cnt = new_element->chain_points.size();
                for(unsigned int ii = 0;ii < new_element->chain_points.size(); ii++){
                    new_element->points_list[ii] = TMath::Point(new_element->chain_points[ii].x,new_element->chain_points[ii].y);
                }
            }
//            int iter = 0;
//            for(auto &elem : GLOBAL_VARS::element_list){
//                if(elem->type == WORLD_EDITOR_ELEMENT_TYPES::STATIC_OBSTACLE){
//                    for(auto &template_item : element_template_list){
//                        if(ids[iter] == template_item->id){
//                            static_cast<StaticElement *>(elem)->element_template_data = template_item;
//                        }
//                    }
//                    iter++;
//                }
//            }
            int iter = 0;
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->type == WORLD_EDITOR_ELEMENT_TYPES::WORLD_EDITOR_STATIC_OBSTACLE){
                    for(auto &template_item : element_template_list){
                        if(names[iter] == template_item->template_name){
                            static_cast<StaticElement *>(elem)->element_template_data = template_item;
                        }
                    }
                    iter++;
                }
            }
        }
        else{
            clan::Console::write("error: could not open xml file(templates.xml)\n\tyour xml file could be corrupt or missing.");
        }
        std::cout << "MADE IT!\n";
    }




//    static void reload_element_templates(std::vector<ElementTemplate*> &element_template_list){
//
//        xml_document doc;
//
//        xml_parse_result result = doc.load_file("templates.xml");
//
//        if(result){
//
//            xml_node element_section = doc.child("elements"); //.append_child();
//
//            if(element_section){
//
//                element_template_list.clear();
//
//                for (xml_node_iterator it = element_section.begin(); it != element_section.end(); ++it){
//
//                    ElementTemplate *new_element = new ElementTemplate;
//
//                    new_element->template_name = it->attribute("template_name").value();
//
//                    int enum_type_value = it->attribute("static").as_int();
//
////                    new_element->type = static_cast<ELEMENT_TYPES>(enum_type_value);
//
//                    xml_node chain_section = it->child("chain_shape");
//                    xml_node element_template = it->child("element_template");
//
//                    for (xml_node_iterator e_it = element_template.begin(); e_it != element_template.end(); ++e_it){
//                        std::vector <Vec2f> chain;
//                        for (xml_node_iterator c_it = chain_section.begin(); c_it != chain_section.end(); ++c_it){
//    //                        new_element->chain_points.push_back(
//    //                            clan::Vec2f(
//    //                                c_it->attribute("x").as_int(),
//    //                                c_it->attribute("y").as_int()
//    //                            )
//    //                        );
//                            chain.push_back(
//                                clan::Vec2f(
//                                    c_it->attribute("x").as_int(),
//                                    c_it->attribute("y").as_int()
//                                )
//                            );
//                        }
//                        new_element->chain_points.push_back(chain);
//                    }
//                    new_element->CreateOutline();
//
//                    element_template_list.push_back(new_element);
//                }
//
//            }
//        }
//        else{
//            clan::Console::write("error: could not open xml file(templates.xml)\n\tyour xml file could be corrupt or missing.");
//        }
//    }

};

struct DriftingRealmsXMLWriter{


    static void write_element_template_to_xml_file(ElementTemplate &element_template){

        xml_document doc;

        xml_parse_result result = doc.load_file("templates.xml");

        if(result){

            xml_node root_doc_node = doc.child("elements"); //.append_child();

            xml_node new_element_node = root_doc_node.append_child("element_template");

            new_element_node.append_attribute("template_name");
            new_element_node.attribute("template_name").set_value(element_template.template_name.c_str());

            new_element_node.append_attribute("static");
//            new_element_node.attribute("static").set_value(element_template.type);


            // save the chain shape into the file
            xml_node elemenet_chain = new_element_node.append_child("chain_shape");

            for(unsigned int i=0; i<element_template.chain_points.size(); ++i){
                xml_node new_point = elemenet_chain.append_child("point");
                new_point.append_attribute("x");
                new_point.append_attribute("y");
                new_point.attribute("x").set_value(element_template.chain_points[i].x);
                new_point.attribute("y").set_value(element_template.chain_points[i].y);
            }

            // add the textures
            xml_node textures_node = new_element_node.append_child("textures");
            xml_node texture_node = textures_node.append_child("texture");
            texture_node.append_attribute("texture_name");
            texture_node.attribute("texture_name").set_value(element_template.texture_name.c_str());

            // save to file
            doc.save_file("templates.xml");
        }
        else{
            clan::Console::write("error: could not open xml file(templates.xml)\n\tyour xml file could be corrupt or missing.");
        }
    }

    static void remove_element_template_from_xml_file(ElementTemplate &element_template){

        xml_document doc;

        xml_parse_result result = doc.load_file("templates.xml");

        if(result){

            xml_node element_section = doc.child("elements"); //.append_child();

            if(element_section){

                xml_node node_to_remove;

                for (xml_node_iterator it = element_section.begin(); it != element_section.end(); ++it){
                    if(it->attribute("template_name").value() == element_template.template_name){
                        node_to_remove = *it;
                        break;
                    }
                }

                element_section.remove_child(node_to_remove);

                // save to file
                doc.save_file("templates.xml");
            }
        }
        else{
            clan::Console::write("error: could not open xml file(templates.xml)\n\tyour xml file could be corrupt or missing.");
        }
    }

};
