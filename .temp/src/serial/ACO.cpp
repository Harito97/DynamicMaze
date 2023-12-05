#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <random>
#include <limits.h>
#include <cmath>
#include <chrono>

struct HashPair
{
    template <class T1, class T2, class T3, class T4>
    std::size_t operator()(const std::pair<std::pair<T1, T2>, std::pair<T3, T4>> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first.first);
        auto h2 = std::hash<T2>{}(p.first.second);
        auto h3 = std::hash<T3>{}(p.second.first);
        auto h4 = std::hash<T4>{}(p.second.second);

        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

const std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

bool isPath(std::pair<int, int> pos, std::vector<std::vector<bool>> &maze)
{
    return pos.first >= 0 &&
           pos.first < maze.size() &&
           pos.second >= 0 &&
           pos.second < maze.at(0).size() &&
           !maze.at(pos.first).at(pos.second);
}

std::vector<std::pair<int, int>> getNeighbors(std::pair<int, int> current, std::vector<std::vector<bool>> &maze)
{
    std::vector<std::pair<int, int>> neighbors;
    for (auto &move : directions)
    {
        std::pair<int, int> newPos = {current.first + move.first, current.second + move.second};
        if (isPath(newPos, maze))
        {
            neighbors.push_back(newPos);
        }
    }
    return neighbors;
}

// std::vector<std::pair<int, int>> ACO(std::vector<std::vector<bool>> &maze,
//                                      std::pair<int, int> start,
//                                      std::pair<int, int> end, int numAnts = 30,
//                                      double iterations = 30,
//                                      double pheromoneConst = 1000.0,
//                                      double evaporationRate = 0.3,
//                                      double alpha = 0.6)
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     // std::uniform_int_distribution<> randomDir(0, 3);
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneMap;
//     std::vector<std::pair<int, int>> bestPath;
//     int bestPathLength = INT_MAX;
//     for (int i = 0; i < iterations; ++i)
//     {
//         std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneDelta;
//         for (int ant = 0; ant < numAnts; ++ant)
//         {
//             std::pair<int, int> current = start;
//             int dir = 1;
//             std::vector<std::pair<int, int>> path = {current};
//             std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPair> usedTrails;
//             while (current.first != end.first || current.second != end.second)
//             {
//                 std::vector<std::pair<int, int>> neighbors = getNeighbors(current, maze);
//                 std::vector<double> weights;
//                 double sumPheromone = 0.0;
//                 for (auto &neighbor : neighbors)
//                 {
//                     // pheromoneMap.try_emplace(std::make_pair(current, neighbor), 0.1);
//                     // sumPheromone += pheromoneMap[std::make_pair(current, neighbor)];
//                     if (pheromoneMap.find(std::make_pair(current, neighbor)) != pheromoneMap.end())
//                     {
//                         sumPheromone += pheromoneMap[std::make_pair(current, neighbor)];
//                     }
//                     else
//                     {
//                         sumPheromone += 0.1;
//                     }
//                 }
//                 for (auto &neighbor : neighbors)
//                 {
//                     // weights.push_back(std::pow(pheromoneMap[std::make_pair(current, neighbor)], alpha) / sumPheromone);
//                     if (pheromoneMap.find(std::make_pair(current, neighbor)) != pheromoneMap.end())
//                     {
//                         weights.push_back(std::pow(pheromoneMap[std::make_pair(current, neighbor)], alpha) / sumPheromone);
//                     }
//                     else
//                     {
//                         weights.push_back(std::pow(0.1, alpha) / sumPheromone);
//                     }
//                 }
//                 // std::vector<int> dirs;
//                 // int behind = (dir + 2) % 4;
//                 // int left = (dir + 3) % 4;
//                 // int right = (dir + 1) % 4;
//                 // for (auto &neighbor : neighbors)
//                 // {
//                 //     std::pair<int, int> deltaPos = {neighbor.first - current.first, neighbor.second - current.second};
//                 //     double lossMultiplier;
//                 //     if (deltaPos.first == directions.at(dir).first && deltaPos.second == directions.at(dir).second)
//                 //     {
//                 //         lossMultiplier = 1;
//                 //         dirs.push_back(dir);
//                 //     }
//                 //     else if (deltaPos.first == directions.at(left).first && deltaPos.second == directions.at(left).second)
//                 //     {
//                 //         lossMultiplier = 1 - alpha;
//                 //         dirs.push_back(left);
//                 //     }
//                 //     else if (deltaPos.first == directions.at(right).first && deltaPos.second == directions.at(right).second)
//                 //     {
//                 //         lossMultiplier = 1 - alpha;
//                 //         dirs.push_back(right);
//                 //     }
//                 //     else
//                 //     {
//                 //         lossMultiplier = std::pow(1 - alpha, 2);
//                 //         dirs.push_back(behind);
//                 //     }
//                 //     weights.push_back(pheromoneMap[std::make_pair(current, neighbor)] * lossMultiplier);
//                 // }
//                 // std::cout << weights.size() << " " << neighbors.size() << std::endl;
//                 std::discrete_distribution<> dist(weights.begin(), weights.end());
//                 std::pair<int, int> pre = current;
//                 int choice = dist(gen);
//                 current = neighbors.at(choice);
//                 path.push_back(current);
//                 usedTrails.insert(std::make_pair(pre, current));
//                 // dir = dirs.at(choice);
//             }
//             if (path.size() < bestPathLength)
//             {
//                 bestPath = path;
//                 bestPathLength = path.size();
//             }
//             for (auto &trail : usedTrails)
//             {
//                 double delta = pheromoneConst / path.size();
//                 pheromoneDelta[trail] += delta;
//             }
//         }
//         for (auto &trail : pheromoneMap)
//         {
//             pheromoneMap[trail.first] *= (1 - evaporationRate);
//         }
//         for (auto &trail : pheromoneDelta)
//         {
//             pheromoneMap[trail.first] += trail.second;
//         }
//     }
//     return bestPath;
// }

// Return pheromone map of ACO algorithm to use for dynamic maze
std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneMap(std::vector<std::vector<bool>> &maze,
                                     std::pair<int, int> start,
                                     std::pair<int, int> end, 
                                     int numAnts = 30,
                                     double iterations = 50,
                                     double pheromoneConst = 1000.0,
                                     double evaporationRate = 0.3,
                                     double alpha = 0.6)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromone_map;
    std::vector<std::pair<int, int>> bestPath;
    int bestPathLength = INT_MAX;
    for (int i = 0; i < iterations; ++i)
    {
        std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneDelta;
        for (int ant = 0; ant < numAnts; ++ant)
        {
            std::pair<int, int> current = start;
            int dir = 1;
            std::vector<std::pair<int, int>> path = {current};
            std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPair> usedTrails;
            while (current.first != end.first || current.second != end.second)
            {
                std::vector<std::pair<int, int>> neighbors = getNeighbors(current, maze);
                std::vector<double> weights;
                double sumPheromone = 0.0;
                for (auto &neighbor : neighbors)
                {
                    if (pheromone_map.find(std::make_pair(current, neighbor)) != pheromone_map.end())
                    {
                        sumPheromone += pheromone_map[std::make_pair(current, neighbor)];
                    }
                    else
                    {
                        sumPheromone += 0.1;
                    }
                }
                for (auto &neighbor : neighbors)
                {
                    if (pheromone_map.find(std::make_pair(current, neighbor)) != pheromone_map.end())
                    {
                        weights.push_back(std::pow(pheromone_map[std::make_pair(current, neighbor)], alpha) / sumPheromone);
                    }
                    else
                    {
                        weights.push_back(std::pow(0.1, alpha) / sumPheromone);
                    }
                }
                std::discrete_distribution<> dist(weights.begin(), weights.end());
                std::pair<int, int> pre = current;
                int choice = dist(gen);
                current = neighbors.at(choice);
                path.push_back(current);
                usedTrails.insert(std::make_pair(pre, current));
            }
            if (path.size() < bestPathLength)
            {
                bestPath = path;
                bestPathLength = path.size();
            }
            for (auto &trail : usedTrails)
            {
                double delta = pheromoneConst / path.size();
                pheromoneDelta[trail] += delta;
            }
        }
        for (auto &trail : pheromone_map)
        {
            pheromone_map[trail.first] *= (1 - evaporationRate);
        }
        for (auto &trail : pheromoneDelta)
        {
            pheromone_map[trail.first] += trail.second;
        }
    }
    return pheromone_map;
}

