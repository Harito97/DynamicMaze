#pragma once
#include <omp.h>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <climits>
#include <algorithm>
#include <chrono>
#include "../include/HashPairACO.h"
#include "../Maze.cpp"

class ACO
{
private:
    int num_ants;
    int iterations;
    double pheromone_const;
    double evaporation_rate;
    double alpha;

public:
    ACO(int num_ants = 16, int iterations = 12, double pheromone_const = 1000.0, double evaporation_rate = 0.3, double alpha = 0.6)
        : num_ants(num_ants), iterations(iterations), pheromone_const(pheromone_const), evaporation_rate(evaporation_rate), alpha(alpha) {}

    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO>>
    solve_serial(Maze &maze, std::pair<int, int> start, std::pair<int, int> end)
    {
        std::mt19937 gen(time(nullptr));
        std::vector<std::pair<int, int>> best_path;
        int best_path_length = INT_MAX;
        std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO> pheromone_map;

        for (int iteration = 0; iteration < iterations; ++iteration)
        {
            std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO> pheromone_delta;

            for (int ant = 0; ant < num_ants; ++ant)
            {
                std::pair<int, int> pre = {-1, -1};
                std::pair<int, int> current = start;
                std::vector<std::pair<int, int>> path = {current};
                std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPairACO> used_trails;
                bool isDead = false;
                while (current != end)
                {
                    std::vector<std::pair<int, int>> neighbors = maze.getNeighbors(current);
                    if (neighbors.size() == 0)
                    {
                        isDead = true;
                        break;
                    }
                    double sum_pheromone = 0.0;

                    for (const auto &neighbor : neighbors)
                    {
                        if (pheromone_map.find({current, neighbor}) != pheromone_map.end())
                        {
                            sum_pheromone += pheromone_map[{current, neighbor}];
                        }
                        else
                        {
                            sum_pheromone += 0.1;
                        }
                    }

                    std::vector<double> weights;

                    for (const auto &neighbor : neighbors)
                    {
                        if (pheromone_map.find({current, neighbor}) != pheromone_map.end())
                        {
                            weights.push_back(std::pow(pheromone_map[{current, neighbor}], alpha) / sum_pheromone);
                        }
                        else
                        {
                            weights.push_back(std::pow(0.1, alpha) / sum_pheromone);
                        }
                    }

                    std::discrete_distribution<> distribution(weights.begin(), weights.end());
                    int nextMove;
                    while (true)
                    {
                        nextMove = distribution(gen);
                        // if (neighbors.size() > 1 && neighbors[nextMove] != pre)
                        if (neighbors.size() > 1)
                        {
                            if (neighbors[nextMove] != pre)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    pre = current;
                    current = neighbors[nextMove];
                    path.push_back(current);
                    used_trails.insert({pre, current});
                }
                if (isDead)
                {
                    continue;
                }
                // std::cout << path.size() << std::endl;
                if (path.size() < best_path_length)
                {
                    best_path = path;
                    best_path_length = path.size();
                }

                for (const auto &trail : used_trails)
                {
                    double delta = pheromone_const / path.size();
                    pheromone_delta[trail] += delta;
                }
            }

            for (auto &trail : pheromone_map)
            {
                pheromone_map[trail.first] *= (1 - evaporation_rate);
            }

            for (const auto &trail : pheromone_delta)
            {
                pheromone_map[trail.first] += trail.second;
            }
        }

        return std::make_pair(best_path, pheromone_map);
    }

    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO>>
    solve_parallel(Maze &maze, std::pair<int, int> start, std::pair<int, int> end)
    {
        std::vector<std::pair<int, int>> best_path;
        int best_path_length = INT_MAX;
        std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO> pheromone_map;

        for (int iteration = 0; iteration < iterations; ++iteration)
        {
            std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO> pheromone_delta;
#pragma omp parallel for
            for (int ant = 0; ant < num_ants; ++ant)
            {
                std::mt19937 gen(time(nullptr) ^ omp_get_thread_num());
                std::pair<int, int> pre = {-1, -1};
                std::pair<int, int> current = start;
                std::vector<std::pair<int, int>> path = {current};
                std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPairACO> used_trails;
                bool isDead = false;
                while (current != end)
                {
                    std::vector<std::pair<int, int>> neighbors = maze.getNeighbors(current);
                    if (neighbors.size() == 0)
                    {
                        isDead = true;
                        break;
                    }

                    double sum_pheromone = 0.0;

                    for (const auto &neighbor : neighbors)
                    {
                        if (pheromone_map.find({current, neighbor}) != pheromone_map.end())
                        {
                            sum_pheromone += pheromone_map[{current, neighbor}];
                        }
                        else
                        {
                            sum_pheromone += 0.1;
                        }
                    }

                    std::vector<double> weights;

                    for (const auto &neighbor : neighbors)
                    {
                        if (pheromone_map.find({current, neighbor}) != pheromone_map.end())
                        {
                            weights.push_back(std::pow(pheromone_map[{current, neighbor}], alpha) / sum_pheromone);
                        }
                        else
                        {
                            weights.push_back(std::pow(0.1, alpha) / sum_pheromone);
                        }
                    }
                    std::discrete_distribution<> distribution(weights.begin(), weights.end());
                    int nextMove;
                    while (true)
                    {
                        nextMove = distribution(gen);
                        // if (neighbors.size() > 1 && neighbors[nextMove] != pre)
                        if (neighbors.size() > 1)
                        {
                            if (neighbors[nextMove] != pre)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    pre = current;
                    current = neighbors[nextMove];
                    path.push_back(current);
                    used_trails.insert({pre, current});
                }
                if (isDead)
                {
                    continue;
                }
#pragma omp critical
                {
                    if (path.size() < best_path_length)
                    {
                        best_path = path;
                        best_path_length = path.size();
                    }
                }
#pragma omp critical
                {
                    for (const auto &trail : used_trails)
                    {
                        double delta = pheromone_const / path.size();
                        pheromone_delta[trail] += delta;
                    }
                }
            }

            // for (auto &trail : pheromone_map)
            // {
            //     pheromone_map[trail.first] *= (1 - evaporation_rate);
            // }

            // for (const auto &trail : pheromone_delta)
            // {
            //     pheromone_map[trail.first] += trail.second;
            // }

            // if (iteration != 0)
            // {
            //     int maxThreads = omp_get_max_threads();
            //     int pMapSize = pheromone_map.size();
            //     int numThreads = maxThreads > pMapSize ? pMapSize : maxThreads;
            //     int chunkSize = int(numThreads / pMapSize);
            //     auto start = pheromone_map.begin();

            for (auto trail = pheromone_map.begin(); trail != pheromone_map.end(); ++trail)
            {
                pheromone_map[trail->first] *= (1 - evaporation_rate);
            }
            // }

            for (auto trail = pheromone_delta.begin(); trail != pheromone_delta.end(); ++trail)
            {
                pheromone_map[trail->first] += trail->second;
            }
        }

        return std::make_pair(best_path, pheromone_map);
    }
};

// int main()
// {
//     Maze myMaze = {
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//     };
//     myMaze.print();

//     std::pair<int, int> start = {0, 0};
//     std::pair<int, int> end = {7, 7};

//     ACO myACO;

//     auto start1 = std::chrono::high_resolution_clock::now();
//     auto result1 = myACO.solve_serial(myMaze, start, end);
//     auto end1 = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration1 = end1 - start1;
//     std::cout << "Thời gian thực hiện tuần tự: " << duration1.count() << " giây\n";

//     // Print path
//     std::cout << "Path: ";
//     for (const auto &point : result1.first)
//     {
//         std::cout << "(" << point.first << ", " << point.second << ") ";
//     }
//     std::cout << std::endl;

//     auto start2 = std::chrono::high_resolution_clock::now();
//     auto result2 = myACO.solve_parallel(myMaze, start, end);
//     auto end2 = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration2 = end2 - start2;
//     std::cout << "Thời gian thực hiện song song: " << duration2.count() << " giây\n";

//     // Print path
//     std::cout << "Path: ";
//     for (const auto &point : result2.first)
//     {
//         std::cout << "(" << point.first << ", " << point.second << ") ";
//     }
//     std::cout << std::endl;

//     std::cout << "Speed Up: " << duration1.count() / duration2.count() << " lần" << std::endl;

//     // // Print map
//     // for (auto path : result1.second){
//     //     std::cout << path.first.first.first << "-" << path.first.first.second << " " << path.first.second.first << "-" << path.first.second.second << " : " << path.second << "\n";
//     // }
// }
