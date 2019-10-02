//
// Created by ker10 on 2019-09-28.
//

#pragma once

namespace MapLoader {
    namespace Driver {
        bool load_Risk_Europe();
        bool load_uk();
        bool load_original_risk_map();
        bool load_a_not_correct_mapfile();
        bool load_map_with_only_continent();
        bool load_empty_file();
        bool load_undefined();
        bool run();
    }
}