// int main()
// {
//     // std::vector<std::vector<bool>> maze = {
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, true, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, true, false, false, false},
//     //     {true, false, false, false, false, true},
//     //     {false, false, true, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, true, false},
//     // };
//     int m = 5;
//     int n = 5;
//     std::vector<std::vector<bool>> maze = {
//         {false, false, false, false, false},
//         {true, true, false, true, false},
//         {false, false, false, false, false},
//         {false, true, true, true, false},
//         {false, false, false, false, false}};
//     auto start = std::chrono::high_resolution_clock::now();
//     // std::vector<std::pair<int, int>> solution = ACO(maze, std::make_pair(0, 0), std::make_pair(4, 3));
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> solution = pheroMap(maze, std::make_pair(0, 0), std::make_pair(4, 3));
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;
//     std::cout << "Thời gian thực hiện tuần tự: " << duration.count() << " giây\n";
//     // Print the path
//     // std::cout << "Path:\n";
//     // for (auto &pos : solution)
//     // {
//     //     std::cout << "(" << pos.first << ", " << pos.second << ") ";
//     // }
//     // std::cout << std::endl
//     //           << "Step: " << solution.size() << std::endl;
//     return 0;
// }






// int main()
// {
//     // std::vector<std::vector<bool>> maze = {
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, true, false, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, true, false, false, false},
//     //     {true, false, false, false, false, true},
//     //     {false, false, true, false, false, false},
//     //     {false, false, false, false, false, false},
//     //     {false, false, false, false, true, false},
//     // };
//     int m = 5;
//     int n = 5;
//     std::vector<std::vector<bool>> maze = {
//         {false, false, false, false, false},
//         {true, true, false, true, false},
//         {false, false, false, false, false},
//         {false, true, true, true, false},
//         {false, false, false, false, false}};
//     auto start = std::chrono::high_resolution_clock::now();
//     std::vector<std::pair<int, int>> solution = ACO(maze, std::make_pair(0, 0), std::make_pair(4, 3));
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;
//     std::cout << "Thời gian thực hiện tuần tự: " << duration.count() << " giây\n";
//     // Print the path
//     std::cout << "Path:\n";
//     for (auto &pos : solution)
//     {
//         std::cout << "(" << pos.first << ", " << pos.second << ") ";
//     }
//     std::cout << std::endl
//               << "Step: " << solution.size() << std::endl;
//     return 0;
// }