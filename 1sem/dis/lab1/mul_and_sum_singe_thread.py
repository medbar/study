import argparse
import time
import numpy as np
from matrix_reader import read_matrix_from_file


def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("file_name", help="Input file name")
    return parser


"""test_4000
[[ 41979177.  -1304005.   3119146. -12511269.]
 [-39292226.  20037734.   6642142.  -7115723.]
 [-46048231.   5812844.   8999342.  13145766.]
 [ -8234181.   9677525.   7167907. -11740145.]]
Total elapsed time:99.82200026512146
"""


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