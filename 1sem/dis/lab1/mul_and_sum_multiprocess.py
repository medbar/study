import numpy as np
import argparse

import time
from matrix_reader import read_matrix_from_file
from ProcessManager import multiproc_calculator



def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("thread_number", type=int, help="Thread number for multiply matrix")
    parser.add_argument("file_name", help="Input file name")
    return parser


"""
[[ 70.  70.  70.  70.]
 [ 90.  90.  90.  90.]
 [110. 110. 110. 110.]
 [130. 130. 130. 130.]]
Total elapsed time:0.014723062515258789
"""

if __name__ =="__main__":
    # main process
    parser = create_parser()
    args = parser.parse_args()

    t_start = time.time()
    process_manager = multiproc_calculator(4)
    process_manager.start_proc()
    for matrix in read_matrix_from_file(args.file_name):
        process_manager.add_new_matrix(matrix)

    print(process_manager.get_result())
    t_end = time.time()
    print("Total elapsed time:{}".format(t_end - t_start))
