#pragma once

#include <vector>
#include <string>

using namespace std;

struct Edge;

struct Node {
    vector<Edge *> *incidents_edges;

    int get_deg();
};

struct Edge {
    Node *a;
    Node *b;

    Edge(Node *a, Node *b);
};

class Country : public Node {
public:
    string *name;
    int *nb_armies;
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

    void insert_node(Node &new_node);

    Edge *insert_edge(Node &a, Node &b);

    bool are_adjacent(Node &a, Node &b);

    vector<Edge *> get_edges();

    vector<Node *> get_nodes();
};

class Continent : public Graph {
    static int id_count;
    int *id;
    string *name;

public:
    Continent(string *name);
};

class Map {
public:
    Graph *graph;

    void add_continent(const Graph *continent);

    void add_country(const Country *new_country);

    void connect(const Country *a, const Country *b);
};
