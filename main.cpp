#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    // Read input graph
    string path = "./data_graph.txt";
    
    Graph g = Graph(path);

    g.printNodes();

    g.printEdges();

    std::cout << "\n";
    std::vector<std::vector<int>> proximity_list;

    // //g.getNodes()[0].printNode();

    breadthFirstSearch(g, 1, proximity_list);

    for(auto &list : proximity_list){
        for(auto &element : list){
            std::cout << element << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    g.printEdges("DISCOVERY_EDGE");

    Graph g_2 = Graph(path);
    depthFirstSearch(g_2, 1);
    g_2.printEdges("");


    return 0;
}
