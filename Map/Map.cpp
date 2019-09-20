#include "Map.h"

//
// Edge
//
Edge::Edge(Node *a, Node *b) : a(a), b(b) {}

//
// Node
//
int Node::get_deg() {
    return incidents_edges.size();
}


//
// Country
//
string Country::get_name() {
    return name;
}

//
// Graph
//
Graph::Graph() {
    edges.reserve(10);
    nodes.reserve(10);
}

bool Graph::are_adjacent(Node &a, Node &b) {
    Node testing = a;
    Node searching = b;

    // Iterate only for lowest degree node
    if(b.get_deg() < a.get_deg()){
        testing = b;
        searching = a;
    }

    // Check all incidences
    for(int i=0; i<testing.get_deg(); i++){
        Edge* e = testing.incidents_edges.at(i);

        // Comparing pointers
        if(e->a == &searching || e->b == &searching){
            return true;
        }
    }

    return false;
}

void Graph::insert_node(Node &new_node) {
    Node* node_ptr = &new_node;
    nodes.push_back(node_ptr);
}

Edge* Graph::insert_edge(Node &a, Node &b) {
    Edge* edge_ptr = new Edge(&a, &b);
    edges.push_back(edge_ptr);
    return edge_ptr;
}

//
// Continent
//
int Continent::id_count = 0;
string Continent::get_name() {
    return name;
}

Continent::Continent(const string &name) : name(name) {
    id = ++id_count;
}
