// __Author: Pham Ngoc Hai _ Harito__
// #pragma once
#include <vector>
#include <utility>
#include <cstdlib>   // create random value: srand(time(NULL)); int answer = std::rand() % 20; - create a random value from 0 to 20
#include <algorithm> // find index of max element in vector
#include <chrono>

#include "Maze.cpp"
#include "Object.cpp"
#include "algo/AStar.cpp"
#include "algo/ACO.cpp"

class Problem
{
private:
    Maze maze;
    std::pair<std::pair<int, int>, std::pair<int, int>> startEnd;
    std::vector<std::pair<Object, bool>> objects;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, HashPairAStar> aStarPath;
    std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, double, HashPairACO> acoMap;
    std::pair<double, double> result; // <time from start to end; percent reached end point>

public:
    Problem(int numberObject = 77, std::pair<int, int> shape = {15, 15}, double diffLevel = 0.12, int distance = 10)
    {
        maze = Maze(shape, diffLevel);
        startEnd = maze.selectStartAndEnd(distance);
        std::cout << "Create maze and start-end point successful" << std::endl;
        for (int i = 0; i < numberObject; ++i)
        {
            Object object(startEnd.first);
            objects.push_back(std::make_pair(object, false)); // object hadn't dead yet
        }
        std::cout << "Create objects successful" << std::endl;
    }

private:
    void preDyMazeSerial(AStar &astar, ACO &aco)
    {
        // Use A* fine optimize path - return map
        aStarPath = astar.solve_serial(maze, startEnd.first, startEnd.second).second;
        std::cout << "Got A*'s optimize path in serial way" << std::endl;
        // Use ACO get pheromone map - return map
        acoMap = aco.solve_serial(maze, startEnd.first, startEnd.second).second;
        std::cout << "Got ACO's pheromone map in serial way" << std::endl;
    }

    void preDyMazeParallel(AStar &astar, ACO &aco)
    {
        // Use A* fine optimize path - return map
        aStarPath = astar.solve_parallel(maze, startEnd.first, startEnd.second).second;
        std::cout << "Got A*'s optimize path in parallel way" << std::endl;
        // Use ACO get pheromone map - return map
        acoMap = aco.solve_parallel(maze, startEnd.first, startEnd.second).second;
        std::cout << "Got ACO's pheromone map in parallel way" << std::endl;
    }

    // Algorithms
    std::vector<std::pair<Object, bool>> &getObjects()
    {
        return objects;
    }

    Maze &getMaze()
    {
        return maze;
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> &getStatEnd()
    {
        return startEnd;
    }

    bool stopCondition()
    {
        for (auto &object : objects)
        {
            if (object.second == false)
            {
                return false;
            }
        }
        return true;
    }

    void getChoice(std::pair<Object, bool> &object)
    {
        std::vector<std::pair<int, int>> neighbors = maze.getNeighbors(object.first.currentPoint());
        if (neighbors.empty())
        {
            object.second = true; // Object dead
            return;
        }
        // std::cout << neighbors.size() << std::endl;
        // for (auto &neighbor : neighbors)
        // {
        //     std::cout << neighbor.first << " " << neighbor.second << std::endl;
        // }
        std::vector<double> probities(neighbors.size());
        for (int i = 0; i < neighbors.size(); ++i)
        {
            if (neighbors[i] == startEnd.second)
            // if (neighbors.at(i).first == startEnd.second.first && neighbors.at(i).second == startEnd.second.second)
            {
                // Object get target point
                object.first.move(neighbors[i]);
                object.first.gotTarget();
                object.second = true; // Object got target point
                // std::cout << "Got target \n";
                // std::cout << object.first.currentPoint().first << "-" << object.first.currentPoint().second << " " << object.second << " <-> " << object.first.isTarget() << " " << object.first.length() << std::endl;
                return;
            }
            if (acoMap.find(std::make_pair(object.first.currentPoint(), neighbors.at(i))) != acoMap.end())
            {
                probities[i] += ((double)rand() / RAND_MAX) * (acoMap.find(std::make_pair(object.first.currentPoint(), neighbors.at(i)))->second);
                // std::cout << "Calculate probities with ACO \n";
            }
            auto it = aStarPath.find(object.first.currentPoint());

            if (it != aStarPath.end())
            {
                if (it->second == neighbors.at(i))
                {
                    // Code to handle the case where the comparison is true
                    probities[i] *= ((double)rand() / RAND_MAX + 1);
                    // std::cout << "Calculate probities with A* \n";
                }
                else
                {
                    // Code to handle the case where the comparison is false
                    // std::cout << "Neighbor is not in A* path \n";
                }
            }
            else
            {
                // Code to handle the case where the key is not found in aStarPath
                // std::cout << "Current point is not in A* path \n";
            }
        }
        auto maxProbityIndex = std::max_element(probities.begin(), probities.end()) - probities.begin();
        object.first.move(neighbors[maxProbityIndex]);
        // std::cout << object.first.currentPoint().first << "-" << object.first.currentPoint().second << " " << object.second << " <-> " << object.first.isTarget() << " " << object.first.length() << std::endl;
    }

public:
    std::string solve()
    {
        srand(time(NULL));
        std::string result = "RESULT: \n";

        std::cout << getStatEnd().first.first << "-" << getStatEnd().first.second << " " << getStatEnd().second.first << "-" << getStatEnd().second.second << std::endl;
        // 1. Prepare work on static maze
        auto start1 = std::chrono::high_resolution_clock::now();
        AStar astar;
        ACO aco;
        // preDyMazeSerial(astar, aco);
        preDyMazeParallel(astar, aco);
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration1 = end1 - start1;
        std::cout << "Time to prepare in static maze: " << std::to_string(duration1.count()) << " s\n";
        result += "Time to prepare in static maze: " + std::to_string(duration1.count()) + " s\n";
        // 2. Start main problem in dynamic maze
        // Condition to stop while loop:
        // +    All maze have no more path to go <-> every object that haven't reach target yet have the neighbors list = {}
        // +    All object reached the target <-> every object have getTarget property = true
        auto start2 = std::chrono::high_resolution_clock::now();
        while (!stopCondition())
        {
            // Each object consider it's choice
            for (auto &object : getObjects())
            {
                getChoice(object);
            }
            getMaze().changeMaze(0.00001, 0.0);
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration2 = end2 - start2;
        std::cout << "Time work in dynamic maze: " << std::to_string(duration2.count()) << " s\n";
        result += "Time work in dynamic maze: " + std::to_string(duration2.count()) + " s\n";

        int numberGotTarget = 0;
        for (auto &object : getObjects())
        {
            if (object.first.isTarget())
            {
                ++numberGotTarget;
            }
            // std::cout << object.first.currentPoint().first << "-" << object.first.currentPoint().second << " " << object.second << " <-> " << object.first.isTarget() << " " << object.first.length() << std::endl;
        }
        result += "Number get target point / total object from start: " + std::to_string(numberGotTarget) + " / " + std::to_string(getObjects().size()) + "\n";
        std::cout << "The process finished with result: \n" << result;
        return result;
    };
};
