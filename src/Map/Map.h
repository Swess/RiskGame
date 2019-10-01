#pragma once

#include <vector>
#include <string>

using namespace std;

namespace Map {

    class Map;

    class Country {
    public:
        int *index;
        string *name;
        int *continent_index;
        int *nb_armies;
        // TODO: Add the owner player of this country

        Country(const string &name);

        ~Country();
    };

    class Continent {
    public:
        int *size;
        int *index;
        string *name;
        Map *map;

        Continent(const string &name, Map *map);

        ~Continent();

        void insert_country(Country &country);

        int get_size();

        bool is_connected();
    };

    class Map {
        vector<Continent *> *continents;
        vector<Country *> *countries;
    public:
        vector<vector<int>> *edges;

        Map();

        ~Map();

        void add_continent(Continent *continent);

        void insert_country(Country &new_node);

        void connect(const Country &from, const Country &to);

        void biconnect(const Country &a, const Country &b);

        void connect(Continent &cont, Country &country);

        bool are_adjacent(const Country &a, const Country &b);

        bool are_adjacent(int &a, int &b);

        vector<Country *> get_countries() const;

        vector<Continent *> get_continents() const;

        bool is_connected();
    };

}