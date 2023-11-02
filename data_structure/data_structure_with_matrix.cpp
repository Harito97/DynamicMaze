// Cách biểu diễn 2: Sử dụng matrix giúp tối ưu tốc độ đọc / ghi
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Hàm tạo mê cung với kích thước m x n và xác suất các ô là "true" (không thể đi qua)
std::vector<std::vector<bool>> createMaze(int m, int n, double obstacleProbability)
{
    // Khởi tạo ma trận mê cung với giá trị ban đầu là "false" (có thể đi qua)
    // Khởi tạo mê cung. Quy ước:
    // false: 0 - không bị chặn
    // true: 1 - bị chặn
    // vì mê cung được chủ động thiết kế để có nhiều đường đi - tức ít bị chặn - tức nhiều ô giá trị 0
    // nhờ nhiều ô giá trị 0 - tức transistor ở trạng thái tắt nhiều hơn
    // -> Tiết kiệm điện hơn
    std::vector<std::vector<bool>> maze(m, std::vector<bool>(n, false));

    // Seed bộ sinh số ngẫu nhiên dựa trên thời gian
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Đặt các ô thành "true" (không thể đi qua) với xác suất obstacleProbability
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double randomValue = static_cast<double>(std::rand()) / RAND_MAX;
            if (randomValue < obstacleProbability)
            {
                maze[i][j] = true;
            }
        }
    }

    return maze;
}

// Hàm in mê cung
void printMaze(const std::vector<std::vector<bool>> &maze)
{
    for (const std::vector<bool> &row : maze)
    {
        for (bool cell : row)
        {
            if (cell)
            {
                std::cout << "*"; // Ô không thể đi qua
            }
            else
            {
                std::cout << " "; // Ô có thể đi qua
            }
        }
        std::cout << std::endl;
    }
}

// Hàm chọn ngẫu nhiên hai ô có giá trị false làm điểm bắt đầu và điểm kết thúc
// Chọn điểm bắt đầu và kết thúc sau khi tạo maze mà không phải trước đó 
// vì để bám sát các vấn đề thực tiễn ứng dụng là map có trước khi ta chọn điểm bắt đầu và kết thúc 
std::pair<std::pair<int, int>, std::pair<int, int>> selectStartAndEnd(const std::vector<std::vector<bool>> &maze)
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

int main()
{
    int m = 5;                        // Số hàng
    int n = 5;                        // Số cột
    double obstacleProbability = 0.3; // Xác suất ô không thể đi qua

    std::vector<std::vector<bool>> maze = createMaze(m, n, obstacleProbability);

    std::pair<std::pair<int, int>, std::pair<int, int>> startAndEnd = selectStartAndEnd(maze);
    std::pair<int, int> start = startAndEnd.first;
    std::pair<int, int> end = startAndEnd.second;

    std::cout << "Mê cung được tạo ra như sau:" << std::endl;
    printMaze(maze);

    std::cout << "Điểm bắt đầu: (" << start.first << ", " << start.second << ")" << std::endl;
    std::cout << "Điểm kết thúc: (" << end.first << ", " << end.second << ")" << std::endl;

    return 0;
}