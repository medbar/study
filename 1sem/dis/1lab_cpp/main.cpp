

#include <iostream>
#include <thread>
#include <pthread.h>

#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "matix_reader.h"
#include <chrono>
#include "Predict.cpp"
#include "Least_Loaded.cpp"
#include "round_robin.cpp"


int main(int argc, char * argv[]) {
    if (argc != 4){
        std::cout << "round_type_id mul thread_num file_name"<< std::endl;
        exit(1);
    }

    auto start = std::chrono::system_clock::now();
    int shape = 4;
    int round_type_id = atoi(argv[1]);
    int thread_num = atoi(argv[2]);
    std::string fname(argv[3]);

    matrix out_matrix(shape, shape);
    switch (round_type_id) {
        case 0:
            std::cout << "Round_Robin" << std::endl;
            Round_Robin::round_robin(shape, thread_num, fname, out_matrix);
            break;
        case 1:
            std::cout << "Least_Loaded" << std::endl;
            Least_Loaded::least_loaded(shape, thread_num, fname, out_matrix);
            break;
        case 2:
            std::cout << "Predict_Loaded" << std::endl;
            Predict_Loaded::predictive_loaded(shape, thread_num, fname, out_matrix);
            break;
    }


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time: " << diff.count() << " s\n";

    std::cout << "---RESULTS-----" <<std::endl;

    for (size_t i = 0; i < shape; i++) {
        for (size_t j = 0; j < shape; j++) {
            int elem = out_matrix.data[i * shape + j];
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}
