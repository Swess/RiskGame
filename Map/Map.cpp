#include "Map.h"

//
// Edge
//
Edge::Edge(Node *a, Node *b) : a(a), b(b) {}

Node *Edge::opposite(const Node &node) {
    if( &node == a )
        return b;
    if( &node == b )
        return a;

    return nullptr;
}

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
    this->nb_armies = new int;
    incidents_edges = new vector<Edge *>;
}

Country::Country(const string &name, const Continent &continent) : Country(name) {
    this->continent = const_cast<Continent *>(&continent);
}

Country::~Country() {
    delete name;
    delete nb_armies;
    delete incidents_edges;

    // Remove dangling pointers
    name = nullptr;
    nb_armies = nullptr;
    incidents_edges = nullptr;
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

    // Add to graph list
    edges->push_back(edge_ptr);

    // Add to incidents edge list
    a.incidents_edges->push_back(edge_ptr);
    b.incidents_edges->push_back(edge_ptr);

    return edge_ptr;
}

vector<Edge *> Graph::get_edges() const {
    return *edges;
}

vector<Node *> Graph::get_nodes() const {
    return *nodes;
}

void Graph::remove_node(Node &node) {
    // Remove all edge connected to it

    // Remove from graph's nodes list

//    vector<Node *>::iterator ptr;
//    for (ptr = nodes->begin(); ptr < nodes->begin(); ptr++){
//
//    }



    // nodes->erase(index);
}

//
// Continent
//
int Continent::id_count = 0;

Continent::Continent(string *name) : name(name) {
    id = new int(++id_count);
}

void Continent::add_country(Country &country) {
    // If already part of a continent
//    if (country.continent != nullptr)
//        country.continent->remove_node(country);

    this->insert_node(country);
    country.continent = this;
}

void Continent::remove_country(Country &country) {
    this->remove_node(country);
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

void Map::add_continent(const Graph *continent) {
    vector<Edge *> edges = continent->get_edges();

    for (int i = 0; i < edges.size(); i++) {
        // Add both Nodes from the edge
        graph->insert_node(*edges[i]->a);
        graph->insert_node(*edges[i]->b);

        // Connect them
        graph->insert_edge(*edges[i]->a, *edges[i]->b);
    }

}
