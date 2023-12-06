#pragma once
#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <chrono>
#include <atomic>
#include <omp.h>
#include <climits>
#include "../include/HashPairAStar.h"
#include "../Maze.cpp"

class thread_state
{
public:

    std::priority_queue<std::tuple<int, int, std::pair<int, int>>,
                        std::vector<std::tuple<int, int, std::pair<int, int>>>,
                        std::greater<std::tuple<int, int, std::pair<int, int>>>>
        open_list;

    std::queue<std::pair<std::tuple<int, int, std::pair<int, int>>, int>> wait_list;

    std::vector<thread_state *> neighbors;

    std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> a_path;

    omp_lock_t lock;

public:
    thread_state()
    {
        omp_init_lock(&lock);
    }
};

class AStar
{
// public:
    // struct HashPairAStar
    // {
    //     template <class T1, class T2>
    //     std::size_t operator()(const std::pair<T1, T2> &p) const
    //     {
    //         auto hash1 = std::hash<T1>{}(p.first);
    //         auto hash2 = std::hash<T2>{}(p.second);

    //         // Combining the hashes to produce a unique hash for the pair
    //         return hash1 ^ hash2;
    //     }
    // };

private:
    int heuristic(std::pair<int, int> p1, std::pair<int, int> p2)
    {
        return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
    }

public:
    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar>>
    solve_serial(Maze &maze, std::pair<int, int> start, std::pair<int, int> end)
    {
        std::vector<std::vector<int>> g_score(maze.getHeight(), std::vector<int>(maze.getWidth(), INT_MAX));
        g_score[start.first][start.second] = 0;

        // std::vector<std::vector<int>> f_score(maze.getHeight(), std::vector<int>(maze.getWidth(), INT_MAX));
        // f_score[start.first][start.second] = heuristic(start, end);

        std::priority_queue<std::tuple<int, int, std::pair<int, int>>,
                            std::vector<std::tuple<int, int, std::pair<int, int>>>,
                            std::greater<std::tuple<int, int, std::pair<int, int>>>>
            open;

        open.push(std::make_tuple(heuristic(start, end), heuristic(start, end), start));

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> a_path;

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

                if (temp_g_score < g_score[neighbor.first][neighbor.second])
                {
                    g_score[neighbor.first][neighbor.second] = temp_g_score;
                    // f_score[neighbor.first][neighbor.second] = temp_f_score;
                    open.push(std::make_tuple(temp_f_score, heuristic(neighbor, end), neighbor));
                    a_path[neighbor] = current;
                }
            }
        }

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> fwd_path;
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

private:
    std::vector<std::tuple<int, int, std::pair<int, int>>>
    start_expand(Maze &maze, std::pair<int, int> start, std::pair<int, int> end,
                 std::vector<std::vector<int>> &g_score,
                 std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> &a_path,
                 int num_threads)
    {
        std::priority_queue<std::tuple<int, int, std::pair<int, int>>,
                            std::vector<std::tuple<int, int, std::pair<int, int>>>,
                            std::greater<std::tuple<int, int, std::pair<int, int>>>>
            open;

        open.push(std::make_tuple(heuristic(start, end), heuristic(start, end), start));
        std::vector<std::tuple<int, int, std::pair<int, int>>> expand_list;

        while (!open.empty())
        {
            auto current = std::get<2>(open.top());
            open.pop();

            if (current == end)
            {
                return std::vector<std::tuple<int, int, std::pair<int, int>>>();
            }

            auto neighbors = maze.getNeighbors(current);

            for (const auto &neighbor : neighbors)
            {
                int temp_g_score = g_score[current.first][current.second] + 1;
                int temp_f_score = temp_g_score + heuristic(neighbor, end);

                if (temp_g_score < g_score[neighbor.first][neighbor.second])
                {
                    g_score[neighbor.first][neighbor.second] = temp_g_score;
                    // f_score[neighbor.first][neighbor.second] = temp_f_score;
                    open.push(std::make_tuple(temp_f_score, heuristic(neighbor, end), neighbor));
                    a_path[neighbor] = current;
                    if (open.size() == num_threads)
                    {
                        while (!open.empty())
                        {
                            expand_list.push_back(open.top());
                            open.pop();
                        }
                        return expand_list;
                    }
                }
            }
        }
        while (!open.empty())
        {
            expand_list.push_back(open.top());
            open.pop();
        }
        return expand_list;
    }

