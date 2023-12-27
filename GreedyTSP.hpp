#ifndef CSCI335_3_GREEDYTSP_HPP
#define CSCI335_3_GREEDYTSP_HPP

#include "NearestNeighbor.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std;

struct Edge {
    int source;
    int target;
    double weight;
    Edge(int source, int target, double weight) {
        this->source = source;
        this->target = target;
        this->weight = weight;
    }

    // Sort based on weights
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class UnionFind {
public:
    UnionFind(int n) {
        parent.resize(n );
        rank.resize(n);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int x) {
        if (x == parent[x]) return x; // x is the parent of its disjoint set
        return parent[x] = find(parent[x]); // compress the path to x
    }

    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false; // Node x and Node y are already connected

        // Union by rank to minimize the maximum path length within a set
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootX] = rootY;
            rank[rootY]++;
        }
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};


vector<Edge> getEdges(vector<Node>& nodes) {
    vector<Edge> edges;

    // Generate the (n choose 2) edges that would be present in a complete graph
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = i + 1; j < nodes.size(); ++j) {

            double weight = nodes[i].distanceTo(nodes[j]);
            Edge edge(i, j, weight);
            edges.push_back(edge);
        }
    }
    return edges;
}

void create_TSP_Path(vector<Node> &nodes) {
    vector<Edge> edges = getEdges(nodes);

    // Sort the edges by weight
    std::sort(edges.begin(), edges.end());
    UnionFind uf = UnionFind(nodes.size());

    for (Edge edge : edges) {
        int sourceID = edge.source;
        int targetID = edge.target;

        // Ensure the edge doesn't give a node an edge degree over 2
        if (nodes[sourceID].edges.size() >= 2 || nodes[targetID].edges.size() >= 2) {
            continue;
        }

        // Ensure we don't close the Hamiltonian cycle yet
        if ((sourceID == 0 || targetID == 0) && nodes[0].edges.size() == 1) {
            continue;
        }

        // Add the edge (bidirectionally) if it doesn't create a cycle
        if (uf.unionSets(sourceID, targetID)) {
            nodes[sourceID].addEdge(targetID);
            nodes[targetID].addEdge(sourceID);
        }

    }
}

vector<Node> get_TSP_Tour(const vector<Node>& nodes) {
    vector<Node> tour;
    vector<bool> visited(nodes.size(), false); // this entire function runs 0-indexed

    int currentNode = 0;
    while (true) {
        tour.push_back(nodes[currentNode]);
        visited[currentNode] = true;

        // Find the next node to visit
        int nextNode = -1;
        for (int neighbor : nodes[currentNode].edges) {
            if (!visited[neighbor]) {
                nextNode = neighbor;
                break;
            }
        }
        if (nextNode == -1) {
            // Complete the cycle
            tour.push_back(nodes[0]);
            return tour;
        }
        currentNode = nextNode;
    }
}

void greedyTSP(string filename) {
    vector<Node> nodes = readFile(filename);
    auto start = chrono::high_resolution_clock::now();


    create_TSP_Path(nodes);

    // Find the last node and connect it to nodes[0] to create a tour
    int lastNode = -1;
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].edges.size() == 1 && i != 0) {
            lastNode = i;
        }
    }

    nodes[lastNode].addEdge(0);
    nodes[0].addEdge(lastNode);

    vector<Node> tour = get_TSP_Tour(nodes);

    double totalDistance = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        totalDistance += tour[i].distanceTo(tour[i + 1]);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    // Print the tour
    for (auto node : tour) {
        cout << node.id << " ";
    }

    cout << "\nTotal Distance: " << totalDistance;
    cout << "\nTime in ms: " << duration.count() << endl;
    isValidTour(tour);
}

#endif //CSCI335_3_GREEDYTSP_HPP
