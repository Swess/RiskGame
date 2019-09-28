//
// Created by ker10 on 2019-09-28.
//

#include "MapLoader.h"
#include <fstream>
#include <string>
#include <iostream>
#include "../exceptions.h"


class MapLoaderDriver {
public:
    static void run() {

        cout << "Starting MapLoader run..." << endl;

        MapLoader mapLoader;

        string s = "RiskEurope.map";
        try {
            mapLoader.load(s);
        } catch (IOException &e) {
            cerr << e.what() << s.c_str() << endl;
        }

        cout << "Finish." << endl;

    }
};