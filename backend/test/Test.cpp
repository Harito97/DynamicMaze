#include "../algo/ACO.cpp"
#include "../algo/AStar.cpp"
#include "../Maze.cpp"
#include <fstream>
#include <string>
#include "tbb/concurrent_unordered_map.h"

std::vector<std::vector<bool>> readMatrixFromFile(const std::string &filename)
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
                // Chuyển ký tự thành số và thêm vào std::vector
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
        std::cerr << "Không thể mở file" << std::endl;
    }

    return matrix;
}

int main()
{
    std::vector<std::vector<bool>> matrix = readMatrixFromFile("maze.txt");
    Maze myMaze(matrix);
    // myMaze.print();
    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end = {199, 199};
    AStar myAStar;
    ACO myACO(3, 30);

    auto start1 = std::chrono::high_resolution_clock::now();
    auto result1 = myACO.solve_serial(myMaze, start, end);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;
    std::cout << "Thời gian thực hiện tuần tự: " << duration1.count() << " giây\n";

    // Print path
    std::cout << result1.first.size() << std::endl;
    std::cout << "Path: ";
    for (const auto &point : result1.first)
    {
        std::cout << "(" << point.first << ", " << point.second << ") ";
    }
    std::cout << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    auto result2 = myACO.solve_parallel(myMaze, start, end);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;
    std::cout << "Thời gian thực hiện song song: " << duration2.count() << " giây\n";

    // Print path
    std::cout << "Path: ";
    for (const auto &point : result2.first)
    {
        std::cout << "(" << point.first << ", " << point.second << ") ";
    }
    std::cout << std::endl;

    std::cout << "Speed Up: " << duration1.count() / duration2.count() << " lần" << std::endl;

    return 0;
}
