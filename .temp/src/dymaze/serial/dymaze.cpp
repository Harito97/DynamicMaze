// #include "../stmaze/stmaze.cpp"
// #include "../serial/AStar.cpp"
// #include "../serial/ACO.cpp"

// #include <iostream>
// #include <vector>
// #include <queue>
// #include <algorithm>
// #include <climits>
// #include <unordered_map>

// struct HashPair
// {
//     template <class T1, class T2, class T3, class T4>
//     std::size_t operator()(const std::pair<std::pair<T1, T2>, std::pair<T3, T4>> &p) const
//     {
//         auto h1 = std::hash<T1>{}(p.first.first);
//         auto h2 = std::hash<T2>{}(p.first.second);
//         auto h3 = std::hash<T3>{}(p.second.first);
//         auto h4 = std::hash<T4>{}(p.second.second);

//         return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
//     }
// };

// // Convert vector to unordered_map
// std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> vector_to_map(std::vector<std::pair<int, int>> &vectorMap)
// {
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> path_map;
//     // Assign values to the map
//     for (int i = 0; i < vectorMap.size(); i+=2) {
//         // Set the value to 0.0 for each pair
//         path_map[std::pair<vectorMap[i], vectorMap[i+1]>] = 0.0;
//     }
//     return path_map;
// }
    
// int main()
// {
//     // What are we trying to do?
//     //     |___Identify dynamic maze
//     //     |___Use A* to get optimize path and ACO pheromone map (work on static maze)
//     //     |___Start the army one by one from the start point to the end point
//     //     |   (follow the pheromone map - if current path is the children path of
//     //     |   optimize path of A* then property to go increasing)
//     //     |       |___Config the property choose to go is the key point
//     //     |           (how the maze change can be effect to the property choose)
//     //     |___Result: The army go to end point in min time. And number go to end point should be max

//     // Step 0: Build dynamic maze
//     int row = 3;
//     int col = 3;
//     std::vector<std::vector<bool>> dymaze = createMaze(row, col, 0.1);
//     std::pair<std::pair<int, int>, std::pair<int, int>> start_end = selectStartAndEnd(dymaze);

//     // Step 1: Choose one moment of dymaze to create stmaze
//     // Do A* and ACO algorithm to solve problem with dymaze in Step 2
//     std::vector<std::vector<bool>> stmaze = dymaze;
//     std::vector<std::pair<int, int>> optimize_path_astar = AStar(stmaze, start_end.first, start_end.second);
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromone_map = pheromoneMap(stmaze, start_end.first, start_end.second);

    

//     // Step 2: Update maze and make all 'ant' run to destination point
//     // Always be number_start <= number_end after end of algorithm
//     int number_start = 10;
//     int number_end = 0;
//     std::vector<std::vector<std::pair<int, int>>> army; // army have number_start elements

//     // 2.1. Update maze and search path

//     // Initialization
//     double pheromoneConst = 1000.0;
//     double evaporationRate = 0.3;
//     double alpha = 0.6;
//     // A random device to generate random numbers
//     std::random_device rd;
//     // Use Mersenne Twister engine seeded with the random device
//     std::mt19937 gen(rd());
//     // A map to store pheromone levels on each trail between two points in the maze
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneMap;
//     bool is_live_maze = true;
//     while (is_live_maze || number_end == number_start)
//     {
//         // A new pheromoneDelta map is created to store the pheromone updates for each ant
//         std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromoneDelta;
//         for (int i = 0; i < number_start; ++i)
//         {

//             std::pair<int, int> current = start_end.first;
//             int dir = 1;
//             std::vector<std::pair<int, int>> path = {current};
//             std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPair> usedTrails;
//             // If not reach to end point
//             if (current.first != start_end.second.first || current.second != start_end.second.second)
//             {
//                 // Iterate all possibility neighbors
//                 std::vector<std::pair<int, int>> neighbors = getNeighbors(current, dymaze);
//                 std::vector<double> weights;
//                 if (neighbors.empty()) {
//                     // This army was dead
//                     continue;
//                 }
//                 for (auto &neighbor : neighbors)
//                 {
//                     // If exist a path in pheromone map
//                     if (pheromoneMap.find(std::make_pair(current, neighbor)) != pheromoneMap.end())
//                     {
//                         // Update pheromone map
//                         if (usedTrails.find(std::make_pair(current, neighbor)) != usedTrails.end()) {
//                             // Increasing if not go to >= 2 times or current path is one path of A* optimize path
//                             pheromoneMap[std::make_pair(current, neighbor)] *= 1.1;
//                         } else {
//                             // Decreasing if go to >= 2 times
//                             pheromoneMap[std::make_pair(current, neighbor)] *= 0.8;
//                         }
//                     }
//                     else
//                     {
//                         // Do nothing yet - because maze change follow the way False -> True
//                     }
//                 }
//                 // for (auto &neighbor : neighbors)
//                 // {
//                 //     if (pheromoneMap.find(std::make_pair(current, neighbor)) != pheromoneMap.end())
//                 //     {
//                 //         // weights.push_back(std::pow(pheromoneMap[std::make_pair(current, neighbor)], alpha) / sumPheromone);
//                 //     }
//                 //     else
//                 //     {
//                 //         // weights.push_back(std::pow(0.1, alpha) / sumPheromone);
//                 //     }
//                 // }

//                 // What neighbor should be choose?
//                 std::discrete_distribution<> dist(weights.begin(), weights.end());
//                 std::pair<int, int> pre = current;
//                 int choice = dist(gen);
//                 current = neighbors.at(choice);
//                 // Add the current to the path
//                 army[i].push_back(current);
//                 usedTrails.insert(std::make_pair(pre, current));
//             }
//             else
//             {
//                 // successful get end point
//                 ++number_end;
//             }
//             // if (path.size() < bestPathLength)
//             // {
//             //     bestPath = path;
//             //     bestPathLength = path.size();
//             // }
//             // for (auto &trail : usedTrails)
//             // {
//             //     double delta = pheromoneConst / path.size();
//             //     pheromoneDelta[trail] += delta;
//             // }
//         }
//         is_live_maze = changeMaze(dymaze, 0.05, 0);
//     }

//     // printMaze(dymaze);
//     // std::cout << "Path by serial AStar:\n";
//     // for (const auto &point : optimize_path_astar)
//     // {
//     //     std::cout << "(" << point.first << ", " << point.second << ") ";
//     // }
//     // std::cout << std::endl
//     //           << "Step: " << optimize_path_astar.size() << std::endl;

//     // std::cout << "Pheromone Map:\n";
//     // for (const auto& entry : pheromone_map) {
//     //     const auto& key = entry.first;
//     //     double value = entry.second;

//     //     std::cout << "Path: ((" << key.first.first << ", " << key.first.second << "), ("
//     //               << key.second.first << ", " << key.second.second << ")) - Pheromone Value: " << value << std::endl;
//     // }
//     return 0;
// }
