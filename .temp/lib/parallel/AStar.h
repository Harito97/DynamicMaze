#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

struct Node
{
    int x, y; // Coordinates of the node
    int g;    // Cost from the start node to current node
    int h;    // Heuristic: estimated cost from current node to the goal
    int f;    // Total cost: g + h

    bool operator>(const Node &other) const
    {
        return f > other.f;
    }
};

// Function to check if a given cell is valid (inside the maze and not blocked)
bool isValid(int x, int y, const std::vector<std::vector<bool>> &maze)
{
    return x >= 0 && y >= 0 && x < maze.size() && y < maze.at(0).size() && !maze[x][y];
}

// Function to calculate the heuristic (Manhattan distance)
int heuristic(int x, int y, const std::pair<int, int> &end)
{
    return std::abs(x - end.first) + std::abs(y - end.second);
}

std::vector<std::pair<int, int>> AStar(std::vector<std::vector<bool>> &maze, std::pair<int, int> start, std::pair<int, int> end)
{
    std::vector<std::pair<int, int>> path;
    int m = maze.size();
    int n = maze.at(0).size();

    // Directions: up, down, left, right
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    // Priority queue to store nodes based on their total cost
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    // Initialize the start node
    Node startNode = {start.first, start.second, 0, heuristic(start.first, start.second, end), 0};
    pq.push(startNode);

    // Matrix to store the cost to reach each cell
    std::vector<std::vector<int>> cost(m, std::vector<int>(n, INT_MAX));
    cost[start.first][start.second] = 0;

    // Matrix to store the parent of each cell
    std::vector<std::vector<std::pair<int, int>>> parent(m, std::vector<std::pair<int, int>>(n, {-1, -1}));

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();

        // Check if the current node is the goal
        if (current.x == end.first && current.y == end.second)
        {
            // Reconstruct the path from the goal to the start
            while (current.x != -1 && current.y != -1)
            {
                path.push_back({current.x, current.y});
                current = {parent[current.x][current.y].first, parent[current.x][current.y].second, 0, 0, 0};
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
#pragma omp parallel for
        for (int i = 0; i < 4; ++i)
        {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // Check if the neighbor is valid
            if (isValid(newX, newY, maze))
            {
                int newG = current.g + 1;

                // Check if the new path to the neighbor is shorter
                if (newG < cost[newX][newY])
                {
                    cost[newX][newY] = newG;
                    int newH = heuristic(newX, newY, end);
                    int newF = newG + newH;
                    pq.push({newX, newY, newG, newH, newF});

#pragma omp critical
                    {
                        parent[newX][newY] = {current.x, current.y};
                    }
                }
            }
        }
    }

    // If the goal is not reachable, return an empty path
    return path;
}

// int main()
// {
//     // Example usage
//     // (0, 0) (0, 1) (0, 2) (1, 2) (2, 2) (2, 3) (2, 4) (3, 4) (4, 4) (4, 3)
//     // std::vector<std::vector<bool>> maze = {
//     //     {false, false, false, false, false},
//     //     {true,  true,  false, true,  false},
//     //     {false, false, false, false, false},
//     //     {false, true,  true,  true,  false},
//     //     {false, false, false, false, false}
//     // };
//     // (0, 0) (1, 0) (2, 0) (3, 0) (4, 0) (4, 1) (5, 1) (6, 1) (6, 2) (6, 3) (7, 3) (8, 3) (8, 4)
//     std::vector<std::vector<bool>> maze = {
//         {false, false, false, false, false, false},
//         {false, false, false, false, false, false},
//         {false, false, false, false, false, false},
//         {false, true, false, false, false, false},
//         {false, false, false, false, false, false},
//         {false, false, true, false, false, false},
//         {true, false, false, false, false, true},
//         {false, false, true, false, false, false},
//         {false, false, false, false, false, false},
//         {false, false, false, false, true, false},
//     };
//     std::pair<int, int> start = {0, 0};
//     std::pair<int, int> end = {8, 4};
//     std::vector<std::pair<int, int>> path = AStar(maze, start, end);
//     // Print the path
//     std::cout << "Path by parallel:\n";
//     for (const auto &point : path)
//     {
//         std::cout << "(" << point.first << ", " << point.second << ") ";
//     }
//     std::cout << std::endl
//               << "Step: " << path.size() << std::endl;
//     return 0;
// }