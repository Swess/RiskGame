#include <iostream>
#include <cassert>
#include "Map.h"

using namespace std;

namespace Board {
    namespace Driver {
    bool test_simple_graph() {
        Map map;

        // Insert 6 stub nodes
        for (int i = 0; i < 6; i++)
            map.insert_country(*new Country("Stub name"));

        vector<Country *> countries = map.get_countries();

        // Link them a specific manner
        map.biconnect(*countries[0], *countries[1]);
        map.biconnect(*countries[0], *countries[2]);
        map.biconnect(*countries[1], *countries[2]);

        map.biconnect(*countries[2], *countries[3]);
        map.biconnect(*countries[3], *countries[4]);
        map.biconnect(*countries[4], *countries[5]);
        map.biconnect(*countries[3], *countries[5]);

        // Validate state
        vector<Country *> test = map.get_countries();
        assert(map.get_countries().size() == 6);

        assert(map.are_adjacent(*countries[0], *countries[1]));
        assert(map.are_adjacent(*countries[0], *countries[2]));
        assert(map.are_adjacent(*countries[1], *countries[2]));
        assert(map.are_adjacent(*countries[2], *countries[3]));
        assert(map.are_adjacent(*countries[3], *countries[4]));
        assert(map.are_adjacent(*countries[4], *countries[5]));
        assert(map.are_adjacent(*countries[3], *countries[5]));

        // Test some cases that should not be true
        assert(!map.are_adjacent(*countries[0], *countries[5]));
        assert(!map.are_adjacent(*countries[0], *countries[3]));
        assert(!map.are_adjacent(*countries[1], *countries[4]));
        assert(!map.are_adjacent(*countries[5], *countries[2]));

        assert(map.is_connected());

        ////
        // None connected graph
        Map map2;

        // Insert 6 stub nodes
        for (int i = 0; i < 6; i++)
            map2.insert_country(*new Country("Stub"));

        countries = map2.get_countries();
        map2.biconnect(*countries[0], *countries[1]);
        map2.biconnect(*countries[0], *countries[2]);

        assert(!map2.is_connected());

        return true;
    }

    bool test_continent_graph() {
        Map map;
        Continent continent("Continent test", &map);

        Country *c1 = new Country("1");
        Country *c2 = new Country("2");
        Country *c3 = new Country("3");
        Country *c4 = new Country("4");
        Country *c5 = new Country("5");

        // Adding to continent adds to the map also, but already connected
        continent.insert_country(*c1);
        continent.insert_country(*c2);
        continent.insert_country(*c3);
        continent.insert_country(*c4);


        map.biconnect(*c1, *c2);
        map.biconnect(*c1, *c3);
        map.biconnect(*c1, *c4);
        map.biconnect(*c3, *c4);

        // Add one extra random country
        map.insert_country(*c5);

        assert(continent.is_connected());
        assert(!map.is_connected());
        assert(map.are_adjacent(*c1, *c2));
        assert(map.are_adjacent(*c1, *c4));
        assert(map.are_adjacent(*c3, *c4));
        assert(!map.are_adjacent(*c2, *c4));

        return true;
    }

    bool test_continent_country_exclusivity() {
        Map map;

        Country *c1 = new Country("1");
        Country *c2 = new Country("2");
        Country *c3 = new Country("3");
        Country *c4 = new Country("4");

        Continent continent1("Empire1", &map);
        continent1.insert_country(*c1);
        continent1.insert_country(*c2);
        continent1.insert_country(*c3);
        continent1.insert_country(*c4);
        map.biconnect(*c1, *c3);
        map.biconnect(*c2, *c3);

        Continent continent2("Empire2", &map);
        // This should assign the countries to continent2, and remove them from continent1
        continent2.insert_country(*c3);
        continent2.insert_country(*c4);

        assert(continent1.get_size() == 2);
        assert(continent2.get_size() == 2);

        return true;
    }

    bool test_composed_map() {
        Map map;

        Continent continent1("America", &map);
        Continent continent2("Asia", &map);

        // Insert 5 stub country
        for (int i = 0; i < 5; i++)
            continent1.insert_country(*new Country("Stub Name"));

        for (int i = 0; i < 5; i++)
            continent2.insert_country(*new Country("Stub Name"));

        vector<Country *> countries = map.get_countries();

        // Insert some stub edges in continents
        // Continent1 links
        map.biconnect(*countries[0], *countries[1]);
        map.biconnect(*countries[1], *countries[2]);
        map.biconnect(*countries[2], *countries[3]);
        map.biconnect(*countries[3], *countries[4]);
        map.biconnect(*countries[0], *countries[3]);
        map.biconnect(*countries[2], *countries[4]);

        // Continent2 links
        int c1_size = continent1.get_size();
        map.biconnect(*countries[c1_size+0], *countries[c1_size+1]);
        map.biconnect(*countries[c1_size+1], *countries[c1_size+2]);
        map.biconnect(*countries[c1_size+2], *countries[c1_size+3]);
        map.biconnect(*countries[c1_size+3], *countries[c1_size+4]);
        map.biconnect(*countries[c1_size+0], *countries[c1_size+3]);
        map.biconnect(*countries[c1_size+2], *countries[c1_size+4]);

        // Make stub continent links
        map.biconnect(*countries[2], *countries[c1_size+0]);
        map.biconnect(*countries[4], *countries[c1_size+4]);

        assert(continent1.get_size() == 5);
        assert(continent2.get_size() == 5);
        assert(map.get_continents().size() == 2);

        // Still connected
        assert(continent1.is_connected());
        assert(continent2.is_connected());
        assert(map.is_connected());

        return true;
    }

    bool run() {
        test_simple_graph();
        test_continent_graph();
        test_continent_country_exclusivity();
        test_composed_map();

        return true;
    };
} }
