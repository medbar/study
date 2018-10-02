from mul_and_sum_matrix import read_matrix_from_file
import argparse
import time
import numpy as np

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("file_name", help="Input file name")
    return parser

if __name__ =="__main__":
    # main process
    parser = create_parser()
    args = parser.parse_args()

    shape = (4,4)
    t_start = time.time()

    matrix_list = []
    result = np.zeros(shape)
    for matrix in read_matrix_from_file(args.file_name):
        for a in matrix_list:
            c = np.matmul(a, matrix)
            np.add(result, c, out=result)
            c = np.matmul(matrix,a)
            np.add(result, c, out=result)
        matrix_list.append(matrix)


    print(result)
    t_end = time.time()
    print("Total elapsed time:{}".format(t_end - t_start))