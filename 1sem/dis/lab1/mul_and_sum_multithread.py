import numpy as np
import argparse
from ThreadManager import multithr_calculator
import time
from matrix_reader import read_matrix_from_file



def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("thread_number", type=int, help="Thread number for multiply matrix")
    parser.add_argument("file_name", help="Input file name")
    return parser


"""test_4000
[[ 41979177.  -1304005.   3119146. -12511269.]
 [-39292226.  20037734.   6642142.  -7115723.]
 [-46048231.   5812844.   8999342.  13145766.]
 [ -8234181.   9677525.   7167907. -11740145.]]
Total elapsed time:968.2676651477814
"""


if __name__ =="__main__":
    # main process
    parser = create_parser()
    args = parser.parse_args()

    t_start = time.time()
    thread_manager = multithr_calculator(args.thread_number)
    thread_manager.start_threads()
    for matrix in read_matrix_from_file(args.file_name):
        thread_manager.add_new_matrix(matrix)

    print(thread_manager.get_result())
    t_end = time.time()
    print("Total elapsed time:{}".format(t_end - t_start))
