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
        map = new Board::Map();
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
    MapLoader *MapLoader::open_file(const string &path) {
        Terminal::debug("Opening file." + path);
        input_stream->open(map_path + path);
        if (!input_stream->is_open()) {
            throw IOException();
        }
        return this;
    }

    MapLoader *MapLoader::open_file(const int &index) {
        Terminal::debug("Finding file...");
        int curr_index = 0;
        string search_path = map_path + "/*.*";
        WIN32_FIND_DATA fd;
        HANDLE h_find = FindFirstFile(search_path.c_str(), &fd);
        if (h_find != INVALID_HANDLE_VALUE) {
            do {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    if (index == curr_index++) {
                        Terminal::debug("File found.");
                        FindClose(h_find);
                        Terminal::debug(fd.cFileName);
                        return open_file(fd.cFileName);
                    }
                }
            } while (FindNextFile(h_find, &fd));
            // Index out of bound
            FindClose(h_find);
        }

        return nullptr;
    }

    MapLoader *MapLoader::read_file() {
        string line;
        ISectionReader *reader;

        if (line == "[Map]")
            reader = new ConquestSectionReader();
        else
            reader = new DominationSectionReader();

        while (getline((*input_stream), line)) {
            if (!line.empty()) {
                try {
                    reader->parse(line, *this);
                } catch (invalid_argument &e) {
                    throw e;
                }
            }
        }

        delete reader;  // Avoid memory leak
        return this;
    }

    MapLoader *MapLoader::close_file() {
        input_stream->close();
        return this;
    }

    MapLoader *MapLoader::load(const string &path) {
        return open_file(path)->read_file()->close_file();
    }

    MapLoader *MapLoader::load(const int &index) {
        return open_file(index)->read_file()->close_file();
    }

    void MapLoader::add_continent_to_memory(const _continent &continent) {
        this->continents_temp->push_back(continent);
    }

    void MapLoader::add_country_to_memory(const _country &country) {
        this->countries_temp->push_back(country);
    }

    void MapLoader::add_border_to_memory(const _border &border) {
        this->borders_temp->push_back(border);
    }

    Board::Map *MapLoader::build() {
        if (continents_temp->empty()) {
            string msg = "There is no continents in the provided file";
            throw runtime_error(msg);
        }
        if (countries_temp->empty()) {
            string msg = "There is no country in the provided file";
            throw runtime_error(msg);
        }
        if (borders_temp->empty()) {
            string msg = "There is no borders in the provided file";
            throw runtime_error(msg);
        }
        for (_continent continent : *continents_temp) {
            new Board::Continent(*continent.name, *continent.bonus, *continent.color, map);
        }
        for (_country country : *countries_temp) {
            Board::Country *new_country = new Board::Country(*country.name);
            Board::Continent *continent = map->get_continents().at(
                    (*country.continentIndex) - 1); // Indexing in files start at 1
            continent->insert_country(*new_country);
        }
        for (_border border : *borders_temp) {
            Board::Country *country = map->get_countries().at((*border.countryIndex) - 1);
            for (int value : *border.values) {
                map->connect(*country, *map->get_countries().at((value - 1))); // Indexing in files start at 1
            }
        }
        map->set_country_neighbors();

        // All values pointed at by struct has now been used.
        // Therefore, clearing memory in heap
        for(_continent c : *continents_temp)
            c.clr_mem();
        for(_country c : *countries_temp)
            c.clr_mem();
        for(_border b : *borders_temp)
            b.clr_mem();


        return map;
    }

    void LegacySectionReader::strategy(const string &line, MapLoader &mapLoader) {
        char *line_c_str = const_cast<char *>(line.c_str());
        char *token = strtok(line_c_str, " ");
        const string FILE_ERROR = "Invalid file format";
        if (token == nullptr) throw invalid_argument(FILE_ERROR);
        switch ((*section)) {
            case continent: {
                _continent continent{nullptr, nullptr, nullptr};
                continent.name = new string(token);
                token = strtok(nullptr, " ");

                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                continent.bonus = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                continent.color = new string(token);
                mapLoader.add_continent_to_memory(continent);
                break;
            }
            case countries: {
                _country country{nullptr, nullptr, nullptr, nullptr, nullptr};
                country.index = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                country.name = new string(token);
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                country.continentIndex = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                country.x = new int(strtol(token, nullptr, 10));
                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);

                country.y = new int(strtol(token, nullptr, 10));
                mapLoader.add_country_to_memory(country);
                break;
            }
            case borders: {
                _border border{nullptr, new vector<int>(0)};
                border.countryIndex = new int(strtol(token, nullptr, 10));

                token = strtok(nullptr, " ");
                if (token == nullptr) throw invalid_argument(FILE_ERROR);
                while (token != nullptr) {
                    border.values->push_back(strtol(token, nullptr, 10));
                    token = strtok(nullptr, " ");
                }
                mapLoader.add_border_to_memory(border);
                break;
            }
            default:
                throw runtime_error("Map parsing section not set.");
        }
    }

    LegacySectionReader::Value LegacySectionReader::get_section_from_string(const string &s) {
        if (s == "[continents]")
            return Value::continent;
        else if (s == "[countries]")
            return Value::countries;
        else if (s == "[borders]")
            return Value::borders;
        else
            throw exception();
    }

    bool LegacySectionReader::is_string_valid_section(const string &s) {
        return (s == "[continents]" || s == "[countries]" || s == "[borders]");
    }

    LegacySectionReader::LegacySectionReader(LegacySectionReader::Value s) {
        section = new Value{s};
    }

    LegacySectionReader::~LegacySectionReader() {
        delete section;
    }

    LegacySectionReader::LegacySectionReader() {
        section = (Value*)new int(-1);
    }

    bool DominationSectionReader::is_string_valid_section(const string &s) const {
        return LegacySectionReader::is_string_valid_section(s);
    }

    void DominationSectionReader::parse(const string &line, MapLoader &mapLoader) {
        // Checking if we are in entering a new section
        try {
            Value v = LegacySectionReader::get_section_from_string(line);
            delete LegacySectionReader::section;    // Avoid leak from older value
            LegacySectionReader::section = new Value{v};
            return;
        } catch (...) {
            // Muting the exception. Not new section simply...
        }

        if(*LegacySectionReader::section != -1)
            LegacySectionReader::strategy(line, mapLoader);
    }

    void ConquestSectionReader::parse(const string &line, MapLoader &mapLoader) {
        // TODO

    }

    bool ConquestSectionReader::is_string_valid_section(const string &s) const {
        return (s == "[Territories]" || s == "[Continents]");
    }

    ConquestSectionReader::ConquestSectionReader() {
        continent_buffer = new vector<_continent>();
    }

    ConquestSectionReader::~ConquestSectionReader() {
        delete continent_buffer;
    }

    void _continent::clr_mem() {
        delete name;
        delete bonus;
        delete color;
    }

    void _country::clr_mem() {
        delete index;
        delete name;
        delete continentIndex;
        delete x;
        delete y;
    }

    void _border::clr_mem() {
        delete countryIndex;
        delete values;
    }
}
