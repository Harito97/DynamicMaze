#include "../../src/serial/AStar.cpp"
#include "../../src/serial/ACO.cpp"
#include "../../src/stmaze/maze.cpp"

void printResult(std::vector<std::pair<int, int>> result) {
    for (const auto& point : result)
    {
        std::cout << "(" << point.first << ", " << point.second << ") ";
    }
}

int main()
{
    int m = 5;                        // Số hàng
    int n = 10;                       // Số cột
    double obstacleProbability = 0.3; // Xác suất ô không thể đi qua

    std::vector<std::vector<bool>> maze = createMaze(m, n, obstacleProbability);

    std::pair<std::pair<int, int>, std::pair<int, int>> startAndEnd = selectStartAndEnd(maze);
    std::pair<int, int> start = startAndEnd.first;
    std::pair<int, int> end = startAndEnd.second;

    std::cout << "Mê cung được tạo ra như sau:" << std::endl;
    printMaze(maze);

    std::cout << "Điểm bắt đầu: (" << start.first << ", " << start.second << ")" << std::endl;
    std::cout << "Điểm kết thúc: (" << end.first << ", " << end.second << ")" << std::endl;

    std::cout <<"Change The Matrix Of The Maze\n";
    changeMaze(maze, 0.1, 0);
    printMaze(maze);

    startAndEnd = selectStartAndEnd(maze);
    start = startAndEnd.first;
    end = startAndEnd.second;
    std::cout << "Điểm bắt đầu: (" << start.first << ", " << start.second << ")" << std::endl;
    std::cout << "Điểm kết thúc: (" << end.first << ", " << end.second << ")" << std::endl;

    std::vector<std::pair<int, int>> path_AStar = AStar(maze, start, end);
    std::vector<std::pair<int, int>> path_ACO = ACO(maze, start, end);

    // std::cout << "Result of the Astar algorithm: ";
    // printResult(path_AStar);

    
    // std::cout << "Result of the ACO algorithm: ";
    // printResult(path_ACO);

    return 0;
}