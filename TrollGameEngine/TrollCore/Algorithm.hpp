#pragma once
#include <iostream>

namespace Troll{

    struct Algorithm{

        //! a modified version of selection sort that allows you to track the index of the
        static void SelectionSort( const unsigned int array_size, float *list_of_numbers, int *list_of_indexes=0 );
        static void Swap( const unsigned int index_a, const unsigned int index_b, float *array);
        static void Swap( const unsigned int index_a, const unsigned int index_b, int *array);

    };
}