public:
    std::pair<std::vector<std::pair<int, int>>, std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar>>
    solve_parallel(Maze &maze, std::pair<int, int> start, std::pair<int, int> end, int num_threads = omp_get_max_threads())
    {
        std::vector<std::vector<int>> g_score(maze.getHeight(), std::vector<int>(maze.getWidth(), INT_MAX));
        g_score[start.first][start.second] = 0;

        // std::vector<std::vector<int>> f_score(maze.getHeight(), std::vector<int>(maze.getWidth(), INT_MAX));
        // f_score[start.first][start.second] = heuristic(start, end);

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> a_path;

        std::atomic<int> open_node_num;
        thread_state *thread_array;

        auto node_list = start_expand(maze, start, end, g_score, a_path, num_threads);

        const int busy_threshold = 100;

        if (node_list.empty())
        {
            std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> fwd_path;
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
        else
        {
            thread_array = new thread_state[num_threads];
            srand(time(NULL));
            for (int i = 0; i < node_list.size(); ++i)
            {
                int thread_id = rand() % num_threads;
                thread_array[thread_id].open_list.push(node_list[i]);
                thread_array[thread_id].a_path = a_path;
            }
            open_node_num = node_list.size();

            int bit_num = (int)log2(num_threads);
            for (int i = 0; i < num_threads; ++i)
            {
                for (int j = 0; j < bit_num; ++j)
                {
                    int neighbor = i ^ (1 << j);
                    thread_array[i].neighbors.push_back(&thread_array[neighbor]);
                }
            }
            omp_set_num_threads(num_threads);
        }

        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> fwd_path;
        std::vector<std::pair<int, int>> path;
        int optimal_length = INT_MAX;

#pragma omp parallel
        {
            std::vector<std::vector<int>> g_value(g_score);
            // std::vector<std::vector<int>> f_value(f_score);
            int id = omp_get_thread_num();

            const int DELTA_NODE_FLUSH_PERIOD = 10;

            int delta_node_flush_count = 0;
            int delta_node_num = 0;

            while (open_node_num > 0)
            {
                // std::cout << open_node_num << std::endl;
                delta_node_flush_count++;
                if (delta_node_flush_count > DELTA_NODE_FLUSH_PERIOD)
                {
                    delta_node_flush_count = 0;
                    open_node_num += delta_node_num;
                    delta_node_num = 0;
                }

                omp_set_lock(&thread_array[id].lock);

                if (thread_array[id].open_list.size() > busy_threshold && num_threads > 1)
                {
                    thread_array[id].wait_list.push({thread_array[id].open_list.top(), id});
                    thread_array[id].open_list.pop();
                }

                int current_open_size = thread_array[id].open_list.size();
                omp_unset_lock(&thread_array[id].lock);

                if (current_open_size == 0)
                {
                    int busiest_neighbor = -1;
                    int longest_wl = 0;
                    for (int i = 0; i < thread_array[id].neighbors.size(); i++)
                    {

                        auto neighbor = thread_array[id].neighbors[i];

                        omp_set_lock(&neighbor->lock);

                        if (neighbor->wait_list.size() > longest_wl)
                        {
                            busiest_neighbor = i;
                            longest_wl = neighbor->wait_list.size();
                        }

                        omp_unset_lock(&neighbor->lock);
                    }

                    if (longest_wl > 0)
                    {

                        omp_set_lock(&thread_array[id].neighbors[busiest_neighbor]->lock);

                        if (thread_array[id].neighbors[busiest_neighbor]->wait_list.size() > 0)
                        {
                            auto fetch_element = thread_array[id].neighbors[busiest_neighbor]->wait_list.front();

                            thread_array[id].neighbors[busiest_neighbor]->wait_list.pop();

                            thread_array[id].open_list.push(fetch_element.first);

                            auto current = std::get<2>(fetch_element.first);

                            int temp_g_value = std::get<0>(fetch_element.first) - std::get<1>(fetch_element.first);
                            // int temp_f_value = temp_g_value + heuristic(current, end);

                            if (temp_g_value < g_value[current.first][current.second])
                            {
                                g_value[current.first][current.second] = temp_g_value;
                                // f_value[current.first][current.second] = std::get<0>(fetch_element.first);
                                // a_path[neighbor] = current;
                                thread_array[id].a_path = thread_array[fetch_element.second].a_path;
                            }
                            omp_unset_lock(&thread_array[id].neighbors[busiest_neighbor]->lock);
                        }
                        else
                        {
                            omp_unset_lock(&thread_array[id].neighbors[busiest_neighbor]->lock);
                            continue;
                        }
                    }

                    else
                    {
                        continue;
                    }
                }

                // if (thread_array[id].open_list.size() == 0) {
                //     continue;
                // }

                omp_set_lock(&thread_array[id].lock);
                auto current_node = std::get<2>(thread_array[id].open_list.top());
                auto prev_node = thread_array[id].a_path[current_node];
                thread_array[id].open_list.pop();

                if (current_node == end)
                {
                    if (g_value[current_node.first][current_node.second] < optimal_length)
                    {
                        optimal_length = g_value[current_node.first][current_node.second];
                        std::cout << "The optimal length of A* path: " << optimal_length << std::endl;
                        std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> fwd_path_local;
                        std::vector<std::pair<int, int>> path_local = {end};
                        auto cell = end;
                        while (cell != start)
                        {
                            fwd_path_local[thread_array[id].a_path[cell]] = cell;
                            path_local.insert(path_local.begin(), thread_array[id].a_path[cell]);
                            cell = thread_array[id].a_path[cell];
                        }
                        std::swap(fwd_path, fwd_path_local);
                        std::swap(path, path_local);
                    }
                }

                omp_unset_lock(&thread_array[id].lock);

                delta_node_num -= 1;

                for (auto neighbor : maze.getNeighbors(current_node))
                {
                    if (prev_node == neighbor)
                    {
                        continue;
                    }
                    int temp_g_value = g_value[current_node.first][current_node.second] + 1;
                    int temp_f_value = temp_g_value + heuristic(neighbor, end);

                    if (temp_g_value < g_value[neighbor.first][neighbor.second])
                    {
                        g_value[neighbor.first][neighbor.second] = temp_g_value;
                        // f_value[neighbor.first][neighbor.second] = temp_f_value;
                        thread_array[id].open_list.push({temp_f_value, heuristic(neighbor, end), neighbor});
                        thread_array[id].a_path[neighbor] = current_node;
                        delta_node_num += 1;
                    }
                }
            }
        }
        return std::make_pair(path, fwd_path);
    }
};


