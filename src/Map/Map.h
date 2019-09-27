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

    class Country : public Node {
    public:
        string *name;
        int *nb_armies;
        Continent *continent;
        // TODO: Add the owner player of this country

        Country(const string &name);

        Country(const string &name, const Continent &continent);

        virtual ~Country();
    };

    class Graph {
    protected:
        vector<Node *> *nodes;
        vector<Edge *> *edges;
    public:
        Graph();

        ~Graph();

        void insert_node(Node &new_node);

        void remove_node(Node &node);

        Edge *insert_edge(Node &a, Node &b);

        bool are_adjacent(Node &a, Node &b);

        vector<Edge *> get_edges() const;

        vector<Node *> get_nodes() const;

        bool is_connected();
    };

    class Continent : public Graph {
        static int id_count;
    public:
        int *id;
        string *name;

        Continent(const string &name);

        virtual ~Continent();

        void add_country(Country &country);

        void remove_country(Country &country);
    };

    class Map {
    public:
        Graph *graph;

        void add_continent(const Graph *continent);

        void add_country(const Country *new_country);

        void connect(const Country *a, const Country *b);
    };
}