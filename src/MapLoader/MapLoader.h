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

        // Used to delete values pointed to by the struct members.
        // Not as normal destructor as we don't always want the memory to be cleared.
        void clr_mem();
    };

    struct _country {
        int *index;
        string *name;
        int *continentIndex;
        int *x;
        int *y;

        // Used to delete values pointed to by the struct members.
        // Not as normal destructor as we don't always want the memory to be cleared.
        void clr_mem();
    };

    struct _border {
        int *countryIndex;
        vector<int> *values;

        // Used to delete values pointed to by the struct members.
        // Not as normal destructor as we don't always want the memory to be cleared.
        void clr_mem();
    };

    /*
     * Helping class LegacySectionReader (Original reader for Domination maps)
     * In reality, it'continents_buffer a glorified enumerations of all the possible sections founded in .map files
     * Each enum should have it'continents_buffer own parse
     *
     * NOTE : As the original class, left without implementing the ISectionReader
     *       voluntarily so that it is used by his Adapter wrapper.
     * */
    class LegacySectionReader {
    public:
        enum Value {
            continent,
            countries,
            borders,
        };

        LegacySectionReader();
        virtual ~LegacySectionReader();

        explicit LegacySectionReader(Value s);

        explicit operator bool() = delete;

        void strategy(const string &line, MapLoader &mapLoader);

        static LegacySectionReader::Value get_section_from_string(const string &s);

        static bool is_string_valid_section(const string &s);

    protected:
        Value *section;
    };

    ///////
    // Adapter Pattern classes for various file formats reading
    ///////
    /**
     * The target interface wanted by the client (MapLoader)
     */
    class ISectionReader {
    public:
        virtual ~ISectionReader() = default;

        virtual void parse(const string &line, MapLoader &mapLoader) = 0;

        virtual bool is_string_valid_section(const string &s) const = 0;

        virtual void flush_buffers(MapLoader &mapLoader) = 0;
    };

    /**
     * New class implementing the wanted client interface & parsing Conquest map format
    */
    class ConquestSectionReader : public ISectionReader {
    public:
        ConquestSectionReader();

        ~ConquestSectionReader() override;

        void parse(const string &line, MapLoader &mapLoader) override;

        bool is_string_valid_section(const string &s) const override;

        void flush_buffers(MapLoader &mapLoader) override;

    private:
        string* mode;
        vector<_continent> *continents_buffer;
        map<string,_country> *countries_buffer;
        vector<pair<string,string>> *borders_buffer;
    };

    /**
     * Adapter class the serves the client wanted interface for the adaptee (LegacySectionReader) that was reading the
     * Domination file format maps.
     */
    class DominationSectionReader : public ISectionReader, private LegacySectionReader {
    public:
        void parse(const string &line, MapLoader &mapLoader) override;

        bool is_string_valid_section(const string &s) const override;

        void flush_buffers(MapLoader &mapLoader) override;
    };


    class MapLoader {
    private:
        ifstream *input_stream;

        ISectionReader *section = nullptr;
        vector<_continent> *continents_temp;
        vector<_country> *countries_temp;
        vector<_border> *borders_temp;

        MapLoader *open_file(const string &path);

        MapLoader *open_file(const int &index);

        MapLoader *read_file();

        MapLoader *close_file();

        Board::Map *map;

    public:
        MapLoader();
        virtual ~MapLoader();

        MapLoader *load(const string &path);

        MapLoader *load(const int &index);

        Board::Map *build();

        void add_continent_to_memory(const _continent &continent);

        void add_country_to_memory(const _country &country);

        void add_border_to_memory(const _border &border);

    };

};
