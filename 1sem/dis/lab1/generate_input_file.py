import numpy as np
import argparse

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("file_name", help="Input file name")
    parser.add_argument("--matrix_dim", help="matrix_dimention", default=4, type=int)
    parser.add_argument("--low", type=int, default=-100)
    parser.add_argument("--high", type=int, default=100)
    parser.add_argument("matrix_num", type=int, help="Number of matrices")
    return parser


if __name__ == "__main__":
    args = create_parser().parse_args()
    random_tensor = np.random.randint(args.low, args.high, (args.matrix_num, args.matrix_dim, args.matrix_dim)).astype(str)

    with open(args.file_name, "w") as f_out:
        for matrix in  random_tensor:
            f_out.write("\n".join(['\t'.join(s) for s in matrix]))
            f_out.write("\n")