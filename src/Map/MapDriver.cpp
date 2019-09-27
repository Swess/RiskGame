#include <iostream>
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

            //

            return true;
        }

        bool test_continent_graph() {

        }

        bool test_composed_map() {

        }

        bool run() {
            cout << "[TEST] -----" << endl;
            cout << "[TEST] - Running Map component tests." << endl;

            bool result = test_simple_graph();

            cout << "[TEST] - End of Map component testing." << endl;
            cout << "[TEST] - Result: " << result << endl;
            cout << "[TEST] -----" << endl;
            return result;
        };
    }
}

// ======= Helpers for clarity

