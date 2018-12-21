//
// Created by tonymit on 27.11.18.
//

#ifndef INC_1LAB_CPP_MATIX_READER_H
#define INC_1LAB_CPP_MATIX_READER_H
#include <fstream>
#include <string>
#include <vector>

struct matrix{
    std::vector<int> data;
    matrix(int columns, int rows) :
            data(columns*rows)
    {}
};

class matix_reader {
    std::ifstream file;
    int shape[2];
public:
    matix_reader(char* fname,  int shape_x, int shape_y):file(fname){
        shape[0]=shape_x;
        shape[1]=shape_y;
    };
    int* get_matrix();
};


#endif //INC_1LAB_CPP_MATIX_READER_H
