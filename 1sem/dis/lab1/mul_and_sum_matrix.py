import numpy as np
import argparse
from ThreadManager import multithr_calculator



def read_matrix_from_file(fname, shape=(4,4)):
    with open(fname, "r", encoding="utf-8") as f_in:
        matrix_list = []
        for line in f_in:
            line = line.strip()
            if not line:
                continue
            matrix_list.append(line.split())
            if len(matrix_list) == shape[0]:
                yield np.array(matrix_list, dtype=np.int64)
                matrix_list.clear()





def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("thread_number", type=int, help="Thread number for multiply matrix")
    parser.add_argument("file_name", help="Input file name")
    return parser

if __name__ =="__main__":
    # main process
    parser = create_parser()
    args = parser.parse_args()

    thread_manager = multithr_calculator(args.thread_number)
    thread_manager.start_threads()
    for matrix in read_matrix_from_file(args.file_name):
        thread_manager.add_new_matrix(matrix)

    print(thread_manager.get_result())