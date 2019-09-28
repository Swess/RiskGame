//
// Created by ker10 on 2019-09-27.
//

#include "MapLoader.h"
#include <fstream>
#include <string>
#include <iostream>
#include "../exceptions.h"

using namespace std;

// Constructors
MapLoader::MapLoader() {
    input_stream = new ifstream();
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
             section->strategy(line);
         }
    }
    return *this;
}

void MapLoader::closeFile() {
    input_stream->close();
}

void MapLoader::load(const string &path) {
    openFile(path).readFile().closeFile();
};

void Sections::strategy(const string &line) {
    switch ((*section)) {
        case continents:
            cout << "[continents] :" << line << endl;
            break;
        case countries:
            cout << "[countries] :" << line << endl;
            break;
        case borders:
            cout << "[borders] :" << line << endl;
            break;
        default:
            throw exception("Enum does not exists");
    }
}

string Sections::toString(){
    switch ((*section)) {
        case continents:
            return "[continents]";
        case countries:
            return "[countries]";
        case borders:
            return "[borders]";
        default:
            throw exception("Enum does not exists");
    }
}

Sections::Value Sections::getSectionFromString(const string &s) {
    if (s == "[continents]")
        return Value::continents;
    else if( s =="[countries]")
        return Value::countries;
    else if ( s == "[borders]")
        return Value::borders;
    else
        throw exception("Enum does not exists");
}

bool Sections::isStringAValidSection(const string &s) {
    return (s == "[continents]" || s == "[countries]" || s == "[borders]");
}

Sections::Sections(Sections::Value s) {
    section = new Value{s};
}
