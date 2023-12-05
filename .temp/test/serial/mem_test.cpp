// #include "../../src/serial/AStar.cpp"
// #include "../../src/serial/ACO.cpp"
// #include "../../src/maze/maze.cpp"

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <chrono>

// #include <Windows.h>
// #include <Psapi.h>
// #include <list>
// #include <fstream> 

// void resultAstar(std::vector<std::vector<bool>> &maze, int m, int n, std::pair<int, int> start, std::pair<int, int> end) {

//     std::vector<std::pair<int, int>> path_Astar = AStar(maze, m, n, start, end);
    
//     // Lấy thông tin về bộ nhớ đã sử dụng
//     HANDLE process = GetCurrentProcess();
//     PROCESS_MEMORY_COUNTERS_EX pmc;
//     if (GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
//         SIZE_T usedRAM = pmc.WorkingSetSize;
//         std::cout << "Used RAM: " << usedRAM / (1024 * 1024) << " MB ";
//     }

// }

// void resultACO(std::vector<std::vector<bool>> &maze, int m, int n, std::pair<int, int> start, std::pair<int, int> end) {

//     std::vector<std::pair<int, int>> path_ACO = ACO(maze, m, n, start, end);

//    // Lấy thông tin về bộ nhớ đã sử dụng
//     HANDLE process = GetCurrentProcess();
//     PROCESS_MEMORY_COUNTERS_EX pmc;
//     if (GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
//         SIZE_T usedRAM = pmc.WorkingSetSize;
//         std::cout << "Used RAM: " << usedRAM / (1024 * 1024) << " MB" << std::endl;
//     }

// }

// void WriteListToFile(const std::vector<std::vector<double>>& data_compare, const std::string& filename) {
//     std::ofstream file(filename);
    
//     if (file.is_open()) {
//         for (const auto& row : data_compare) {
//             for (const auto& element : row) {
//                 file << element << " ";
//             }
//             file << std::endl;
//         }
        
//         file.close();
//         std::cout << "Data written to file successfully." << std::endl;
//     } else {
//         std::cout << "Unable to open file." << std::endl;
//     }
// }

// int main() {
//     std::vector<std::vector<double>> data_compare = {};
//     double ramAstar, ramACO;
//     int row_column[] = {100,500,1000,5000};
//     for (int i = 0; i < 4; i++) {
//         std::vector<std::vector<bool>> maze = createMaze(row_column[i], row_column[i], 0.3);
//         std::pair<std::pair<int, int>, std::pair<int, int>> startAndEnd = selectStartAndEnd(maze);
//         std::pair<int, int> start = startAndEnd.first;
//         std::pair<int, int> end = startAndEnd.second;

//         resultAstar(maze, row_column[i], row_column[i], start, end);
//         resultACO(maze, row_column[i], row_column[i], start, end);
//         std::cout << std::endl;

//         std::vector<double> arrays;

//         arrays.push_back(ramAstar);
//         arrays.push_back(ramACO);

//         data_compare.push_back(arrays);

//     }
//     std::string filename = "../test_main/compareAlgorithm/data_compare_ram.txt";
//     WriteListToFile(data_compare, filename);
   
//     return 0;
// }