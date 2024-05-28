#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>

bool readData(std::string path, std::vector<std::tuple<int, int>> &input_edges)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (getline(file, line))
    {
        std::istringstream iss(line);
        int num1, num2;
        if (!(iss >> num1 >> num2))
        {
            std::cerr << "Error reading integers from line: " << line << std::endl;
            return false;
        }
        else
        {
            input_edges.push_back(std::tuple<int, int>(num1, num2));
        }
    }
    return true;
}

template <typename T>
bool containsElement(std::vector<T> list, T element)
{
    bool found = false;
    for (const T &elem : list)
    {
        if (elem == element)
        {
            return true;
        }
    }
    return found;
}

class Node
{
    int id;
    std::string label;
    std::vector<int> adjacency_list;

public:
    Node(int id, std::string label = "0") : id(id), label(label) {}
    int getId() { return id; }
    void addNeighbour(int neighbour)
    {
        adjacency_list.push_back(neighbour);
    }
    std::vector<int> getNeighbours() { return adjacency_list; }
    void visit() { label = "1"; }
    std::string getLabel() { return label; }
    void printNode()
    {
        std::cout << "Neighbours of " << getId() << ": ";
        for (auto &neighbour : adjacency_list)
        {
            std::cout << (neighbour) << " ";
        }
        std::cout << "\n";
    }
    bool operator==(const Node &other) const
    {
        return this->id == other.id;
    }
};

class Edge
{
    int start;
    int end;
    std::string label;

public:
    Edge(int start, int end, std::string label = "null") : start(start), end(end), label(label){};
    std::string getLabel() { return label; }
    void setLabel(std::string label) { this->label = label; }
    bool correspondsTo(int first, int second) { return start == first && end == second; }
    void printEdge(std::string filter_label="")
    {
        if(filter_label == "" || label == filter_label)
            std::cout << "(" << start << ", " << end << ") [" << label << "]" << "\n";
    }
};

class Graph
{
public:
    std::vector<Node> nodes;
    std::vector<Edge> edges;


    void addNode(Node &node)
    {
        if (!containsElement(nodes, node))
        {
            // std::cout << "Inserting node: " << node.getId() << " with address: " << &node << "\n";
            nodes.push_back(node);
        }
    }

    int searchNodeById(int id)
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i].getId() == id)
            {
                return i;
            }
        }
        return -1;
    }

    int searchEdgeById(int first, int second)
    {
        for (int i = 0; i < edges.size(); i++)
        {
            if (edges[i].correspondsTo(first, second))
            {
                return i;
            }
        }
        return -1;
    }

    void link(int first, int second)
    {
        int first_indx = searchNodeById(first);
        // int second_indx = searchNodeById(second.getId());
        if (first_indx != -1)
        {
            nodes[first_indx].addNeighbour(second);
        }
        else
        {
            std::cout << "first_indx == -1\n";
        }
    }

    Graph(std::string input_path)
    {
        // Reads from a .txt file the edges
        std::vector<std::tuple<int, int>> input_edges;
        readData(input_path, input_edges);

        for (auto &edge : input_edges)
        {
            // std::cout << "Inserting edge: (" << std::get<0>(edge) << ", " << std::get<1>(edge) << ")\n";
            //  Creates the two nodes and add them if they don't exist
            Node node_1 = Node(std::get<0>(edge));
            Node node_2 = Node(std::get<1>(edge));

            addNode(node_1);
            addNode(node_2);

            // Link them in their adjacency list
            link(node_1.getId(), node_2.getId());

            edges.push_back(Edge(node_1.getId(), node_2.getId()));
        }

        // for (auto node : nodes)
        // {
        //     for (auto neighbour : node.getNeighbours())
        //     {
        //         // Add edge
        //         edges.push_back(Edge(node.getId(), neighbour));
        //     }
        // }
    }

    void printNodes()
    {
        std::cout << "------ NODES ------------\n";
        for (auto &node : nodes)
        {
            node.printNode();
        }
    }
    
    void printEdges(std::string filter_label="")
    {
        std::cout << "------ EDGES ------------\n";
        for (auto &edge : edges)
        {
            edge.printEdge(filter_label);
        }
    }

    void visit(int node){
        int indx = searchNodeById(node);
        nodes[indx].visit();
    }

    void setLabel(int edge, std::string label){
        edges[edge].setLabel(label);
    }

};

void breadthFirstSearch(Graph &g, int start_node_index, std::vector<std::vector<int>> &proximity_list)
    {
        g.visit(start_node_index);

        proximity_list.clear();
        proximity_list.push_back(std::vector<int>{start_node_index});

        int index = 0;

        while (proximity_list[index].size() > 0)
        {
            std::vector<int> l_index_plus_one;
            for (int node_index : proximity_list[index])
            {
                // ...
                Node graph_node = g.nodes[g.searchNodeById(node_index)];

                if (graph_node.getLabel() != "1")
                {
                    std::cout << "Error\n";
                }

                for (int neighbour_node_int : graph_node.getNeighbours())
                {
                    Node neighbour_node = g.nodes[g.searchNodeById(neighbour_node_int)];

                    // Find the edge from node to neighbour_node
                    Edge edge = g.edges[g.searchEdgeById(node_index, neighbour_node_int)];
                    if (edge.getLabel() == "null")
                    {
                        if (neighbour_node.getLabel() == "0")
                        {
                            g.setLabel(g.searchEdgeById(node_index, neighbour_node_int), "DISCOVERY_EDGE");
                            g.visit(neighbour_node_int);
                            l_index_plus_one.push_back(neighbour_node_int);
                        }
                        else if (neighbour_node.getLabel() == "1")
                        {
                            g.setLabel(g.searchEdgeById(node_index, neighbour_node_int), "CROSS_EDGE");
                        }
                        else
                        {
                            std::cout << "Error\n";
                        }
                    }
                }
            }
            proximity_list.push_back(l_index_plus_one);
            index++;
        }
    }

void depthFirstSearch(Graph &g, int start_node_index){
    // Visit node
    g.visit(start_node_index);

    // For each neighbour node
    std::vector<int> neighbours = g.nodes[g.searchNodeById(start_node_index)].getNeighbours();
    for(int neighbour : neighbours){
        // Check the edge between them:
        int edge_index = g.searchEdgeById(start_node_index, neighbour);
        if(g.edges[edge_index].getLabel() == "null"){
            // If the neighbour is not visited, then mark the edge as DISCOVERY otherwise CROSS
            if(g.nodes[g.searchNodeById(neighbour)].getLabel() == "0"){
                g.edges[edge_index].setLabel("DISCOVERY_EDGE");
                depthFirstSearch(g, neighbour);
            }
            else if(g.nodes[g.searchNodeById(neighbour)].getLabel() == "1"){
                g.edges[edge_index].setLabel("BACK_EDGE");
            }
        }
    }
}

//...