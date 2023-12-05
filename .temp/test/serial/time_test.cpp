#include "../../src/serial/AStar.cpp"
#include "../../src/serial/ACO.cpp"
#include "../../src/stmaze/maze.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <list>
#include <fstream> 

double resultAStar(std::vector<std::vector<bool>> &maze, int m, int n, std::pair<int, int> start, std::pair<int, int> end) {
    // Đo thời gian bắt đầu
    auto begin_time = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int, int>> path_AStar = AStar(maze, start, end);
    
    // Đo thời gian kết thúc
    auto end_time = std::chrono::high_resolution_clock::now();

    // Tính thời gian chạy
    std::chrono::duration<double> duration = end_time - begin_time;
    return duration.count();

    // double timeInSeconds = duration.count();
    // return timeInSeconds;
}

double resultACO(std::vector<std::vector<bool>> &maze, int m, int n, std::pair<int, int> start, std::pair<int, int> end) {
    // Đo thời gian bắt đầu
    auto begin_time = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int, int>> path_ACO = ACO(maze, start, end);
    
    // Đo thời gian kết thúc
    auto end_time = std::chrono::high_resolution_clock::now();

    // Tính thời gian chạy
    std::chrono::duration<double> duration = end_time - begin_time;
    return duration.count();

    // double timeInSeconds = duration.count();
    // return timeInSeconds;
}

void WriteListToFile(const std::vector<std::vector<double>>& data_compare, const std::string& filename) {
    std::ofstream file(filename);
    
    if (file.is_open()) {
        for (const auto& row : data_compare) {
            for (const auto& element : row) {
                file << element << " ";
            }
            file << std::endl;
        }
        
        file.close();
        std::cout << "Data written to file successfully." << std::endl;
    } else {
        std::cout << "Unable to open file." << std::endl;
    }
}

int main()
{
    std::vector<std::vector<double>> data_compare = {};
    double timeAStar, timeACO;
    int row_column[] = {10, 20, 30, 40};
    // int row_column[] = {3, 5, 7, 9};
    for (int i = 0; i < 4; i++) {
        std::vector<std::vector<bool>> maze = createMaze(row_column[i], row_column[i], 0.3);
        std::pair<std::pair<int, int>, std::pair<int, int>> startAndEnd = selectStartAndEnd(maze);
        std::pair<int, int> start = startAndEnd.first;
        std::pair<int, int> end = startAndEnd.second;

        timeAStar = resultAStar(maze, row_column[i], row_column[i], start, end);
        timeACO = resultACO(maze, row_column[i], row_column[i], start, end);

        std::vector<double> arrays;
        arrays.push_back(timeAStar);
        arrays.push_back(timeACO);

        data_compare.push_back(arrays);
    }
    std::string filename = "../test/serial/compare/time.txt";
    WriteListToFile(data_compare, filename);

    return 0;
}