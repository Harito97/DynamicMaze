#include "../stmaze/stmaze.cpp"
#include "../serial/AStar.cpp"
#include "../serial/ACO.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <map>

// AStar vector path to AStar hash map
std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, double> astarMap(std::vector<std::pair<int, int>> astartPath)
{
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, double> astar_map;
    for (int i = 0; i < astartPath.size() - 1; ++i)
    {
        astar_map.insert({std::make_pair(astartPath[i], astartPath[i + 1]), (i + 20.0) * 10.0});
    }
    return astar_map;
}

// Find max probability to choose
int max_probability(std::vector<double> probabilities)
{
    double max_probability = -1;
    int chosen_index = -1;
    for (int i = 0; i < probabilities.size(); i++)
    {
        if (probabilities[i] > max_probability)
        {
            max_probability = probabilities[i];
            chosen_index = i;
        }
    }
    return chosen_index;
}

// Check the path be going is the back step or not
bool back_step(std::vector<std::pair<int, int>> path, std::pair<std::pair<int, int>, std::pair<int, int>> future_path)
{
    std::pair<int, int> second_point = future_path.second;
    std::pair<int, int> first_point = future_path.first;
    if (path.size() <= 1)
    {
        return false;
    }
    for (int i = 0; i < path.size() - 1; ++i)
    {
        if (second_point.first == path[i].first && second_point.second == path[i].second 
            && first_point.first == path[i + 1].first && first_point.second == path[i + 1].second)
        {
            return true;
        }
    }
    return false;
}

// Create a random double number
double random_double_number() 
{
    // Use a random device as a seed
    std::random_device rd;

    // Choose a random number engine and seed it with the random device
    std::mt19937 gen(rd());

    // Define a uniform distribution over the range [0, 1)
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    // Generate a random double in the specified range
    return dis(gen);
}

// Function use ACO-based movement of objects on dynamic maze to reach end point
void solveDyMaze(std::vector<std::vector<bool>> &maze, const std::pair<std::pair<int, int>, std::pair<int, int>> &start_end,
                 std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> &pheromone_map,
                 std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, double> &astar_map,
                 int num_objects)
{
    // Initialize variables
    int num_reached = 0;                                              // Count of objects that reached the destination
    int num_not_reached = 0;                                          // Count of objects that can't reached the destination
    std::vector<std::vector<std::pair<int, int>>> paths;
    for (int i = 0; i < num_objects; ++i) {
        std::vector<std::pair<int, int>> ant_path;
        paths.push_back(ant_path);
    } // Paths taken by each object
    std::vector<std::pair<int, int>> current;                         // Current point for each object
    for (int obj = 0; obj < num_objects; ++obj)
    {
        current.push_back(start_end.first);
    }
    
    
    // Main simulation loop
    while (true)
    {
        for (int obj = 0; obj < num_objects; ++obj)
        {
            if (current[obj] == std::make_pair(-1, -1))
            {
                continue; // Skip if the object has reached the destination or marked as failed
            }

            // Determine neighbors and calculate probabilities
            std::vector<std::pair<int, int>> neighbors = getNeighbors(current[obj], maze);
            std::vector<double> probabilities(neighbors.size(), 0.0);

            // Update probabilities based on pheromone_map and A* information
            int i = 0;
            for (auto &neighbor : neighbors)
            {
                std::pair<std::pair<int, int>, std::pair<int, int>> future_path = std::pair{current[obj], neighbor};
                // Decreasing the pheromone if is the back step
                if (back_step(paths[obj], future_path))
                {
                    std::pair<std::pair<int, int>, std::pair<int, int>> bad_path = std::pair{future_path.second, future_path.first};
                    pheromone_map[bad_path] *= 0.6;
                }
                if (pheromone_map.find(future_path) != pheromone_map.end())
                {
                    // future_path can go throw
                    if (astar_map.find(future_path) != astar_map.end()) 
                    {
                        // future_path is a part of AStar optimize path in static maze
                        pheromone_map[future_path] *= 1.15;
                    }
                    pheromone_map[future_path] *= 1.15;
                }        
                probabilities[i] = pheromone_map[future_path] * random_double_number();
            }

            // Choose the neighbor with the highest probability
            int chosen_index = max_probability(probabilities);

            // Move the object and update the maze
            paths[obj].push_back(neighbors[chosen_index]);
            current[obj] = neighbors[chosen_index];
            changeMaze(maze, 0.05, 0);

            // Check if the object has reached the destination
            if (current[obj] == start_end.second)
            {
                ++num_reached;
            }

            // Check if the object is stuck (no available neighbors)
            if (neighbors.empty())
            {
                current[obj] = std::make_pair(-1, -1); // Mark as failed
                ++num_not_reached;
            }
        }

        // Check termination condition
        if (num_not_reached + num_reached == num_objects)
        {
            break; // All objects have either reached or failed
        }
    }

    // Output results
    // for (int obj = 0; obj < num_objects; obj++)
    // {
    //     std::cout << "Object " << obj + 1 << ": ";
    //     if (num_reached[obj] == 1)
    //     {
    //         std::cout << "Reached destination. Path: ";
    //         for (const auto &step : paths[obj])
    //         {
    //             std::cout << "(" << step.first << "," << step.second << ") ";
    //         }
    //     }
    //     else
    //     {
    //         std::cout << "Failed to reach destination.";
    //     }
    //     std::cout << std::endl;
    // }
}

// int main()
// {
//     // Define maze and start_end
//     std::vector<std::vector<bool>> dymaze = {};
//     dymaze = createMaze(10, 10, 0.1);
//     std::pair<std::pair<int, int>, std::pair<int, int>> start_end = {};
//     start_end = selectStartAndEnd(dymaze);

//     // Initialize AStar hash map path
//     std::vector<std::pair<int, int>> astar_path = AStar(dymaze, start_end.first, start_end.second);
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, HashPair> astar_map;
//     astar_map = astar_map(astar_path);
    
//     // Initialize pheromone_map
//     std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromone_map;
//     pheromone_map = pheromoneMap(dymaze, start_end.first, start_end.second);

//     // Number of objects
//     int num_objects = 10;

//     // Run the solve for dynamic maze
//     solveDyMaze(dymaze, start_end, pheromone_map, astar_map, num_objects);

//     return 0;
// }

int main()
{
    // Define maze and start_end
    std::vector<std::vector<bool>> dymaze = createMaze(10, 10, 0.1);
    std::pair<std::pair<int, int>, std::pair<int, int>> start_end = selectStartAndEnd(dymaze);

    // // Initialize AStar hash map path
    std::vector<std::pair<int, int>> astarPath = AStar(dymaze, start_end.first, start_end.second);
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, double> astar_map = astarMap(astarPath);
    // // Initialize pheromone_map
    std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPair> pheromone_map = pheromoneMap(dymaze, start_end.first, start_end.second);

    // // Number of objects
    int num_objects = 10;

    // // Initialize variables
    std::vector<std::pair<int, int>> current(num_objects, start_end.first);
    std::vector<std::vector<std::pair<int, int>>> paths(num_objects, std::vector<std::pair<int, int>>());

    // // Run the solve for dynamic maze
    solveDyMaze(dymaze, start_end, pheromone_map, astar_map, num_objects);

    return 0;
}