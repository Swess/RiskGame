#include <iostream>
#include <cassert>
#include "Map.h"

using namespace std;

namespace Map::Driver {
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

        assert(edges[0]->opposite(*nodes[0]) == (nodes[1]));
        assert(edges[0]->opposite(*nodes[1]) == (nodes[0]));

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
        Continent continent("Continent test");

        Country *c1 = new Country("1");
        Country *c2 = new Country("2");
        Country *c3 = new Country("3");
        Country *c4 = new Country("4");

        continent.insert_country(*c1);
        continent.insert_country(*c2);
        continent.insert_country(*c3);
        continent.insert_country(*c4);

        continent.insert_edge(*c1, *c2);
        continent.insert_edge(*c1, *c3);
        continent.insert_edge(*c1, *c4);
        continent.insert_edge(*c3, *c4);

        vector<Node *> nodes = continent.get_nodes();
        assert(continent.is_connected());
        assert(continent.are_adjacent(*c1, *c2));
        assert(continent.are_adjacent(*c1, *c4));
        assert(continent.are_adjacent(*c3, *c4));
        assert(!continent.are_adjacent(*c2, *c4));

        return true;
    }

    bool test_continent_country_exclusivity() {
        Country *c1 = new Country("1");
        Country *c2 = new Country("2");
        Country *c3 = new Country("3");
        Country *c4 = new Country("4");

        Continent continent1("Empire1");
        continent1.insert_country(*c1);
        continent1.insert_country(*c2);
        continent1.insert_country(*c3);
        continent1.insert_country(*c4);
        continent1.insert_edge(*c1, *c3);
        continent1.insert_edge(*c2, *c3);

        Continent continent2("Empire2");
        // This should assign the countries to continent2, and remove them from continent1
        continent2.insert_country(*c3);
        continent2.insert_country(*c4);

        assert(continent1.get_nodes().size() == 2);
        assert(continent2.get_nodes().size() == 2);

        // Make sure the edge has also been removed
        assert(continent1.get_edges().empty());
        assert(c2->incidents_edges->empty());

        return true;
    }

    bool test_composed_map() {
        Map map;

        Continent continent1("America");
        Continent continent2("Asia");

        // Insert 5 stub country
        for (int i = 0; i < 5; i++)
            continent1.insert_country(*new Country("Stub Name"));

        for (int i = 0; i < 5; i++)
            continent2.insert_country(*new Country("Stub Name"));

        vector<Node *> c1_nodes = continent1.get_nodes();
        vector<Node *> c2_nodes = continent2.get_nodes();

        // Insert some stub edges in continents
        continent1.insert_edge(*c1_nodes[0], *c1_nodes[1]);
        continent1.insert_edge(*c1_nodes[1], *c1_nodes[2]);
        continent1.insert_edge(*c1_nodes[2], *c1_nodes[3]);
        continent1.insert_edge(*c1_nodes[3], *c1_nodes[4]);
        continent1.insert_edge(*c1_nodes[0], *c1_nodes[3]);
        continent1.insert_edge(*c1_nodes[2], *c1_nodes[4]);

        continent2.insert_edge(*c2_nodes[0], *c2_nodes[1]);
        continent2.insert_edge(*c2_nodes[1], *c2_nodes[2]);
        continent2.insert_edge(*c2_nodes[2], *c2_nodes[3]);
        continent2.insert_edge(*c2_nodes[3], *c2_nodes[4]);
        continent2.insert_edge(*c2_nodes[0], *c2_nodes[3]);
        continent2.insert_edge(*c2_nodes[2], *c2_nodes[4]);

        map.insert_continent(&continent1);
        map.insert_continent(&continent2);

        // Make stub continent links
        map.connect_continents((Country &) (*c1_nodes[2]), (Country &) (*c2_nodes[0]));
        map.connect_continents((Country &) (*c1_nodes[4]), (Country &) (*c2_nodes[4]));

        // Continent are unmodified and self contained
        // Map is now a super set of continent and edge connecting country of different continent
        assert(continent1.get_nodes().size() == 5);
        assert(continent2.get_nodes().size() == 5);
        assert(map.get_continents().size() == 2);

        // Still connected
        assert(continent1.is_connected());
        assert(continent2.is_connected());
        assert(map.is_connected());

        return true;
    }

    bool run() {
        cout << "[TEST] -----" << endl;
        cout << "[TEST] - Running Map component tests." << endl;

        test_simple_graph();
        test_continent_graph();
        test_continent_country_exclusivity();
        test_composed_map();

        cout << "[TEST] - End of Map component testing." << endl;
        cout << "[TEST] -----" << endl;
        return true;
    };
}
