#pragma once

#include "Shape.hpp"

namespace TMath{

    class ShapeList{
        public:
            ShapeList(unsigned int initial_size);
            void Append(Shape *new_shape);
            void Remove(unsigned int index);
            void Remove(Shape *shape);
            const unsigned int GetSize();
            void RemoveLast();
            Shape* Get(unsigned int index);
            void SetBufferSize(unsigned int new_buffer_size);
            void ShrinkBuffer(unsigned int amount);
            void ExpandBuffer(unsigned int amount);
            void Swap(unsigned int index_a, unsigned int index_b);
        private:
            unsigned int current_buffer_size;
            unsigned int current_size;
            Shape **pointer_list;
            Shape **tmp_pointer_list;
    };
};
