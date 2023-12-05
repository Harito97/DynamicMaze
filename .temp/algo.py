# Author __Luong Duc Anh__
# Author __Tran Minh Duc__
# ------------------------
from maze import Maze
import random
import math
import numpy as np
import timeit
from queue import PriorityQueue
from collections import defaultdict

class AStar():
    def astar(self, maze: Maze, start: tuple, end: tuple):
        def heuristic(p1, p2): return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])
        g_score = np.full((maze.rows, maze.cols), np.iinfo(np.int32).max, dtype=np.int32)
        g_score[start] = 0
        f_score = np.full((maze.rows, maze.cols), np.iinfo(np.int32).max, dtype=np.int32)
        f_score[start] = heuristic(start, end)
        
        open = PriorityQueue()
        open.put((heuristic(start, end), heuristic(start, end), start))
        a_path = {}
        
        while not open.empty():
            current = open.get()[2]
            if current == end:
                break
            neighbors = maze.get_neighbors(current)
            for neighbor in neighbors:
                temp_g_score = g_score[current] + 1
                temp_f_score = temp_g_score + heuristic(neighbor, end)
                
                if temp_f_score < f_score[neighbor]:
                    g_score[neighbor] = temp_g_score
                    f_score[neighbor] = temp_f_score
                    open.put((temp_f_score, heuristic(neighbor, end), neighbor))
                    a_path[neighbor] = current
        fwd_path = {}
        cell = end
        path = [end]
        while cell != start:
            fwd_path[a_path[cell]] = cell
            path.insert(0, a_path[cell])
            cell = a_path[cell]
        return path, fwd_path

def test_AStar():
    # Sử dụng lớp Maze
    rows = 10 # Số hàng
    cols = 10 # Số cột
    obstacle_prob = 0.3  # Xác suất ô bị chặn

    my_maze = Maze((rows, cols), obstacle_prob).array([
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False]])
    
    # print(my_maze)
    astar = AStar()
    print(astar.astar(my_maze, (0, 0), (5, 5))[0])


class ACO():
    def __init__(self,
                 num_ants=30,
                 iterations=30,
                 pheromone_const=1000.0,
                 evaporation_rate=0.4,
                 alpha=0.3):
        self.num_ants = num_ants
        self.iterations = iterations
        self.pheromone_const = pheromone_const
        self.evaporation_rate = evaporation_rate
        self.alpha = alpha

    def aco(self, maze: Maze, start: tuple, end: tuple):
        best_path = None
        best_path_length = float('inf')
        pheromone_map = defaultdict(lambda: 0.1)
        for _ in range(self.iterations):
            pheromone_delta = defaultdict(lambda: 0.1)
            for _ in range(self.num_ants):
                current = start
                path = [current]
                used_trails = set()
                while current != end:
                    neighbors = maze.get_neighbors(current)
                    weights = []
                    sum_pheromone = 0.0
                    for neighbor in neighbors:
                        sum_pheromone += pheromone_map[(
                            current, neighbor)]
                    for neighbor in neighbors:
                        weights.append(
                            math.pow(pheromone_map[(current, neighbor)], self.alpha)/sum_pheromone)
                    pre = current
                    current = random.choices(neighbors, weights)[0]
                    path.append(current)
                    used_trails.add((pre, current))
                if len(path) < best_path_length:
                    best_path = path
                    best_path_length = len(path)
                for trail in used_trails:
                    delta = self.pheromone_const / len(path)
                    pheromone_delta[trail] += delta
            for trail in pheromone_map.keys():
                pheromone_map[trail] *= (1 - self.evaporation_rate)
            for trail, value in pheromone_delta.items():
                pheromone_map[trail] += value
        return best_path, pheromone_map

def test_ACO():
    # Sử dụng lớp Maze
    rows = 10 # Số hàng
    cols = 10 # Số cột
    obstacle_prob = 0.3  # Xác suất ô bị chặn

    my_maze = Maze((rows, cols), obstacle_prob).array([
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False],
                    [False, False, False, False, False, False, False, False, False, False]])
    # print(my_maze)
    aco = ACO()
    print(aco.aco(my_maze, (0, 0), (5, 5))[0])


if __name__ == '__main__':
    test_AStar()
    test_ACO()