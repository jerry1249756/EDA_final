#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include "tech.h"

using namespace std;

int main(int argc, char* argv[]){
    fstream fin(argv[1]);
    //fstream fout(argv[2]);
    vector<tech> tech_stack;
    string trash, Tech_name, Libcell_name, pin_name, top_die_tech, bottom_die_tech;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y, die_lower_x, die_lower_y;
    int die_upper_x, die_upper_y, top_die_max_util, bottom_die_max_util;  
    int top_start_x, top_start_y, top_row_length, top_row_height, top_repeat_count;  
    int bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count;  
    int terminal_size_x, terminal_size_y, terminal_spacing;

    fin >> trash >> NumTechnologies;

    for(int i = 0; i < NumTechnologies; i++){
        vector<libcell> libcell_stack;
        fin >> trash >> Tech_name >> Num_lib_cell;
        for(int j = 0; j < Num_lib_cell; j++){
            vector<pin> pin_stack;
            fin >> trash >> Libcell_name >> lib_x >> lib_y >> Num_pin;
            for(int k = 0; k < Num_pin; k++){
                fin >> trash >> pin_name >> pin_x >> pin_y;
                pin cell_pin = {pin_name, pin_x, pin_y};
                pin_stack.push_back(cell_pin);
            }
            libcell cell = {Libcell_name, lib_x, lib_y, pin_stack};
            libcell_stack.push_back(cell);
        }
        tech technology = {Tech_name, libcell_stack};
        tech_stack.push_back(technology);
    }

    fin >> trash >> die_lower_x >> die_lower_y >> die_upper_x >> die_upper_y;
    die_size die = {die_lower_x, die_lower_y, die_upper_x, die_upper_y};
    
    fin >> top_die_max_util >> bottom_die_max_util;
    fin >> top_start_x >> top_start_y >> top_row_length >> top_row_height >> top_repeat_count;
    fin >> bottom_start_x >> bottom_start_y >> bottom_row_length >> bottom_row_height >> bottom_repeat_count;
    die_rows top_die_rows = {top_start_x, top_start_y, top_row_height, top_row_height, top_repeat_count};
    die_rows bottom_die_rows = {bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count};

    fin >> trash >> top_die_tech >> bottom_die_tech;

    fin >> trash >> terminal_size_x >> terminal_size_y;
    fin >> trash >> terminal_spacing;

    fin.close();
    return 0;
}