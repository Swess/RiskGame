#include <iostream>
#include <assert.h>
#include "Map.h"

using namespace std;

namespace Map {
    namespace Driver {
        bool test_simple_graph() {
            Graph graph;

            // Insert 6 stub nodes
            for (int i = 0; i < 6; i++)
                graph.insert_node(*new Node());

            vector<Node *> nodes = graph.get_nodes();

            // Link them a specific manner
            graph.insert_edge(*nodes[0], *nodes[1]);
            graph.insert_edge(*nodes[0], *nodes[2]);
            graph.insert_edge(*nodes[1], *nodes[2]);

            graph.insert_edge(*nodes[2], *nodes[3]);
            graph.insert_edge(*nodes[3], *nodes[4]);
            graph.insert_edge(*nodes[4], *nodes[5]);
            graph.insert_edge(*nodes[3], *nodes[5]);

            vector<Edge *> edges = graph.get_edges();

            // Validate state
            assert(nodes.size() == 6);
            assert(edges.size() == 7);

            assert( edges[0]->opposite(*nodes[0]) == (nodes[1]) );
            assert( edges[0]->opposite(*nodes[1]) == (nodes[0]) );

            assert(graph.are_adjacent(*nodes[0], *nodes[1]));
            assert(graph.are_adjacent(*nodes[0], *nodes[2]));
            assert(graph.are_adjacent(*nodes[1], *nodes[2]));
            assert(graph.are_adjacent(*nodes[2], *nodes[3]));
            assert(graph.are_adjacent(*nodes[3], *nodes[4]));
            assert(graph.are_adjacent(*nodes[4], *nodes[5]));
            assert(graph.are_adjacent(*nodes[3], *nodes[5]));

            // Test some cases that should not be true
            assert(!graph.are_adjacent(*nodes[0], *nodes[5]));
            assert(!graph.are_adjacent(*nodes[0], *nodes[3]));
            assert(!graph.are_adjacent(*nodes[1], *nodes[4]));
            assert(!graph.are_adjacent(*nodes[5], *nodes[2]));

            assert(graph.is_connected());

            ////
            // None connected graph
            Graph graph2;

            // Insert 6 stub nodes
            for (int i = 0; i < 6; i++)
                graph2.insert_node(*new Node());

            nodes = graph2.get_nodes();
            graph2.insert_edge(*nodes[0], *nodes[1]);
            graph2.insert_edge(*nodes[0], *nodes[2]);

            assert(!graph2.is_connected());

            return true;
        }

        bool test_continent_graph() {
            auto* continent = new Continent("Continent test");

            Country* c1 = new Country("1");
            Country* c2 = new Country("2");
            Country* c3 = new Country("3");
            Country* c4 = new Country("4");

            continent->add_country(*c1);
            continent->add_country(*c2);
            continent->add_country(*c3);
            continent->add_country(*c4);

            continent->insert_edge(*c1, *c2);
            continent->insert_edge(*c1, *c3);
            continent->insert_edge(*c1, *c4);
            continent->insert_edge(*c3, *c4);

            delete continent;
        }

        bool test_composed_map() {

        }

        bool run() {
            cout << "[TEST] -----" << endl;
            cout << "[TEST] - Running Map component tests." << endl;

            test_simple_graph();
            test_continent_graph();
            test_composed_map();

            cout << "[TEST] - End of Map component testing." << endl;
            cout << "[TEST] -----" << endl;
            return true;
        };
    }
}