// int main()
// {
//     // Maze myMaze({50, 50});
//     // std::pair<std::pair<int, int>, std::pair<int, int>> startEnd = myMaze.selectStartAndEnd();
//     // std::pair<int, int> startPos = startEnd.first;
//     // std::pair<int, int> endPos = startEnd.second;

//     Maze myMaze = {
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false},
//         {false, false, false, false, false, false, false, false}};
//     // myMaze.print();

//     std::pair<int, int> startPos = {0, 0};
//     std::pair<int, int> endPos = {7, 7};

//     AStar myAStar;

//     auto start1 = std::chrono::high_resolution_clock::now();
//     auto result1 = myAStar.solve_serial(myMaze, startPos, endPos);
//     auto end1 = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration1 = end1 - start1;
//     std::cout << "Thời gian thực hiện tuần tự: " << duration1.count() << " giây\n";

//     // Print path
//     std::cout << "Size: " << result1.first.size() << std::endl;
//     std::cout << "Path: ";
//     for (const auto &point : result1.first)
//     {
//         std::cout << "(" << point.first << ", " << point.second << ") ";
//     }
//     std::cout << std::endl;

//     // // Print map
//     // std::cout << "Map: ";
//     // for (const auto &point : result.second)
//     // {
//     //     std::cout << "[ (" << point.first.first << ", " << point.first.second << ") : (" << point.second.first << ", " << point.second.second << ") ]";
//     // }
//     // std::cout << std::endl;
//     // return 0;

//     auto start2 = std::chrono::high_resolution_clock::now();
//     auto result2 = myAStar.solve_parallel(myMaze, startPos, endPos, 8);
//     auto end2 = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration2 = end2 - start2;
//     std::cout << "Thời gian thực hiện song song: " << duration2.count() << " giây\n";

//     // Print path
//     std::cout << "Size: " << result2.first.size() << std::endl;
//     std::cout << "Path: ";
//     for (const auto &point : result2.first)
//     {
//         std::cout << "(" << point.first << ", " << point.second << ") ";
//     }
//     std::cout << std::endl;
// }
