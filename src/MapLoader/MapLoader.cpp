//
// Created by ker10 on 2019-09-27.
//

#include "MapLoader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include "../exceptions.h"

using namespace std;

// Constructors
MapLoader::MapLoader() {
    input_stream = new ifstream();
    continents_temp = new vector<_continent>(0);
    countries_temp = new vector<_country>(0);
    borders_temp = new vector<_border>(0);
}

MapLoader MapLoader::openFile(const string& path) {
    input_stream->open("ressources/maps/" + path);
    if (!input_stream->is_open()){
        throw IOException();
    }
    return *this;
}

MapLoader MapLoader::readFile() {
    string line;
    while( getline( (*input_stream) , line) ){
         if ( Sections::isStringAValidSection(line) ) {
             Sections::Value value = Sections::getSectionFromString(line);
             section = new Sections(value);
             continue; // jump to the next line
         }
         if (section != nullptr && !line.empty()){
             section->strategy(line, *this);
         }
    }
    return *this;
}

MapLoader MapLoader::closeFile() {
    input_stream->close();
    return *this;
}

void MapLoader::load(const string &path) {
    openFile(path).readFile().closeFile().build();
}

void MapLoader::addContinentToMemory(const _continent &continent) {
    this->continents_temp->emplace_back(continent);
}

void MapLoader::addCountryToMemory(const _country &country) {
    this->countries_temp->emplace_back(country);
}

void MapLoader::addBorderToMemory(const _border &border) {
    this->borders_temp->push_back(border);
}

void MapLoader::build() {
    for (int i = 0; i < this->continents_temp->size(); ++i) {
        cout << continents_temp->at(i).toString() << endl;
    }
    for (int i = 0; i < this->countries_temp->size(); ++i) {
        cout << countries_temp ->at(i).toString() << endl;
    }
    for (int i = 0; i < this->borders_temp->size(); ++i) {
        cout << borders_temp->at(i).toString() << endl;
    }
};

void Sections::strategy(const string &line, MapLoader &mapLoader) {
    char * line_c_str = const_cast<char *>(line.c_str());
    char * token = strtok(line_c_str, " ");
    switch ((*section)) {
        case continent: {
            _continent continent{nullptr, nullptr, nullptr};
            continent.name =  new string(token);
            token = strtok(nullptr, " ");
            continent.bonus = new int(strtol(token, nullptr, 10));
            token = strtok(nullptr, " ");
            continent.color = new string(token);
            mapLoader.addContinentToMemory(continent);
            break;
        }
        case countries: {
            _country country {nullptr, nullptr, nullptr, nullptr, nullptr};
            country.index = new int(strtol(token, nullptr, 10));
            token = strtok(nullptr, " ");
            country.name = new string(token);
            token = strtok(nullptr, " ");
            country.continentIndex = new int(strtol(token, nullptr, 10));
            token = strtok(nullptr, " ");
            country.x = new int(strtol(token, nullptr, 10));
            token = strtok(nullptr, " ");
            country.y = new int(strtol(token, nullptr, 10));
            mapLoader.addCountryToMemory(country);
            break;
        }
        case borders: {
            _border border{nullptr, new vector<int>(0)};
            border.countryIndex = new int(strtol(token, nullptr, 10));
            token = strtok(nullptr, " ");
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



string Sections::toString(){
    switch ((*section)) {
        case continent:
            return "[continents]";
        case countries:
            return "[countries]";
        case borders:
            return "[borders]";
        default:
            throw exception();
    }
}

Sections::Value Sections::getSectionFromString(const string &s) {
    if (s == "[continents]")
        return Value::continent;
    else if( s =="[countries]")
        return Value::countries;
    else if ( s == "[borders]")
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
