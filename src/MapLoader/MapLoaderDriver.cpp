//
// Created by ker10 on 2019-09-28.
//

#include "MapLoader.h"
#include "MapLoaderDriver.h"
#include "../Map/Map.h"
#include <string>
#include <iostream>
#include "../exceptions.h"

namespace MapLoader::Driver {
    bool run() {

        cout << "Starting MapLoader run..." << endl;

        MapLoader mapLoader;

        string s = "RiskEurope.map";
        Map::Map * testMap = nullptr;
        try {
            testMap = mapLoader.load(s).build();
        } catch (IOException &e) {
            cerr << e.what() << s.c_str() << endl;
        }

        cout << *testMap->get_continents().at(0)->name << endl;
        cout << *testMap->get_countries().at(0)->name << endl;

        return true;
    }
}