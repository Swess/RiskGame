#pragma once
#include <vector>
#include <string>

using namespace std;

class Edge;

struct Node {
    vector<Edge *> incidents_edges;
public:
    int get_deg();
};

struct Edge {
public:
    Node *a;
    Node *b;
    Edge(Node *a, Node *b);
};

class Country : public Node {
    string name = "";
public:
    string get_name();
};

class Graph {
protected:
    vector<Node *> nodes;
    vector<Edge *> edges;
public:
    Graph();

    void merge_subgraph(const Graph &to_merge_in);

    void insert_node(Node &new_node);

    Edge *insert_edge(Node &a, Node &b);

    bool are_adjacent(Node &a, Node &b);
};

class Continent : public Graph {
    static int id_count;
    int id;
    string name = "";
public:
    Continent(const string &name);

    string get_name();

    vector<Country *> get_countries();
};

class Map {
    Graph *graph;

};
