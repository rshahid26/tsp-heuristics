#include <iostream>
#include "NearestNeighbor.hpp"
#include "GreedyTSP.hpp"

int main() {
    // Change the filepath here to one of
    // dj38.tsp, rw1621.tsp, eg7146.tsp, or ar9152.tsp

    string filename = "/Users/raasikh/CLionProjects/csci335_3/rw1621.tsp";
    nearestNeighbor(filename);
    greedyTSP(filename);

    return 0;
}
