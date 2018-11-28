
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


struct matrix{
    std::vector<int> data;
    matrix(int columns, int rows) :
            data(columns*rows)
    {}
};


//template <typename T>
class block_write_queue{
    std::queue<std::pair<int,int>> data;
    std::mutex w_lock;
    //std::mutex r_tasks;

public:
    bool done = false;
    bool have_tasks = false;
    block_write_queue() = default;
    std::pair<int,int>* pop(){
        w_lock.lock();
        if (data.empty()){
            have_tasks = false;
            w_lock.unlock();
            return NULL;
        }
        std::pair<int,int> *e = new std::pair<int,int>(data.front());
        data.pop();
        have_tasks = not data.empty();
        w_lock.unlock();
        return e;
    };

    void push(std::pair<int,int> e){
        w_lock.lock();
        data.push(e);
        have_tasks = true;
        w_lock.unlock();
    }
};


std::mutex out_lock;

void calculator(block_write_queue * q, std::vector<matrix*> *data_ptr, matrix * out_matrix, int shape) {
    std::vector<matrix*>& data =  *data_ptr;

    //std::cout<<"thread function started" << std::endl;
    while (true){
        //std::cout<<"thread start task" << std::endl;
        if (q->have_tasks) {
            std::pair<int, int> *task = q->pop();
            if (not task)
                continue;
            matrix result_matrix = matrix(shape, shape);

            //all_matrix_lock.unlock();
            matrix *xm = data[task->first];
            matrix *ym = data[task->second];
            //all_matrix_lock.unlock();
            for (size_t i = 0; i < shape; i++)
                for (size_t j = 0; j < shape; j++) {
                    int c = 0;
                    for (size_t g = 0; g < shape; g++) {
                        int x = xm->data[i * shape + g];
                        int y = ym->data[g * shape + j];
                        c += x * y;
                    }
                    result_matrix.data[i * shape + j] = c;
                }
            out_lock.lock();
            for (size_t i = 0; i < shape; i++)
                for (size_t j = 0; j < shape; j++)
                    out_matrix->data[i * shape + j] += result_matrix.data[i * shape + j];
            out_lock.unlock();

        } else if (q->done)
            return;
    }

}



int main(int argc, char * argv[]) {
    int shape = 4;
    std::vector<matrix*> all_matrix;
    block_write_queue tasks;

    if (argc != 3){
        std::cout << "mul thr_num file_name"<< std::endl;
        exit(1);
    }

    auto start = std::chrono::system_clock::now();
    int thread_num = atoi(argv[1]);
    std::string fname(argv[2]);


    matrix out_matrix(shape, shape);
    std::vector<std::thread> thrs;
    for (size_t i =0; i<thread_num; i++)
        thrs.push_back(std::thread(calculator, &tasks, &all_matrix, &out_matrix, shape));

    //std::thread thr2(anyFunc, &tasks, &all_matrix, &out_matrix, shape);

    std::ifstream file(fname);
    while ( not file.eof()) {
        matrix *A = new matrix(shape, shape);
        for (size_t i = 0; i < shape; i++)
            for (size_t j = 0; j < shape; j++)
                file >> A->data[i * shape + j];


        auto id = all_matrix.size();
        //all_matrix_lock.lock();
        all_matrix.push_back(A);
        //all_matrix_lock.unlock();
        for (size_t i = 0; i < id; i++) {
            tasks.push(std::pair<int, int>(i, id));
            tasks.push(std::pair<int, int>(id, i));
        }


    }
    file.close();
    tasks.done = true;


    for (size_t i =0; i<thread_num; i++)
       thrs[i].join();


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
