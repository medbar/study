import numpy as np

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


