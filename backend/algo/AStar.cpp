#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <chrono>
#include <omp.h>

#include "../Maze.cpp"

class AStar
{
private:
    struct HashPair
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &p) const
        {
            auto hash1 = std::hash<T1>{}(p.first);
            auto hash2 = std::hash<T2>{}(p.second);

            // Combining the hashes to produce a unique hash for the pair
            return hash1 ^ hash2;
        }
    };

public:
    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair>>
    solve_serial(Maze &maze, std::pair<int, int> start, std::pair<int, int> end)
    {
        auto heuristic = [](std::pair<int, int> p1, std::pair<int, int> p2)
        {
            return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
        };

        std::vector<std::vector<int>> g_score(maze.getHeight(), std::vector<int>(maze.getWidth(), std::numeric_limits<int>::max()));
        g_score[start.first][start.second] = 0;

        std::vector<std::vector<int>> f_score(maze.getHeight(), std::vector<int>(maze.getWidth(), std::numeric_limits<int>::max()));
        f_score[start.first][start.second] = heuristic(start, end);

        std::priority_queue<std::tuple<int, int, std::pair<int, int>>,
                            std::vector<std::tuple<int, int, std::pair<int, int>>>,
                            std::greater<std::tuple<int, int, std::pair<int, int>>>>
            open;

        open.push(std::make_tuple(heuristic(start, end), heuristic(start, end), start));

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair> a_path;

        while (!open.empty())
        {
            auto current = std::get<2>(open.top());
            open.pop();

            if (current == end)
            {
                break;
            }

            auto neighbors = maze.getNeighbors(current);

            for (const auto &neighbor : neighbors)
            {
                int temp_g_score = g_score[current.first][current.second] + 1;
                int temp_f_score = temp_g_score + heuristic(neighbor, end);

                if (temp_f_score < f_score[neighbor.first][neighbor.second])
                {
                    g_score[neighbor.first][neighbor.second] = temp_g_score;
                    f_score[neighbor.first][neighbor.second] = temp_f_score;
                    open.push(std::make_tuple(temp_f_score, heuristic(neighbor, end), neighbor));
                    a_path[neighbor] = current;
                }
            }
        }

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair> fwd_path;
        auto cell = end;
        std::vector<std::pair<int, int>> path = {end};

        while (cell != start)
        {
            fwd_path[a_path[cell]] = cell;
            path.insert(path.begin(), a_path[cell]);
            cell = a_path[cell];
        }

        return std::make_pair(path, fwd_path);
    }

    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair>>
    solve_parallel(Maze &maze, std::pair<int, int> start, std::pair<int, int> end, int thread_count)
    {
        auto heuristic = [](std::pair<int, int> p1, std::pair<int, int> p2)
        {
            return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
        };

        std::vector<std::vector<int>> g_score(maze.getHeight(), std::vector<int>(maze.getWidth(), std::numeric_limits<int>::max()));
        g_score[start.first][start.second] = 0;

        std::vector<std::vector<int>> f_score(maze.getHeight(), std::vector<int>(maze.getWidth(), std::numeric_limits<int>::max()));
        f_score[start.first][start.second] = heuristic(start, end);

        std::priority_queue<std::tuple<int, int, std::pair<int, int>>,
                            std::vector<std::tuple<int, int, std::pair<int, int>>>,
                            std::greater<std::tuple<int, int, std::pair<int, int>>>>
            open;

        open.push(std::make_tuple(heuristic(start, end), heuristic(start, end), start));

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair> a_path;

        while (!open.empty())
        {
            auto current = std::get<2>(open.top());
            open.pop();

            if (current == end)
            {
                break;
            }

            auto neighbors = maze.getNeighbors(current);
            #pragma omp parallel for
            for (const auto &neighbor : neighbors)
            {
                int temp_g_score = g_score[current.first][current.second] + 1;
                int temp_f_score = temp_g_score + heuristic(neighbor, end);

                if (temp_f_score < f_score[neighbor.first][neighbor.second])
                {
                    g_score[neighbor.first][neighbor.second] = temp_g_score;
                    f_score[neighbor.first][neighbor.second] = temp_f_score;
                    open.push(std::make_tuple(temp_f_score, heuristic(neighbor, end), neighbor));
                    a_path[neighbor] = current;
                }
            }
        }

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPair> fwd_path;
        auto cell = end;
        std::vector<std::pair<int, int>> path = {end};

        while (cell != start)
        {
            fwd_path[a_path[cell]] = cell;
            path.insert(path.begin(), a_path[cell]);
            cell = a_path[cell];
        }

        return std::make_pair(path, fwd_path);
    }
};

int main()
{
    Maze myMaze({50, 50});
    std::pair<std::pair<int, int>, std::pair<int, int>> startEnd = myMaze.selectStartAndEnd();
    myMaze.print();

    AStar myAStar;

    auto start = std::chrono::high_resolution_clock::now();
    auto result = myAStar.solve_serial(myMaze, startEnd.first, startEnd.second);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Thời gian thực hiện tuần tự: " << duration.count() << " giây\n";

    // Print path
    std::cout << "Path: ";
    for (const auto &point : result.first)
    {
        std::cout << "(" << point.first << ", " << point.second << ") ";
    }
    std::cout << std::endl;

    // Print map
    std::cout << "Map: ";
    for (const auto &point : result.second)
    {
        std::cout << "[ (" << point.first.first << ", " << point.first.second << ") : (" << point.second.first << ", " << point.second.second << ") ]";
    }
    std::cout << std::endl;
    return 0;
}
