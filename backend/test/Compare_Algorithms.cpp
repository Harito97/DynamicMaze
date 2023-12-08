#include "../algo/AStar.cpp"
#include "../algo/ACO.cpp"
#include "../DynamicMaze.cpp"
#include "../Maze.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <list>
#include <fstream> 
#include <cmath>

#include <Windows.h>
#include <Psapi.h>
#include <functional>
#include <omp.h>

std::vector<std::vector<bool>> readDataFromFile(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::vector<bool>> data;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<bool> row;
            for (char c : line) {
                bool value = (c == '1');
                row.push_back(value);
            }
            data.push_back(row);
        }
        file.close();
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    return data;
}


class Compare_Algorithms {
    public:
        Maze maze;
        std::pair<int, int> start;
        std::pair<int, int> end;
        DynamicMaze dynamic;

        Compare_Algorithms() {}

        Compare_Algorithms(Maze &maze, std::pair<int, int> start, std::pair<int, int> end, DynamicMaze newDynamic) {
            maze = maze;
            start = start;
            end = end;
            dynamic = newDynamic;
        };
        
};

class Time_Algorithm: public Compare_Algorithms {
    public:
        Time_Algorithm() {}

        // double getTimeAlgorithmAStar(std::function<double()> functionCall) {
        //      // Đo thời gian bắt đầu
        //     auto begin_time = std::chrono::high_resolution_clock::now();

        //     auto path = functionCall();
            
        //     // Đo thời gian kết thúc
        //     auto end_time = std::chrono::high_resolution_clock::now();

        //     // Tính thời gian chạy
        //     std::chrono::duration<double> duration = end_time - begin_time;
        //     return duration.count();
        // }

        double getTimeAStarSerial() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_AStar = AStar().solve_serial(maze, start, end);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            return duration.count();

        }

        double getTimeAStarParallel(int num_threads) {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_AStar = AStar().solve_parallel(maze, start, end, num_threads);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            return duration.count();

        }

        double getTimeACOSerial() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_ACO = ACO().solve_serial(maze, start, end);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            return duration.count();

        }

        double getTimeACOParallel(int num_threads) {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_ACO = ACO().solve_parallel(maze, start, end, num_threads);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            return duration.count();

        }

        double getTimeDyAStarSerial() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            int numGotTarget = dynamic.getNumberGotTarget(dynamic, 0);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            std::ofstream file("../test/report_data/Report_NumGotTarget.txt", std::ios::app);
            file << numGotTarget << " ";
            return duration.count();

        }

        double getTimeDyAStarParallel(int num_threads) {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            int numGotTarget = dynamic.getNumberGotTarget(dynamic, 1);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            std::ofstream file("../test/report_data/Report_NumGotTarget.txt", std::ios::app);
            file << numGotTarget << " ";
            return duration.count();

        }

        double getTimeDyACOSerial() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            int numGotTarget = dynamic.getNumberGotTarget(dynamic, 2);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            std::ofstream file("../test/report_data/Report_NumGotTarget.txt", std::ios::app);
            file << numGotTarget << " ";
            return duration.count();

        }

        double getTimeDyACOParallel(int num_threads) {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            int numGotTarget = dynamic.getNumberGotTarget(dynamic, 3);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
            std::ofstream file("../test/report_data/Report_NumGotTarget.txt", std::ios::app);
            file << numGotTarget << std::endl;
            return duration.count();

        }

};

class Memory_Algorithm: public Compare_Algorithms {
    public:

        Memory_Algorithm() {}

        #if defined(__linux__)
            // Linux
            #include <sys/sysinfo.h>

            double getMemoryUsed() {
                struct sysinfo sysInfo;
                sysinfo(&sysInfo);
                double memUsed = static_cast<double>(sysInfo.totalram - sysInfo.freeram) * sysInfo.mem_unit / (1024 * 1024);
                return memUsed;
            }

        #elif defined(_WIN32) || defined(_WIN64)
            // Windows
            #include <Windows.h>

            double getMemoryUsed() {
                HANDLE process = GetCurrentProcess();
                PROCESS_MEMORY_COUNTERS_EX pmc;
                if (GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
                    SIZE_T usedRAM = pmc.WorkingSetSize;
                    double usedRAMInMB = static_cast<double>(usedRAM) / (1024 * 1024);
                    return usedRAMInMB;
                }
            }

           
        #elif defined(__APPLE__)
            // macOS
            #include <mach/mach.h>

