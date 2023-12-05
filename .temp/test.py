# Author __Nguyen Van Thang__
# ---------------------------

# pip install memory_profiler
# pip install psutil

import sys, os
import psutil
import timeit
from memory_profiler import profile
import pandas as pd, plotly.express as px

# import DyMaze
from algo import ACO, AStar
from maze import Maze
from dymaze import DyMaze

class Compare_Algorithms():
    # When work with static maze (serial and parallel)
    def __init__(self, maze: Maze, start: tuple, end: tuple):
        self.maze = maze
        self.start = start
        self.end = end

    def compare_time(self):
        list_time_astar = timeit.repeat(lambda: AStar().astar(self.maze, self.start, self.end), number=1, repeat=3)
        list_time_aco = timeit.repeat(lambda: ACO().aco(self.maze, self.start, self.end), number=1, repeat=3)
        _astar_aco = list(zip(list_time_astar, list_time_aco))
        return _astar_aco
    
    def convert_bytes_to_megabytes(self, bytes):
        megabytes = bytes / (1 << 20)
        return megabytes

    def get_process_memory(self):
        process = psutil.Process(os.getpid())
        memory_start = process.memory_info().rss
        self.compare_time()
        memory_end = process.memory_info().rss
        return self.convert_bytes_to_megabytes(memory_end - memory_start)
    
    @profile
    def memory_astar(self):
        path_astar = AStar().astar(self.maze, self.start, self.end)
        return path_astar

    @profile
    def memory_aco(self):
        path_aco = ACO().aco(self.maze, self.start, self.end)
        return path_aco

class Test(DyMaze):
    # When work with dynamic maze (serial and parallel)
    def __init__():
        pass

class Compare_Papers:
    # When work with static maze and dynamic maze
    def __init__():
        pass

class Visualization(Test):
    # Visualization the result from Test
    def __init__():
        pass

if __name__ == "__main__":
    # Sử dụng lớp Maze
    rows = 25 # Số hàng
    cols = 25 # Số cột
    obstacle_prob = 0.3  # Xác suất ô bị chặn

    maze = Maze((rows, cols), obstacle_prob).array([
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False],
            [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False]])
    

    compare = Compare_Algorithms(maze, (0,0), (24,24))
    time = compare.compare_time()
    print(time)
        
    print(compare.get_process_memory())
    print(compare.memory_astar())
    print(compare.memory_aco())

   

    


    