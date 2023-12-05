# Author __Pham Ngoc Hai - Harito__
# ---------------------------------
import numpy as np

class Maze:
    # Create a maze with rows, cols and obstacle_prob para or create a Maze with random maze value
    def __init__(self, shape: tuple, obstacle_prob=0.3, maze=None):
        self.rows = shape[0]
        self.cols = shape[1]
        self.maze = maze
        if maze is None:
            self.__generate__(obstacle_prob)

    def __generate__(self, obstacle_prob):
        random_values = np.random.rand(self.rows, self.cols)
        self.maze = np.full((self.rows, self.cols), False, dtype=bool)
        self.maze[random_values < obstacle_prob] = True

    @classmethod
    def array(cls, array):
        maze = np.array(array, dtype=bool)
        return cls(maze.shape, maze=maze)

    # Change maze follow by probability and check the updated maze can have path to go or not
    def change_maze(self, prob_false_to_true, prob_true_to_false):
        is_dead_maze = 0
        for i in range(self.rows):
            for j in range(self.cols):
                random_value = np.random.rand()
                if not self.maze[i, j]:
                    if random_value < prob_false_to_true:
                        self.maze[i, j] = True
                    else:
                        is_dead_maze += 1
                elif self.maze[i, j] and random_value < prob_true_to_false:
                    self.maze[i, j] = False
                    is_dead_maze += 1
        return is_dead_maze == 0

    # Return a clone maze (type: Maze)
    def copy(self):
        return Maze((self.rows, self.cols), maze=np.copy(self.maze))

    # Check the current position can go throw or not
    def is_valid(self, current: tuple):
        return current[0] >= 0 and current[1] >= 0 \
            and current[0] < self.rows \
            and current[1] < self.cols \
            and not self.maze[current[0], current[1]]

    # Return a list of neighbors can reach of the current position
    def get_neighbors(self, current: tuple):
        neighbors = []
        directions = ((-1, 0), (0, 1), (1, 0), (0, -1))
        for move in directions:
            neighbor = (current[0] + move[0], current[1] + move[1])
            if self.is_valid(neighbor):
                neighbors.append(neighbor)
        return neighbors

    # Return the String describe maze
    def __str__(self):
        return self.maze.__str__()
    
    # Print maze
    def print(self):
        print(self.__str__())

    # # Solve 
    # def solve(self, method: str, type: str):
    #     if method == 'AStar':
    #         if type == 'Path':
    #             return AStar.path(self.maze)
    #         elif type == 'Map':
    #             return AStar.map(self.maze)
    #         else:
    #             return Exception('Only support "Path" and "Map"')
    #     elif method == 'ACO':
    #         return 
    #     else:
    #         return Exception('Only support "AStar" and "ACO"')


def main():
    # Sử dụng lớp Maze
    rows = 5  # Số hàng
    cols = 5  # Số cột
    obstacle_prob = 0.3  # Xác suất ô bị chặn

    maze_instance = Maze((rows, cols), obstacle_prob)

    # Gọi các phương thức của lớp Maze
    print("Initial Maze with random value:")
    maze_instance.print()

    print("Maze with selected value: ")
    maze2 = Maze.array([[True, True, False],
                        [True, True, False],
                        [True, True, False],
                        [True, True, False],
                        [True, True, False]])
    maze2.print()

    is_dead_maze = maze_instance.change_maze(0.5, 0)
    print("Changed the Random Maze:")
    maze_instance.print()
    print("Is Dead Maze:", is_dead_maze)

    cloned_maze = maze_instance.copy()
    print("Cloned the Random Maze:")
    cloned_maze.print()

    is_dead_maze = cloned_maze.change_maze(0.2, 0)
    print("Change cloned Maze:")
    cloned_maze.print()
    print("Is Dead Maze:", is_dead_maze)

if __name__ == "__main__":
    main()
