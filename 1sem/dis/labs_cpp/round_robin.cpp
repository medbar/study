
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

namespace Round_Robin {

class block_write_queue{
    std::queue<std::pair<int,int>> data;
    std::mutex w_lock;
    int counter=0;

public:
    bool done = false;
    bool have_tasks = false;
    block_write_queue() = default;
    std::pair<int,int>* pop(){


        std::pair<int,int> *e = new std::pair<int,int>(data.front());
        w_lock.lock();
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
        counter++;
    }
    int get_processed_count(){
        return counter;
    }
};


void calculator(block_write_queue * q, std::vector<matrix*> *data_ptr, matrix * out_matrix, int shape, int thr_id) {
    std::vector<matrix*>& data =  *data_ptr;
    std::chrono::duration<double> helpfull_time;
    std::chrono::duration<double> useless_time;
    std::cout << "Thread "+ std::to_string(thr_id) + " started.\n";
    auto hard_start_time = std::chrono::system_clock::now();
    while (true){
        if (q->have_tasks) {
            std::pair<int, int> *task = q->pop();
            if (not task)
                continue;
            auto start = std::chrono::system_clock::now();
            matrix result_matrix = matrix(shape, shape);
            matrix *xm = data[task->first];
            matrix *ym = data[task->second];
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

            for (size_t i = 0; i < shape; i++)
                for (size_t j = 0; j < shape; j++)
                    out_matrix->data[i * shape + j] += result_matrix.data[i * shape + j];

        helpfull_time += (std::chrono::system_clock::now() - start);
        } else if (q->done){
            std::chrono::duration<double> thread_live_time = std::chrono::system_clock::now()-hard_start_time;
            std::chrono::duration<double> useless_time = thread_live_time - helpfull_time;
            std::cout << "Thread "+ std::to_string(thr_id) + " done.\nwork time "
            + std::to_string(helpfull_time.count()) + "sec. ("
            + std::to_string(helpfull_time.count()/(thread_live_time.count()) * 100) + "%)\n"
            + "useless time: " + std::to_string(useless_time.count()) + " sec. ("
            + std::to_string(useless_time.count()/(thread_live_time.count()) * 100) + "%)\n";
            return;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}


void round_robin(int shape, int thread_num, std::string fname, matrix &out_matrix){
    std::vector<matrix*> all_matrix;

    std::vector<std::thread> thrs;
    std::vector<block_write_queue*> tasks_queues;
    std::vector<matrix*> out_matrix_for_thrs;
    for (size_t i =0; i<thread_num; i++){
        block_write_queue *tasks = new block_write_queue;
        matrix *out_m = new matrix(shape, shape);
        tasks_queues.push_back(tasks);
        out_matrix_for_thrs.push_back(out_m);
        thrs.push_back(std::thread(calculator, tasks, &all_matrix, out_m, shape, i));
    }

    std::ifstream file(fname);
    int cur_process_id=0;
    while ( not file.eof()) {
        matrix *A = new matrix(shape, shape);
        for (size_t i = 0; i < shape; i++)
            for (size_t j = 0; j < shape; j++)
                file >> A->data[i * shape + j];

        auto matrix_id = all_matrix.size();
        all_matrix.push_back(A);
        for (size_t i = 0; i < matrix_id; i++) {
            tasks_queues[cur_process_id]->push(std::pair<int, int>(i, matrix_id));
            cur_process_id = (cur_process_id + 1) % thread_num;
            tasks_queues[cur_process_id]->push(std::pair<int, int>(matrix_id, i));
            cur_process_id = (cur_process_id + 1) % thread_num;
        }     
    }
    file.close();
    for ( size_t i = 0 ;i <thread_num ; i++)
        tasks_queues[i]->done = true;

    for (size_t i =0; i<thread_num; i++) {
        thrs[i].join();
        for (size_t k = 0; k < shape; k++)
            for (size_t l = 0; l < shape; l++)
                out_matrix.data[k * shape + l] += out_matrix_for_thrs[i]->data[k * shape + l];
    }
    std::cout << "TASKS:" <<std::endl;
    for (size_t i =0; i < thread_num; i++)
        std::cout << "Thread " << i << " processed " <<tasks_queues[i]->get_processed_count() << std::endl;
}

}
