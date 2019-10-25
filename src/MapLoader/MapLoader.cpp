//
// Created by ker10 on 2019-09-27.
//

#include "MapLoader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <windows.h>
#include <stdexcept>
#include "../Map/Map.h"
#include "../exceptions.h"
#include "../Terminal/Terminal.h"

namespace MapLoader {

    const string map_path = "ressources/maps/";

    // Constructors
    MapLoader::MapLoader() {
        input_stream = new ifstream();
        continents_temp = new vector<_continent>(0);
        countries_temp = new vector<_country>(0);
        borders_temp = new vector<_border>(0);
        map = new Map::Map();
    }

    //Deconstructors
    MapLoader::~MapLoader() {
        delete input_stream;
        delete continents_temp;
        delete countries_temp;
        delete borders_temp;

        input_stream = nullptr;
        continents_temp = nullptr;
        countries_temp = nullptr;
        borders_temp = nullptr;
    };

    //Methods
    MapLoader * MapLoader::openFile(const string &path) {
        Terminal::debug("Opening file." + path);
        input_stream->open(map_path + path);
        if (!input_stream->is_open()) {
            throw IOException();
        }
        return this;
    }

    MapLoader * MapLoader::openFile(const int &index) {
        Terminal::debug("Finding file...");
        int curr_index = 0;
        string search_path = map_path + "/*.*";
        WIN32_FIND_DATA fd;
        HANDLE h_find = FindFirstFile(search_path.c_str(), &fd);
        if(h_find != INVALID_HANDLE_VALUE) {
            do {
                if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                    if (index == curr_index++) {
                        Terminal::debug("File found.");
                        FindClose(h_find);
                        Terminal::debug(fd.cFileName);
                        return openFile(fd.cFileName);
                    }
                }
            } while(FindNextFile(h_find, &fd) );
            // Index out of bound
            FindClose(h_find);
        }

        return nullptr;
    }

    MapLoader * MapLoader::readFile() {
        string line;
        while (getline((*input_stream), line)) {
            if (Sections::isStringAValidSection(line)) {
                Sections::Value value = Sections::getSectionFromString(line);
                section = new Sections(value);
                continue; // jump to the next line
            }
            if (section != nullptr && !line.empty()) {
                section->strategy(line, *this);
            }
        }
        return this;
    }

    MapLoader * MapLoader::closeFile() {
        input_stream->close();
        return this;
    }

    MapLoader * MapLoader::load(const string &path) {
        return openFile(path)->readFile()->closeFile();
    }

    MapLoader * MapLoader::load(const int &index) {
        return openFile(index)->readFile()->closeFile();
    }

    void MapLoader::addContinentToMemory(const _continent &continent) {
        this->continents_temp->push_back(continent);
    }

    void MapLoader::addCountryToMemory(const _country &country) {
        this->countries_temp->push_back(country);
    }

    void MapLoader::addBorderToMemory(const _border &border) {
        this->borders_temp->push_back(border);
    }

    Map::Map * MapLoader::build() {
        if ( continents_temp->empty() ){
            string msg = "There is no continents in the provided file";
            throw runtime_error(msg);
        }
        if ( countries_temp->empty()  ){
            string msg = "There is no country in the provided file";
            throw runtime_error(msg);
        }
        if ( borders_temp->empty()  ){
            string msg = "There is no borders in the provided file";
            throw runtime_error(msg);
        }
        for ( _continent continent : *continents_temp) {
            new Map::Continent(*continent.name, *continent.bonus, *continent.color, map);
        }
        for (_country country : *countries_temp) {
            Map::Country * new_country = new Map::Country(*country.name);
            Map::Continent * continent = map->get_continents().at( (*country.continentIndex) - 1 ); // Indexing in files start at 1
            continent->insert_country(*new_country);
        }
        for (_border border : *borders_temp) {
            Map::Country * country = map->get_countries().at( (*border.countryIndex) -1 );
            for (int value : *border.values){
                map->connect( *country,  *map->get_countries().at( (value - 1) )); // Indexing in files start at 1
            }
        }
        return map;
    }

    void Sections::strategy(const string &line, MapLoader &mapLoader) {
        char *line_c_str = const_cast<char *>(line.c_str());
        char *token = strtok(line_c_str, " ");
        const string FILE_ERROR = "Invalid file format";
        if (token == nullptr) throw invalid_argument( FILE_ERROR );
        switch ((*section)) {
            case continent: {
                _continent continent{nullptr, nullptr, nullptr};
                continent.name = new string(token);
                token = strtok(nullptr, " ");

                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                continent.bonus = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                continent.color = new string(token);
                mapLoader.addContinentToMemory(continent);
                break;
            }
            case countries: {
                _country country{nullptr, nullptr, nullptr, nullptr, nullptr};
                country.index = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                country.name = new string(token);
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                country.continentIndex = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                country.x = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );

                country.y = new int(strtol(token, nullptr, 10));
                mapLoader.addCountryToMemory(country);
                break;
            }
            case borders: {
                _border border{nullptr, new vector<int>(0)};
                border.countryIndex = new int(strtol(token, nullptr, 10));

                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument( FILE_ERROR );
                while (token != nullptr) {
                    border.values->push_back(strtol(token, nullptr, 10));
                    token = strtok(nullptr, " ");
                }
                mapLoader.addBorderToMemory(border);
                break;
            }
            default:
                throw exception();
        }
    }

    Sections::Value Sections::getSectionFromString(const string &s) {
        if (s == "[continents]")
            return Value::continent;
        else if (s == "[countries]")
            return Value::countries;
        else if (s == "[borders]")
            return Value::borders;
        else
            throw exception();
    }

    bool Sections::isStringAValidSection(const string &s) {
        return (s == "[continents]" || s == "[countries]" || s == "[borders]");
    }

    Sections::Sections(Sections::Value s) {
        section = new Value{s};
    }

}
