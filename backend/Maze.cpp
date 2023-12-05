#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Maze
{
private:
    int rows;
    int cols;
    std::vector<std::vector<bool>> maze;

    void generate(double obstacle_prob)
    {
        maze.resize(rows, std::vector<bool>(cols, false));

        // Seed bộ sinh số ngẫu nhiên dựa trên thời gian
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                double random_value = static_cast<double>(std::rand()) / RAND_MAX;
                if (random_value < obstacle_prob)
                {
                    maze[i][j] = true;
                }
            }
        }
    }

public:
    Maze() {} // default constructor - don't delete or Main.cpp will crash

    Maze(std::pair<int, int> shape, double obstacle_prob = 0.1)
    {
        rows = shape.first;
        cols = shape.second;
        generate(obstacle_prob);
    }

    Maze(std::initializer_list<std::vector<bool>> values)
    {
        for (std::vector<bool> row : values)
        {
            maze.push_back(row);
        }
        rows = maze.size();
        cols = maze.at(0).size();
    }

    Maze readMatrixFromFile(const std::string &filename)
    {
        std::vector<std::vector<bool>> matrix;
        std::ifstream file(filename);

        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                std::vector<bool> row;
                for (char c : line)
                {
                    if (c - '0' == 0)
                    {
                        row.push_back(false);
                    }
                    else
                    {
                        row.push_back(true);
                    }
                }
                matrix.push_back(row);
            }
            file.close();
        }
        else
        {
            std::cerr << "Cannot open file" << std::endl;
        }

        return Maze(matrix);
    }

    Maze(std::vector<std::vector<bool>> maze)
    {
        this->maze = maze;
        rows = maze.size();
        cols = maze.at(0).size();
    }

    void config(std::pair<int, int> shape, double obstacle_prob = 0.3)
    {
        rows = shape.first;
        cols = shape.second;
        generate(obstacle_prob);
    }

    int getHeight()
    {
        return rows;
    }

    int getWidth()
    {
        return cols;
    }

    void setValue(std::pair<int, int> pos, bool value)
    {
        maze[pos.first][pos.second] = value;
    }

    // Hàm chọn ngẫu nhiên hai ô có giá trị false làm điểm bắt đầu và điểm kết thúc
    // Chọn điểm bắt đầu và kết thúc sau khi tạo maze mà không phải trước đó
    // vì để bám sát các vấn đề thực tiễn ứng dụng là map có trước
    std::pair<std::pair<int, int>, std::pair<int, int>> selectStartAndEnd()
    {
        int m = maze.size();
        int n = maze[0].size();

        std::pair<int, int> start, end;

        // Lặp để chọn ngẫu nhiên hai ô false
        while (true)
        {
            int x1 = std::rand() % m;
            int y1 = std::rand() % n;
            int x2 = std::rand() % m;
            int y2 = std::rand() % n;

            if (!maze[x1][y1] && !maze[x2][y2] && (x1 != x2 || y1 != y2))
            {
                start = std::make_pair(x1, y1);
                end = std::make_pair(x2, y2);
                break;
            }
        }

        return std::make_pair(start, end);
    }

    void changeMaze(double prob_false_to_true, double prob_true_to_false)
    {
        // int is_dead_maze = 0;
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                double random_value = (double)rand() / RAND_MAX;
                if (!maze[i][j])
                {
                    if (random_value < prob_false_to_true)
                    {
                        maze[i][j] = true;
                    }
                    // else
                    // {
                    //     is_dead_maze += 1;
                    // }
                }
                else if (maze[i][j] && random_value < prob_true_to_false)
                {
                    maze[i][j] = false;
                    // is_dead_maze += 1;
                }
            }
        }
        // return is_dead_maze == 0;
    }

    Maze copy()
    {
        return Maze(maze);
    }

    bool isValid(std::pair<int, int> current)
    {
        return current.first >= 0 && current.second >= 0 &&
               current.first < rows && current.second < cols &&
               !maze[current.first][current.second];
    }

    std::vector<std::pair<int, int>> getNeighbors(std::pair<int, int> current)
    {
        std::vector<std::pair<int, int>> neighbors;
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (const auto &move : directions)
        {
            std::pair<int, int> neighbor = {current.first + move.first, current.second + move.second};
            if (isValid(neighbor))
            {
                neighbors.push_back(neighbor);
            }
        }
        return neighbors;
    }

    void print()
    {
        for (const auto &row : maze)
        {
            for (bool cell : row)
            {
                std::cout << (cell ? "1" : "0");
            }
            std::cout << std::endl;
        }
    }
};

// int main()
// {
//     Maze myMaze = Maze({5, 5});
//     // Maze myMaze = {{false, false, true}, {false, true, false}, {false, false, false}};
//     std::cout << "The origen maze..." << std::endl;
//     myMaze.print();

//     std::cout << "The copy maze: " << std::endl;
//     Maze copyMaze = myMaze.copy();
//     copyMaze.print();

//     std::cout << "Changing origen maze..." << std::endl;
//     myMaze.changeMaze(0.5, 0.0);
//     myMaze.print();

//     std::cout << "The copy maze: " << std::endl;
//     copyMaze.print();
//     return 0;
// }
