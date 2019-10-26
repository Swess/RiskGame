#pragma once

#include <vector>
#include <string>
#include <map>
// #include "../Player/Player.h"

using namespace std;

// Some Fwd declarations
namespace Player { class Player; }

namespace Board {

    class Map;
    class Continent;

    /**
     * Country class
     * Used as a Map node in the game. Holds information regarding the number of armies on the country,
     * which continent index in the map it belongs to, etc
     */
    class Country {
        friend Map;
        friend Continent;

        int *index;
        string *name;
        int *continent_index;
        int *nb_armies;
        Player::Player *owner;

    public:
        Country(const string &name);

        ~Country();

        string get_name();

        int get_armies();

        void set_armies(int num);
        Player::Player *get_owner();
        void set_owner(Player::Player* p);
        int get_index();
        string toString();
    };

    /**
     * Continent Class
     * The continents are pointed to by country to refer their grouping.
     * Continents can be considered as subgraphs in the map, but do not holds any countries, as they are
     * themselves pointing to their corresponding continent.
     */
    class Continent {
        friend Map;

        int *size;
        int *index;
        string *name;
        int *bonus;
        string *color;
        Map *map;

    public:

        Continent(const string &name, Map *map);

        Continent(const string &name, const int &bonus, const string &color, Map *map);

        ~Continent();

        /**
         * Adds a country to the map contining this continent, and make it belong to this Continent.
         * @param country
         */
        void insert_country(Country &country);

        /**
         * Get the current number of countries in the continent.
         * @return
         */
        int get_size();

        /**
         * Get the name property
         * @return
         */
        string get_name();

        /**
         * Get the current Bonus
         * @return
         */
        int get_bonus();

        /**
         * Check with Breadth First Search the connections between the countries in the continent to check it
         * the continent constitute a valid connected subgraph.
         * @return
         */
        bool is_connected();
    };

    /**
     * Map Class
     * The Map is the complete graph of all countries (Nodes). It also contains the continents and manages them.
     * The edges are implemented as an AdjacencyList with the corresponding object index to ensure O(1) seek & query.
     */
    class Map {
        friend Continent;

        vector<Continent *> *continents;
        vector<Country *> *countries;
        vector<vector<int>> *edges;

    public:

        Map();

        ~Map();

        /**
         * Adds a continent to the list of Continents in the map,
         * and will manage his destruction when the map gets destroyed.
         * @param continent
         */
        void add_continent(Continent *continent);

        /**
         * Adds a country to the list of countries in the map,
         * and will manage his destruction when the map gets destroyed.
         * @param new_node
         */
        void insert_country(Country &new_node);

        /**
         * Connects 2 map entity together.
         * That overload creates an edge in the map to link the 2 given countries with a
         * directional connection.
         * @param from
         * @param to
         */
        void connect(const Country &from, const Country &to);

        /**
         * Connects 2 map entity together.
         * That overload link a country to a given continent, removing it from his previous one if applicable.
         * @param cont
         * @param country
         */
        void connect(Continent &cont, Country &country);

        /**
         * Helper function to create a bidirectional connection between 2 countries.
         * Basically calls connect() in both directions.
         * @param a
         * @param b
         */
        void biconnect(const Country &a, const Country &b);

        /**
         * Checks if 2 countries are adjacent in the map. (Have an edge between them)
         * @param a
         * @param b
         * @return
         */
        bool are_adjacent(const Country &a, const Country &b);

        /**
         * Checks if 2 countries (given their index) are adjacent in the map. (Have an edge between them)
         * @param a
         * @param b
         * @return
         */
        bool are_adjacent(int &a, int &b);

        /**
         * finds all neighboring countries given a country.
         * @param county
         * @return vector of neighboring countries
         */
        vector<Country *> get_neighbors(Country country);

        /**
         * Get a vector of pointers to all countries present in the map.
         * @return
         */
        vector<Country *> get_countries() const;

        /**
         * Get a vector of pointers to all continents present in the map.
         * @return
         */
        vector<Continent *> get_continents() const;

        /**
         * Check with Breadth First Search if the map is connected, meaning that all countries are reachable
         * from any other countries in the graph.
         * @return
         */
        bool is_connected();


    private:
        Country *get_country_from_index(int index);
    };


}