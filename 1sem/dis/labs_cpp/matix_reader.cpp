//
// Created by tonymit on 27.11.18.
//

#include "matix_reader.h"



int* matix_reader::get_matrix() {
    int *A = new int[16];
    if (file.is_open()){
        for ( size_t i=0; i< shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                file >> A[i*4+j];
            }
        }
        return A;
    }
    return NULL;
}