//
// Map loader reads and loads a map file in the .map text file format as found in the domination game sources files available at http://domination.sourceforge.net/getmaps.shtml
// Singleton
// Created by ker10 on 2019-09-27.
//

#ifndef TEAM12_DOMINATIONGAME_MAPLOADER_H
#define TEAM12_DOMINATIONGAME_MAPLOADER_H
#include <string>

using namespace std;

/*
 * Helping class Sections
 * In reality, it's a glorified enumerations of all the possible sections founded in .map files
 * Each enum should have it's own strategy
 * */
class Sections {
public:
    enum Value {
        continents,
        countries,
        borders,
    };

    Sections() = default;
    Sections(Value s);

    explicit operator bool() = delete;
    void strategy(const string &line);
    string toString();
    static Sections::Value getSectionFromString(const string& s);
    static bool isStringAValidSection(const string& s);
private:
    Value *section;
};


class MapLoader {
private:
    ifstream *input_stream;
    void closeFile();
    Sections * section = nullptr ;
    MapLoader openFile(const string& path);
    MapLoader readFile();
public:
    MapLoader();
    void load(const string& path);
    void run();

};

#endif //TEAM12_DOMINATIONGAME_MAPLOADER_H
