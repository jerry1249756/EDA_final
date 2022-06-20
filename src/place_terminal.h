#ifndef PLACE_TERMINAL_H
#define PLACE_TERMINAL_H

#include "module.h"
#include "extern.h"
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>

//unordered_map<string, net*> nets

class Terminal_Placment{
    public:
        Terminal_Placment(int terminal_x, int terminal_y, int terminal_spacing);
        point transform_from_grid_to_pos(int, int);
        point transform_from_pos_to_grid(int, int);
        point find_pos(unordered_map<string, instance>& instances, unordered_map<string, net*>& nets, string netname);
        point lee_algorithm(int x, int j);
        void Thorolfsson_via_assignment(unordered_map<string, instance>& , unordered_map<string, net*>& );
        float border_width;
        float grid_height;
        float grid_width;  
        vector<vector<int>> data;
        int toptech, bottomtech;
};

#endif