#ifndef CSCI335_3_NEARESTNEIGHBOR_HPP
#define CSCI335_3_NEARESTNEIGHBOR_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <unordered_set>

using namespace std;


class Node {
public:
    int id;
    double x, y;
    std::vector<int> edges; // Used in Greedy.tsp only

    Node(int id, double x, double y) {
        this->id = id;
        this->x = x;
        this-> y= y;
    }

    double distanceTo(const Node& other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }

    bool addEdge(int nodeId) {
        // Return false if the node already has an edge degree of 2
        if (edges.size() < 2) {
            edges.push_back(nodeId);
            return true;
        }
        return false;
    }
};

string readLine(ifstream& file) {
    string line;
    if (getline(file, line)) {
        // Remove letter that wont convert to string
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
    }
    return line;
}

vector<Node> readFile(const string& filename) {
    vector<Node> nodes;
    ifstream file(filename, ios::binary);
    bool startParsing = false;

    if (file.is_open()) {
        while (file) {
            string line = readLine(file);

            if (line == "NODE_COORD_SECTION") {
                startParsing = true;
                continue;
            }

            if (startParsing && !line.empty() && line != "EOF") {
                istringstream iss(line);
                int id;
                double x, y;
                if (iss >> id >> x >> y) {
                    nodes.emplace_back(id, x, y);
                }
            }
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
    return nodes;
}

vector<Node> get_Nearest_Neighbor_Path(const vector<Node>& nodes) {
    vector<Node> path;

    // Keep track of nodes that are in the path already
    vector<bool> visited(nodes.size(), false);

    // Begin at the first node
    Node current = nodes[0];
    visited[current.id - 1] = true;
    path.push_back(current);

    while (path.size() < nodes.size()) {
        double minDistance = numeric_limits<double>::max();
        int nearestNodeId = -1;

        for (const auto& node : nodes) {
            // Exclude nodes that are already visited. Ensures a Hamiltonian Path
            if (!visited[node.id - 1]) {
                double distance = current.distanceTo(node);
                if (distance < minDistance) {
                    // This node is nearest the current node
                    minDistance = distance;
                    nearestNodeId = node.id;
                }
            }
        }
        visited[nearestNodeId - 1] = true;
        current = nodes[nearestNodeId - 1];
        path.push_back(current);
    }
    return path;
}

void isValidTour(vector<Node> path) {
    unordered_set<int> uniqueNodes;
    for (const auto& node : path) {
        if (node.id != path.front().id) {
            uniqueNodes.insert(node.id);
        }
    }
    if (uniqueNodes.size() == path.size() - 2 && path.front().id == path.back().id) {
        cout << "valid tour" << endl;
    } else {
        cout << "invalid tour" << endl;
    }
}

void nearestNeighbor(string filename) {
    vector<Node> nodes = readFile(filename);

    auto startTime = chrono::high_resolution_clock::now();
    vector<Node> path = get_Nearest_Neighbor_Path(nodes);
    // Add the starting node to the end to turn the path into a tour
    path.push_back(nodes[0]);

    // Calculate the distance traveled by the NN tour
    double totalDistance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalDistance += path[i].distanceTo(path[i + 1]);
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

    for (const auto& node : path) {
        cout << node.id << " ";
    }
    cout << "\nTotal Distance: " << totalDistance;
    cout << "\nTime in ms: " << duration << endl;
    isValidTour(path);
}




#endif //CSCI335_3_NEARESTNEIGHBOR_HPP
