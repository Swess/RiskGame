#include "Map.h"
#include <list>

namespace Map {

    Country::Country(const string &name) {
        this->name = new string(name);
        this->index = new int(-1);
        this->continent_index = new int(-1);
        this->nb_armies = new int;
    }

    Country::~Country() {
        delete index;
        delete name;
        delete continent_index;
        delete nb_armies;

        // Remove dangling pointers
        index = nullptr;
        name = nullptr;
        continent_index = nullptr;
        nb_armies = nullptr;
    }

    Continent::Continent(const string &name, Map *map) {
        this->index = new int(-1);
        this->size = new int(0);
        this->name = new string(name);
        this->map = map;
    }

    Continent::~Continent() {
        delete index;
        delete size;
        delete name;
    }

    void Continent::insert_country(Country &country) {
        map->insert_country(country);
        map->connect(*this, country);
    }

    bool Continent::is_connected() {
        if(*size < 2) return true;
        int current = -1;
        int visited_count = 0;

        // Finding first country in this continent
        vector<Country> countries = map->get_countries();
        for(unsigned i=0; i<countries.size(); i++){
            if(countries[i].continent_index == index)
                current = *countries[i].index;
        }

        if(current == -1) return false;     // Should never happen... Something went wrong

        vector<bool> visited(countries.size(), false);

        list<int> queue;
        visited[current] = true;
        queue.push_back(current);

        vector<int>::iterator it;
        while(!queue.empty()){
            current = queue.front();
            queue.pop_front();

            // Adjacents of current visited node
            for(it = map->edges->at(current).begin(); it!=map->edges->at(current).end(); it++){
                if(!visited[*it]){
                    visited[*it] = true;
                    queue.push_back(*it);
                    visited_count++;
                }
            }
        }

        // We visited everyone of them ?
        return visited_count == *size;
    }

    int Continent::get_size() {
        return *size;
    }


    Map::Map() {
        continents = new vector<Continent>;
        countries = new vector<Country>;
        edges = new vector<vector<int>>;
    }

    Map::~Map() {
        delete continents;
        delete countries;
        delete edges;
    }

    void Map::insert_country(Country &new_node) {
        *new_node.index = countries->size();
        countries->push_back(new_node);

        // Create edges mem loc
        edges->push_back(*new vector<int>);
    }

    bool Map::are_adjacent(const Country &a, const Country &b) {
        return are_adjacent(*a.index, *b.index);
    }

    bool Map::are_adjacent(int &a, int &b) {
        vector<int> links = edges->at(a);

        for (unsigned i = 0; i < links.size(); i++) {
            if (links.at(i) == b)
                return true;
        }
        return false;
    }

    vector<Country> Map::get_countries() const {
        return *countries;
    }

    vector<Continent> Map::get_continents() const {
        return *continents;
    }

    void Map::connect(const Country &a, const Country &b) {
        // Define direction, a to b
        edges->at(*a.index).push_back(*b.index);
    }

    void Map::connect(Continent &cont, Country &country) {
        if (*country.continent_index != -1)
            continents->at(*country.continent_index).size--;

        country.continent_index = cont.index;
        cont.size++;
    }

    void Map::biconnect(const Country &a, const Country &b) {
        connect(a, b);
        connect(b, a);
    }

    bool Map::is_connected() {
        if(countries->size() < 2) return true;

        int current = 0;
        vector<bool> visited(countries->size(), false);

        list<int> queue;
        visited[current] = true;
        queue.push_back(current);

        vector<int>::iterator it;
        while(!queue.empty()){
            current = queue.front();
            queue.pop_front();

            // Adjacents of current visited node
            for(it = edges->at(current).begin(); it!=edges->at(current).end(); it++){
                if(!visited[*it]){
                    visited[*it] = true;
                    queue.push_back(*it);
                }
            }
        }

        // Check if all visited
        for(unsigned i=0; i<visited.size(); i++){
            if(!visited[i])
                return false;
        }

        return true;
    }


}