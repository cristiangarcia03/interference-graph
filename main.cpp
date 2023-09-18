#include "InterferenceGraph.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {

    InterferenceGraph<string> g;
    g.addVertex("a");
    g.addVertex("b");
    g.addVertex("c");

    g.addEdge("a","c");
    //g.removeVertex("a");
    g.view();

    return 0;
}

// g++ main.cpp -std=c++11 -o p