            double getMemoryUsed() {
                struct task_basic_info_64 info;
                mach_msg_type_number_t size = sizeof(info);
                task_info(mach_task_self(), TASK_BASIC_INFO_64, (task_info_t)&info, &size);
                double memUsed = static_cast<double>(info.resident_size) / (1024 * 1024);
                return memUsed;
            }

        #else
            #error Unsupported operating syste
        #endif

        double getMemoryAStarSerial() {

            auto path_AStar = AStar().solve_serial(maze, start, end);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryAStarParallel(int num_threads) {
    
            auto path_AStar = AStar().solve_parallel(maze, start, end, num_threads);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryACOSerial() {

            auto path_ACO = ACO().solve_serial(maze, start, end);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryACOParallel(int num_threads) {

            auto path_ACO = ACO().solve_parallel(maze, start, end, num_threads);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryDyAStarSerial() {

            dynamic.getNumberGotTarget(dynamic, 0);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryDyAStarParallel(int num_threads) {
    
            dynamic.getNumberGotTarget(dynamic, 1);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryDyACOSerial() {

            dynamic.getNumberGotTarget(dynamic, 2);

            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryDyACOParallel(int num_threads) {

            dynamic.getNumberGotTarget(dynamic, 3);

            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }
};

class Intersection: public Time_Algorithm {
    public:
        Time_Algorithm time;
        std::vector<Maze> listFileName;

        Intersection() {}

        Intersection(Time_Algorithm newTime, std::vector<Maze> listFile) {
            time = newTime;
            listFileName = listFile;
        }

        void writeFileTimeStatic(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            if (file.is_open()) {
               
                file << time.getTimeAStarSerial() * 1000 << " ";
                file << time.getTimeAStarParallel(1) * 1000 << " ";
                file << time.getTimeACOSerial() * 1000 << " ";
                file << time.getTimeACOParallel(1) * 1000 << " ";
                file << std::endl;
            
                file.close();

                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileTimeDynamic(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);
            if (file.is_open()) {
               
                file << time.getTimeDyAStarSerial() * 1000 << " ";
                file << time.getTimeDyAStarParallel(1) * 1000 << " ";
                file << time.getTimeDyACOSerial() * 1000 << " ";
                file << time.getTimeDyACOParallel(1) * 1000 << " ";
                file << std::endl;
            
                file.close();

                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void getIntersection(std::string fileName, std::string fileNameDy) {
            for (const auto &maze : listFileName) {
                Maze myMaze = maze;
                std::pair<int, int> start = {0,0};
                std::pair<int, int> end = {myMaze.getWidth()-1, myMaze.getWidth()-1};
                DynamicMaze dynamic(myMaze, 5);
                dynamic.startEnd = {start, end};
           
                time.maze = myMaze;
                time.start = start;
                time.end = end;
                time.dynamic = dynamic;

                writeFileTimeStatic(fileName);
                // writeFileTimeDynamic(fileNameDy);
            }
        }
};


class Write_File: public Time_Algorithm, public Memory_Algorithm {
    public:
        Time_Algorithm time;
        Memory_Algorithm memory;

        Write_File() {}

        Write_File(Time_Algorithm newTime, Memory_Algorithm newMemory) {
            time = newTime;
            memory = newMemory;
        }

        void writeFileTime(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            int rows = 20; // Số hàng

            if (file.is_open()) {
                for (int i = 0; i < rows; i++) {
                    file << time.getTimeAStarSerial() * 1000 << " ";
                    file << time.getTimeAStarParallel(1) * 1000 << " ";
                    file << time.getTimeACOSerial() * 1000 << " ";
                    file << time.getTimeACOParallel(1) * 1000 << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileTimeDy(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            int rows = 20; // Số hàng

            if (file.is_open()) {
                for (int i = 0; i < rows; i++) {
                    file << time.getTimeDyAStarSerial() * 1000 << " ";
                    file << time.getTimeDyAStarParallel(1) * 1000 << " ";
                    file << time.getTimeDyACOSerial() * 1000 << " ";
                    file << time.getTimeDyACOParallel(1) * 1000 << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileMemory(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            int rows = 20; // Số hàng
        
            if (file.is_open()) {
                // Gán giá trị cho mảng 2 chiều
                for (int i = 0; i < rows; i++) {
                    file << memory.getMemoryAStarSerial() << " ";
                    file << memory.getMemoryAStarParallel(1) << " ";
                    file << memory.getMemoryACOSerial() << " ";
                    file << memory.getMemoryACOParallel(1) << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileMemoryDy(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            int rows = 20; // Số hàng
        
            if (file.is_open()) {
                // Gán giá trị cho mảng 2 chiều
                for (int i = 0; i < rows; i++) {
                    file << memory.getMemoryDyAStarSerial() << " ";
                    file << memory.getMemoryDyAStarParallel(1) << " ";
                    file << memory.getMemoryDyACOSerial() << " ";
                    file << memory.getMemoryDyACOParallel(1) << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileProcessThreads(std::string fileName) {
            std::ofstream file(fileName, std::ios::app);

            if (file.is_open()) {
                for (int i = 1; i < 9; i++) {
                    file << time.getTimeAStarParallel(pow(2,i)) * 1000 << " ";
                    file << time.getTimeACOParallel(pow(2,i)) * 1000 << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        } 
};


int main() {

    Maze myMaze_10_10 = readDataFromFile("../test/maze_test/maze_10_10.txt");
    Maze myMaze_15_15 = readDataFromFile("../test/maze_test/maze_15_15.txt");
    Maze myMaze_20_20 = readDataFromFile("../test/maze_test/maze_20_20.txt");
    Maze myMaze_25_25 = readDataFromFile("../test/maze_test/maze_25_25.txt");
    Maze myMaze_50_50 = readDataFromFile("../test/maze_test/maze_50_50.txt");
    Maze myMaze_75_75 = readDataFromFile("../test/maze_test/maze_75_75.txt");
    Maze myMaze_100_100 = readDataFromFile("../test/maze_test/maze_100_100.txt");
    Maze myMaze_125_125 = readDataFromFile("../test/maze_test/maze_125_125.txt");
    Maze myMaze_150_150 = readDataFromFile("../test/maze_test/maze_150_150.txt");
    Maze myMaze_175_175 = readDataFromFile("../test/maze_test/maze_175_175.txt");
    Maze myMaze_200_200 = readDataFromFile("../test/maze_test/maze_200_200.txt");

    // Maze myMaze({25, 25});

    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end = {24, 24};

    std::string fileTime = "../test/report_data/Report_Time.txt";
    std::string fileTimeDy = "../test/report_data/Report_Time_Dynamic.txt";
    std::string fileMemory = "../test/report_data/Report_Memory.txt";
    std::string fileMemoryDy = "../test/report_data/Report_Memory_Dynamic.txt";
    std::string fileThreads = "../test/report_data/Report_Threads.txt";

    std::vector<std::vector<bool>> maze_base(25, std::vector<bool>(25, false));

    DynamicMaze dynamic(maze_base, 5);
    dynamic.startEnd = {start, end};

    Compare_Algorithms compare(myMaze_25_25, start, end, dynamic);

    Time_Algorithm time;
    time.maze = maze_base;
    time.start = start;
    time.end = end;
    time.dynamic = dynamic;

    Memory_Algorithm memory;
    memory.maze = myMaze_25_25;
    memory.start = start;
    memory.end = end;
    memory.dynamic = dynamic;

    // std::cout << time.getTimeAStarSerial() * 1000 << "ms" << "\n";
    // std::cout << time.getTimeAStarParallel(1) * 1000 << "ms" << "\n";
    // std::cout << time.getTimeACOSerial() * 1000 << "ms" << "\n";
    // std::cout << time.getTimeACOParallel(1) * 1000 << "ms" << std::endl;

    // std::cout << memory.getMemoryAStarSerial() << "MB" << std::endl;
    // std::cout << memory.getMemoryAStarParallel(1) << "MB" << std::endl;
    // std::cout << memory.getMemoryACOSerial() << "MB" << std::endl;
    // std::cout << memory.getMemoryACOParallel(1) << "MB" << std::endl;

    Write_File write = Write_File(time, memory);
    // write.writeFileTime(fileTime);
    // write.writeFileTimeDy(fileTimeDy);
    // write.writeFileMemory(fileMemory);
    // write.writeFileMemoryDy(fileMemoryDy);
    // write.writeFileProcessThreads(fileThreads);

    std::vector<Maze> listFileName = {myMaze_10_10, myMaze_15_15, myMaze_20_20, myMaze_25_25, myMaze_50_50, myMaze_75_75, myMaze_100_100};
    Intersection intersection = Intersection(time, listFileName);

    intersection.getIntersection("../test/report_data/Report_Time_Mazes.txt", "../test/report_data/Report_Time_MazeDy.txt");


    return 0;
}