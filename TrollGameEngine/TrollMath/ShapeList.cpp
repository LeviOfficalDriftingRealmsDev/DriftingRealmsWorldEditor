
#include "ShapeList.hpp"

using namespace TMath;

ShapeList::ShapeList(unsigned int initial_size){
    current_buffer_size = initial_size;
    current_size = 0;
    pointer_list = new Shape*[initial_size];
}

void ShapeList::Append(Shape *new_shape){

    if( (current_size+1) > current_buffer_size){

        ExpandBuffer(current_size);
    }

    pointer_list[current_size] = new_shape;

    ++current_size;
}

void ShapeList::Remove(unsigned int index){

    delete pointer_list[index];

    tmp_pointer_list = new Shape*[current_buffer_size];

    for(unsigned int i=0; i<index; ++i){
        tmp_pointer_list[i] = pointer_list[i];
    }

    for(unsigned int i=index; i<current_size; ++i){
        tmp_pointer_list[i] = pointer_list[i+1];
    }

    pointer_list = tmp_pointer_list;

    --current_size;
}

void ShapeList::Remove(Shape *shape){
    for(unsigned int index = 0;index < current_size;index++){
        if(shape->get_id() == pointer_list[index]->get_id()){
            Remove(index);
            break;
        }
    }
}

const unsigned int ShapeList::GetSize(){
    return current_size;
}

void ShapeList::RemoveLast(){
    Remove(current_size-1);
}

Shape* ShapeList::Get(unsigned int index){
    return pointer_list[index];
}

void ShapeList::SetBufferSize(unsigned int new_buffer_size){

    int buffer_difference = current_buffer_size - new_buffer_size;

    if(buffer_difference > 0){
        ShrinkBuffer( buffer_difference );
    }
    else{
        ExpandBuffer( buffer_difference );
    }

}

void ShapeList::ShrinkBuffer(unsigned int amount){

    current_buffer_size = current_buffer_size-amount;

    // there are elements in the list that must be deleted
    if(current_buffer_size < current_size){
        unsigned int end_i = current_buffer_size+amount;

        for(unsigned int i=current_buffer_size; i<end_i; ++i){
            delete pointer_list[i];
        }

        current_size = current_buffer_size;
    }

    tmp_pointer_list = new Shape*[current_buffer_size];

    for(unsigned int i=0; i<current_size; ++i){
        tmp_pointer_list[i] = pointer_list[i];
    }

    pointer_list = tmp_pointer_list;
}

void ShapeList::ExpandBuffer(unsigned int amount){

    current_buffer_size = current_buffer_size+amount;

    tmp_pointer_list = new Shape*[current_buffer_size];

    for(unsigned int i=0; i<current_size; ++i){
        tmp_pointer_list[i] = pointer_list[i];
    }

    pointer_list = tmp_pointer_list;

}

void ShapeList::Swap(unsigned int index_a, unsigned int index_b){
    Shape *tmp_pointer = pointer_list[index_a];
    pointer_list[index_a] = pointer_list[index_b];
    pointer_list[index_b] = tmp_pointer;
}
