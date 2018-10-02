import numpy as np
import threading
from queue import Queue


class matmul_thread(threading.Thread):

    def __init__(self, in_queue, output_matrix, lock_for_output):
        threading.Thread.__init__(self)
        self.input_queue = in_queue
        self.output_matrix = output_matrix
        self.lock_for_output = lock_for_output

    def run(self):
        while True:
            a, b = self.input_queue.get()
            c = np.matmul(a, b)

            self.lock_for_output.acquire()
            np.add(self.output_matrix, c, out=self.output_matrix)
            self.lock_for_output.release()

            self.input_queue.task_done()


class multithr_calculator():
    def __init__(self, num_threads, shape=(4, 4)):
        self.matrix_list = []
        self.task_queue = Queue()
        self.lock_for_output = threading.Lock()
        self.result = np.zeros(shape)
        self.threads = [matmul_thread(self.task_queue, self.result, self.lock_for_output)
                        for i in range(num_threads)]


    def get_result(self):
        self.task_queue.join()
        return self.result


    def start_threads(self):
        for thr in self.threads:
            thr.setDaemon(True)
            thr.start()

    def add_new_matrix(self, matrix):
        # генерируем все возможные пары
        for a in self.matrix_list:
            self.task_queue.put((a, matrix))
            self.task_queue.put((matrix, a))
        self.matrix_list.append(matrix)