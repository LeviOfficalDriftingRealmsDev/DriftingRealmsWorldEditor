#include "Algorithm.hpp"

using namespace Troll;

void Algorithm::SelectionSort( const unsigned int array_size, float *list_of_numbers, int *list_of_indexes ){

    float compare_value = 0;
    unsigned int track_index = 0;

    for(unsigned int i=0; i<array_size; ++i){
        for(unsigned int ii=0; ii<array_size-i; ++ii){
            //! find the largest value and stick it on the end of the list
            if( compare_value < list_of_numbers[ii] ){
                compare_value = list_of_numbers[ii];
                track_index = ii;
            }
        }

        const unsigned int swap_index = array_size-(i+1);

        //! make sure that the number isnt already at the end and will be swapped back to the middle
        if(track_index != swap_index){
            Algorithm::Swap(track_index, swap_index, list_of_numbers);

            if(list_of_indexes){
                Algorithm::Swap(track_index, swap_index, list_of_indexes);
            }
        }

        compare_value = 0;
    }

}

void Algorithm::Swap( const unsigned int index_a, const unsigned int index_b, float *array){
    const float swap_tmp = array[index_a];
    array[index_a] = array[index_b];
    array[index_b] = swap_tmp;
}

void Algorithm::Swap( const unsigned int index_a, const unsigned int index_b, int *array){
    const int swap_tmp = array[index_a];
    array[index_a] = array[index_b];
    array[index_b] = swap_tmp;
}
