import numpy as np
from multiprocessing import JoinableQueue, Lock, Process, Queue


class matmul_process(Process):

    def __init__(self, in_queue, result_queue):
        Process.__init__(self)
        self.input_queue = in_queue
        self.result_queue = result_queue
        #self.output_matrix = output_matrix
        #self.lock_for_output = lock_for_output

    def run(self):
        while True:
            a, b = self.input_queue.get()
            c = np.matmul(a, b)
            self.result_queue.put(c)
            self.input_queue.task_done()
            #self.lock_for_output.acquire()
            #np.add(self.output_matrix, c, out=self.output_matrix)
            #self.lock_for_output.release()



class multiproc_calculator():
    def __init__(self, num_processes, shape=(4, 4)):
        self.matrix_list = []
        self.task_queue = JoinableQueue()
        self.result_queue = Queue()
        self.result = np.zeros(shape)
        self.processes = [matmul_process(self.task_queue, self.result_queue)
                        for i in range(num_processes)]


    def get_result(self):
        self.task_queue.join()
        self.task_queue.close()
        while not self.result_queue.empty():
            np.add(self.result, self.result_queue.get(), out=self.result)

        return self.result


    def start_proc(self):
        for proc in self.processes:
            proc.daemon = True
            proc.start()


    def add_new_matrix(self, matrix):
        # генерируем все возможные пары
        for a in self.matrix_list:
            self.task_queue.put((a, matrix))
            self.task_queue.put((matrix, a))
        self.matrix_list.append(matrix)


