//
// Map loader reads and loads a map file in the .map text file format as found in the domination game sources files available at http://domination.sourceforge.net/getmaps.shtml
// Singleton
// Created by ker10 on 2019-09-27.
//

#ifndef TEAM12_DOMINATIONGAME_MAPLOADER_H
#define TEAM12_DOMINATIONGAME_MAPLOADER_H
#include <string>
#include <vector>

using namespace std;

class MapLoader;
/*
 * Struct to save the data in the first pass
 * */
struct _continent {
    string * name;
    int * bonus;
    string * color;
    string toString() {
        return "Name: " + (*name)
        + ", bonus: "+ to_string(*bonus)
        + ", color: " + (*color);
    };
};

struct _country {
    int * index;
    string * name;
    int * continentIndex;
    int * x;
    int * y;
    string toString() {
        return "index: " + to_string(*index)
        + ", name:" + (*name)
        + ", continentIndex: " + to_string(*continentIndex)
        + ", x: " + to_string(*x)
        + ", y: " + to_string(*y);
     }
};

struct _border {
    int * countryIndex;
    vector<int> * values;
    string toString() {
        string temp;
        for (int value : *values) {
            temp += ( to_string(value) + ", " );
        }
        return "countryIndex: " + to_string(*countryIndex) + ", " +  temp;
    }
};

/*
 * Helping class Sections
 * In reality, it's a glorified enumerations of all the possible sections founded in .map files
 * Each enum should have it's own strategy
 * */
class Sections {
public:
    enum Value {
        continent,
        countries,
        borders,
    };

    Sections() = default;
    Sections(Value s);

    explicit operator bool() = delete;
    void strategy(const string &line, MapLoader &mapLoader);
    string toString();
    static Sections::Value getSectionFromString(const string& s);
    static bool isStringAValidSection(const string& s);
private:
    Value *section;
};


class MapLoader {
private:
    ifstream *input_stream;
    MapLoader closeFile();
    Sections * section = nullptr ;
    vector<_continent> * continents_temp;
    vector<_country> * countries_temp;
    vector<_border> * borders_temp;
    MapLoader openFile(const string& path);
    MapLoader readFile();
public:
    MapLoader();

    void addContinentToMemory(const _continent &continent);
    void addCountryToMemory(const _country &country);
    void addBorderToMemory(const _border &border);

    void load(const string& path);
    void build();

};

#endif //TEAM12_DOMINATIONGAME_MAPLOADER_H
