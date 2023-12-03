#include "../algo/AStar.cpp"
#include "../algo/ACO.cpp"
#include "../Maze.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <list>
#include <fstream> 

#include <Windows.h>
#include <Psapi.h>
#include <functional>


class Compare_Algorithms {
    public:
        Maze maze;
        std::pair<int, int> start;
        std::pair<int, int> end;

        Compare_Algorithms() {}

        Compare_Algorithms(Maze &maze, std::pair<int, int> start, std::pair<int, int> end) {
            maze = maze;
            start = start;
            end = end;
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

        double getTimeAStarParallel() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_AStar = AStar().solve_parallel(maze, start, end);
            
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

        double getTimeACOParallel() {
            // Đo thời gian bắt đầu
            auto begin_time = std::chrono::high_resolution_clock::now();

            auto path_ACO = ACO().solve_parallel(maze, start, end);
            
            // Đo thời gian kết thúc
            auto end_time = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy
            std::chrono::duration<double> duration = end_time - begin_time;
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
            #error Unsupported operating system

        #endif

        double getMemoryAStarSerial() {

            auto path_AStar = AStar().solve_serial(maze, start, end);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }

        double getMemoryAStarParallel() {
    
            auto path_AStar = AStar().solve_parallel(maze, start, end);
            
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

        double getMemoryACOParallel() {

            auto path_ACO = ACO().solve_parallel(maze, start, end);
            
            // Đo bộ nhớ đã sử dụng sau khi gọi hàm
            double memUsed = getMemoryUsed();

            return memUsed;
        }
};

class Write_File: public Time_Algorithm, public Memory_Algorithm {
    public:
        Time_Algorithm time;
        Memory_Algorithm memory;

        Write_File() {}

        Write_File(Time_Algorithm t, Memory_Algorithm m) {
            time = t;
            memory = m;
        }

        void writeFileTime(std::string fileName) {
            std::ofstream file(fileName);

            int rows = 10; // Số hàng

            if (file.is_open()) {
                for (int i = 0; i < rows; i++) {
                    file << time.getTimeAStarSerial() * 1000 << " ";
                    file << time.getTimeAStarParallel() * 1000 << " ";
                    file << time.getTimeACOSerial() * 1000 << " ";
                    file << time.getTimeACOParallel() * 1000 << " ";
                    file << std::endl;
                }
                file.close();
                std::cout << "Data written to file successfully." << std::endl;
            } else {
                std::cout << "Unable to open file." << std::endl;
            }
        }

        void writeFileMemory(std::string fileName) {
            std::ofstream file(fileName);

            int rows = 10; // Số hàng
        
            if (file.is_open()) {
                // Gán giá trị cho mảng 2 chiều
                for (int i = 0; i < rows; i++) {
                    file << memory.getMemoryAStarSerial() << " ";
                    file << memory.getMemoryAStarParallel() << " ";
                    file << memory.getMemoryACOSerial() << " ";
                    file << memory.getMemoryACOParallel() << " ";
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

    Maze myMaze = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
    };

    // Maze myMaze({100, 100});

    // std::pair<int, int> start = {1, 1};
    // std::pair<int, int> end = {99, 99};

    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end = {9, 9};

    std::string fileTime = "../test/Report_Time.txt";
    std::string fileMemory = "../test/Report_Memory.txt";

    Compare_Algorithms compare(myMaze, start, end);
    Time_Algorithm time;
    time.maze = myMaze;
    time.start = start;
    time.end = end;
    
    // std::cout << time.getTimeAStarSerial() * 1000 << "ms" << "\n";
    // std::cout << time.getTimeAStarParallel() * 1000 << "ms" << "\n";
    // std::cout << time.getTimeACOSerial() * 1000 << "ms" << "\n";
    // std::cout << time.getTimeACOParallel() * 1000 << "ms" << std::endl;
    

    Memory_Algorithm memory;
    memory.maze = myMaze;
    memory.start = start;
    memory.end = end;

    // std::cout << memory.getMemoryAStarSerial() << "MB" << std::endl;
    // // std::cout << memory.getMemoryAStarParallel() << "MB" << std::endl;
    // std::cout << memory.getMemoryACOSerial() << "MB" << std::endl;
    // std::cout << memory.getMemoryACOParallel() << "MB" << std::endl;

    // Write_File write = Write_File(time, memory);
    // write.writeFileTime(fileTime);
    // write.writeFileMemory(fileMemory);

    return 0;
}