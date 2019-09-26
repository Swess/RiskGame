#include "Map.h"

//
// Edge
//
Edge::Edge(Node *a, Node *b) : a(a), b(b) {}

//
// Node
//
int Node::get_deg() {
    return incidents_edges->size();
}


//
// Country
//
Country::Country(const string &name) {
    this->name = new string(name);
    incidents_edges = new vector<Edge *>;
}

Country::~Country() {
    delete this->name;
    delete this->nb_armies;

    for (int i = 0; i < incidents_edges->size(); ++i) {
        // Delete each edges
    }

    // Remove dangling pointers
    this->name = nullptr;
    this->nb_armies = nullptr;
}


//
// Graph
//
Graph::Graph() {
    edges = new vector<Edge *>;
    nodes = new vector<Node *>;

    edges->reserve(10);
    nodes->reserve(10);
}

bool Graph::are_adjacent(Node &a, Node &b) {
    Node testing = a;
    Node searching = b;

    // Iterate only for lowest degree node
    if (b.get_deg() < a.get_deg()) {
        testing = b;
        searching = a;
    }

    // Check all incidences
    for (int i = 0; i < testing.get_deg(); i++) {
        Edge *e = testing.incidents_edges->at(i);

        // Comparing pointers
        if (e->a == &searching || e->b == &searching) {
            return true;
        }
    }

    return false;
}

void Graph::insert_node(Node &new_node) {
    Node *node_ptr = &new_node;
    nodes->push_back(node_ptr);
}

Edge *Graph::insert_edge(Node &a, Node &b) {
    Edge *edge_ptr = new Edge(&a, &b);
    edges->push_back(edge_ptr);
    return edge_ptr;
}

vector<Edge *> Graph::get_edges() {
    return *edges;
}

vector<Node *> Graph::get_nodes() {
    return *nodes;
}

//
// Continent
//
int Continent::id_count = 0;

Continent::Continent(string *name) : name(name) {
    id = new int(++id_count);
}


//
// Map
//
void Map::add_country(const Country *new_country) {
    graph->insert_node((Node &) *new_country);
}

void Map::connect(const Country *a, const Country *b) {
    graph->insert_edge((Node &) *a, (Node &) *b);
}
