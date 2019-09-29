#pragma once

#include <vector>
#include <string>

using namespace std;

namespace Map {
    struct Edge;

    class Continent;

    struct Node {
        vector<Edge *> *incidents_edges;

        Node();

        ~Node();

        int get_deg();
    };

    struct Edge {
        Node *a;
        Node *b;

        Edge(Node *a, Node *b);

        ~Edge();

        Node *opposite(const Node &node);
    };

    class ContinentalEdge : public Edge {
        Continent *c1, *c2;
    public:
        ContinentalEdge(Node *a, Node *b, Continent *c1, Continent *c2);

        Continent *opposite(const Continent &continent);
    };

    class Country : public Node {
    public:
        string *name;
        int *nb_armies;
        Continent *continent;
        // TODO: Add the owner player of this country

        Country(const string &name);

        virtual ~Country();
    };

    class Graph {
    protected:
        vector<Node *> *nodes;
        vector<Edge *> *edges;
    public:
        Graph();

        virtual ~Graph();

        void insert_node(Node &new_node);

        bool remove_node(Node &node);

        Edge *insert_edge(Node &a, Node &b);

        bool remove_edge(Edge &edge);

        bool are_adjacent(Node &a, Node &b);

        vector<Edge *> get_edges() const;

        vector<Node *> get_nodes() const;

        virtual bool is_connected();
    };

    class Continent : public Graph {
    private:
        // Name hiding specific method. Should be using insert_country instead
        using Graph::insert_node;
    public:

        vector<ContinentalEdge *> *incidents_edges;
        string *name;

        Continent(const string &name);

        virtual ~Continent();

        void insert_country(Country &country);

        void remove_country(Country &country);

        bool is_connected() override;
    };

    class Map {
        vector<Continent *> *continents;
        vector<ContinentalEdge *> *edges;
    public:
        Map();

        ~Map();

        void insert_continent(Continent *continent);

        ContinentalEdge *connect_continents(Country &country1, Country &country2);

        vector<Continent *> get_continents();

        bool is_connected();
    };
}