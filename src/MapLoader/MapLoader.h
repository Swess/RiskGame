//
// Map loader reads and loads a map file in the .map text file format as found in the domination game sources files available at http://domination.sourceforge.net/getmaps.shtml
// Singleton
// Created by ker10 on 2019-09-27.
//
#pragma once
#include <string>
#include <vector>
#include "../Map/Map.h"


using namespace std;
namespace MapLoader {
    class MapLoader;

    /*
     * Struct to save the data in the first pass
     * */
    struct _continent {
        string *name;
        int *bonus;
        string *color;
    };

    struct _country {
        int *index;
        string *name;
        int *continentIndex;
        int *x;
        int *y;
    };

    struct _border {
        int *countryIndex;
        vector<int> *values;
    };

    /*
     * Helping class SectionsReader
     * In reality, it's a glorified enumerations of all the possible sections founded in .map files
     * Each enum should have it's own strategy
     * */
    class SectionsReader {
    public:
        enum Value {
            continent,
            countries,
            borders,
        };

        SectionsReader() = default;

        explicit SectionsReader(Value s);
        explicit operator bool() = delete;
        void strategy(const string &line, MapLoader &mapLoader);
        static SectionsReader::Value getSectionFromString(const string &s);
        static bool isStringAValidSection(const string &s);

    private:
        Value *section;
    };


    class MapLoader {
        friend struct SectionsReader;
    private:
        ifstream *input_stream;

        SectionsReader *section = nullptr;
        vector<_continent> *continents_temp;
        vector<_country> *countries_temp;
        vector<_border> *borders_temp;

        MapLoader * openFile(const string &path);
        MapLoader * openFile(const int &index);
        MapLoader * readFile();
        MapLoader * closeFile();

        void addContinentToMemory(const _continent &continent);
        void addCountryToMemory(const _country &country);
        void addBorderToMemory(const _border &border);

        Board::Map *map;
    public:
        MapLoader();
        virtual ~MapLoader();
        MapLoader * load(const string &path);
        MapLoader * load(const int &index);
        Board::Map * build();

    };

};
