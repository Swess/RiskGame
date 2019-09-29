#include "Map.h"
#include <iostream>
#include <list>
#include <algorithm>

namespace Map {
    //
    // Edge
    //
    Edge::Edge(Node *a, Node *b) : a(a), b(b) {}

    Node *Edge::opposite(const Node &node) {
        if (&node == a)
            return b;
        if (&node == b)
            return a;

        return nullptr;
    }

    Edge::~Edge() {
        a = nullptr;
        b = nullptr;
    }

    ContinentalEdge::ContinentalEdge(Node *a, Node *b, Continent *c1, Continent *c2) : Edge(a, b), c1(c1), c2(c2) {}

    Continent *ContinentalEdge::opposite(const Continent &continent) {
        if (&continent == c1)
            return c2;
        if (&continent == c2)
            return c1;

        return nullptr;
    }


    //
    // Node
    //
    int Node::get_deg() {
        return incidents_edges->size();
    }

    Node::Node() { incidents_edges = new vector<Edge *>; }

    Node::~Node() {
        delete incidents_edges;
    }


    //
    // Country
    //
    Country::Country(const string &name) {
        this->name = new string(name);
        this->nb_armies = new int;
        incidents_edges = new vector<Edge *>;
        this->continent = nullptr;
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
        Node *testing = &a;
        Node *searching = &b;

        // Iterate only for lowest degree node
        if (b.get_deg() < a.get_deg()) {
            testing = &b;
            searching = &a;
        }

        // Check all incidences
        for (int i = 0; i < testing->get_deg(); i++) {
            Edge *e = testing->incidents_edges->at(i);

            // Comparing pointers
            if (e->opposite(*testing) == searching)
                return true;
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

    bool Graph::remove_node(Node &node) {
        // Find it
        vector<Node *>::iterator it = find(nodes->begin(), nodes->end(), &node);
        if (it != nodes->end()) {

            // Remove all edge connected to it
            for (Edge *edge : *node.incidents_edges)
                this->remove_edge(*edge);

            nodes->erase(it);
            return true;
        }
        return false;
    }

    bool Graph::remove_edge(Edge &edge) {
        // Freaking ugly.. but no choices for now... all vectors should have been a custom linked list...
        vector<Edge *>::iterator it = find(edges->begin(), edges->end(), &edge);
        if (it != edges->end()) {
            vector<Edge *> *incA = edge.a->incidents_edges;
            vector<Edge *> *incB = edge.b->incidents_edges;

            incA->erase(find(incA->begin(), incA->end(), &edge));
            incB->erase(find(incB->begin(), incB->end(), &edge));

            delete &edge;
            edges->erase(it);
            return true;
        }
        return false;
    }

    Graph::~Graph() {
        for (auto &node : *nodes)
            delete node;

        for (auto &edge : *edges)
            delete edge;

        delete nodes;
        delete edges;
    }

    bool Graph::is_connected() {
        // BFS to check if we are able to visit each nodes
        int size = nodes->size();
        if (size < 2) return true;

        Node *current = nodes->front();
        vector<Node *> *visited = new vector<Node *>;

        list<Node *> queue;
        visited->insert(visited->begin(), current);
        queue.push_back(current);

        // Iterate
        vector<Edge *>::iterator i;
        while (!queue.empty()) {
            current = queue.front();
            queue.pop_front();

            for (i = current->incidents_edges->begin(); i != current->incidents_edges->end(); i++) {
                Node *opposite = (*i)->opposite(*current);

                // If not visited
                if (opposite && find(visited->begin(), visited->end(), opposite) == visited->end()) {
                    visited->push_back(opposite);
                    queue.push_back(opposite);
                }
            }
        }

        return nodes->size() == visited->size();
    }

    //
    // Continent
    //
    Continent::Continent(const string &name) {
        incidents_edges = new vector<ContinentalEdge *>;
        this->name = new string(name);
    }

    void Continent::insert_country(Country &country) {
        // If already part of a continent
        if (country.continent)
            country.continent->remove_node(country);

        this->insert_node(country);
        country.continent = this;
    }

    void Continent::remove_country(Country &country) {
        this->remove_node(country);
    }

    Continent::~Continent() {
        delete incidents_edges;
        delete name;
    }

    bool Continent::is_connected() {
        // BFS to check if we are able to visit each nodes of the continent only
        int size = nodes->size();
        if (size < 2) return true;

        Node *current = nodes->front();
        vector<Node *> *visited = new vector<Node *>;

        list<Node *> queue;
        visited->insert(visited->begin(), current);
        queue.push_back(current);

        // Iterate
        vector<Edge *>::iterator i;
        while (!queue.empty()) {
            current = queue.front();
            queue.pop_front();

            for (i = current->incidents_edges->begin(); i != current->incidents_edges->end(); i++) {
                Country *opposite = (Country *)(*i)->opposite(*current);

                // If not visited (Continent only)
                if (opposite && opposite->continent == this &&
                    find(visited->begin(), visited->end(), opposite) == visited->end()) {
                    visited->push_back(opposite);
                    queue.push_back(opposite);
                }
            }
        }

        return nodes->size() == visited->size();
    }


    //
    // Map
    //
    void Map::insert_continent(Continent *continent) {
        continents->push_back(continent);
    }

    Map::Map() {
        continents = new vector<Continent *>;
        edges = new vector<ContinentalEdge *>;
    }

    Map::~Map() {
        for (auto &edge : *edges)
            delete edge;

        delete continents;
        delete edges;
    }

    ContinentalEdge* Map::connect_continents(Country &country1, Country &country2) {
        if( !country1.continent || !country2.continent ){
            cout << "Could not connect countries since one/both of them is not part of a continent." << endl;
            return nullptr;
        }

        if( country1.continent == country2.continent ){
            cout << "Cannot connect 2 countries that are part of the same continent. Use the Continent Graph itself." << endl;
            return nullptr;
        }

        if(
            find(continents->begin(), continents->end(), country1.continent) == continents->end()
            || find(continents->begin(), continents->end(), country2.continent) == continents->end()
        ){
            cout << "Cannot connect countries which continent is not part of the map." << endl;
            return nullptr;
        }

        auto *edge_ptr = new ContinentalEdge(&country1, &country2, country1.continent, country2.continent);

        // Add to graph list
        edges->push_back(edge_ptr);

        // Add to incidents edge list
        country1.incidents_edges->push_back(edge_ptr);
        country1.continent->incidents_edges->push_back(edge_ptr);
        country2.incidents_edges->push_back(edge_ptr);
        country2.continent->incidents_edges->push_back(edge_ptr);

        return edge_ptr;
    }

    bool Map::is_connected() {
        // Check if all continent are connected themselves internally
        for(int i=0; i<continents->size(); i++){
            if (!continents->at(i)->is_connected())
                return false;
        }

        // Check if all continents are interconnected, using ContinentalEdges
        // BFS to check if we are able to visit each continent
        int size = continents->size();
        if (size < 2) return true;

        Continent *current = continents->front();
        auto *visited = new vector<Continent *>;

        list<Continent *> queue;
        visited->insert(visited->begin(), current);
        queue.push_back(current);

        // Iterate
        vector<ContinentalEdge *>::iterator i;
        while (!queue.empty()) {
            current = queue.front();
            queue.pop_front();

            for (i = current->incidents_edges->begin(); i != current->incidents_edges->end(); i++) {
                Continent *opposite = (*i)->opposite(*current);

                // If not visited
                if (opposite && find(visited->begin(), visited->end(), opposite) == visited->end()) {
                    visited->push_back(opposite);
                    queue.push_back(opposite);
                }
            }
        }

        return continents->size() == visited->size();
    }

    vector<Continent *> Map::get_continents() {
        return *continents;
    }


}