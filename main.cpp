#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include "module.h"
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]){
    fstream fin(argv[1]);
    //fstream fout(argv[2]);
    vector<tech> tech_stack;
    unordered_map<string, string> instances;
    unordered_map<string, net> nets;
    string trash, Tech_name, Libcell_name, pin_name, top_die_tech, bottom_die_tech, instance_name, net_name;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y, die_lower_x, die_lower_y;
    int die_upper_x, die_upper_y, top_die_max_util, bottom_die_max_util;  
    int top_start_x, top_start_y, top_row_length, top_row_height, top_repeat_count;  
    int bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count;  
    int terminal_size_x, terminal_size_y, terminal_spacing;
    int Num_instance, Num_net, Num_net_pin;

    fin >> trash >> NumTechnologies;

    for(int i = 0; i < NumTechnologies; i++){
        unordered_map<string,libcell> libcells;
        fin >> trash >> Tech_name >> Num_lib_cell;
        for(int j = 0; j < Num_lib_cell; j++){
            vector<pin> pin_stack;
            fin >> trash >> Libcell_name >> lib_x >> lib_y >> Num_pin;
            for(int k = 0; k < Num_pin; k++){
                fin >> trash >> pin_name >> pin_x >> pin_y;
                point pin_point(pin_x,pin_y);
                pin cell_pin = {pin_name, pin_point};
                pin_stack.push_back(cell_pin);
            }
            libcell cell(lib_x, lib_y);
            cell.pins = pin_stack;
            cell.cell_name = Libcell_name;
            libcells[Libcell_name] = cell;
        }
        tech technology = {Tech_name, libcells};
        tech_stack.push_back(technology);
    }

    fin >> trash >> die_lower_x >> die_lower_y >> die_upper_x >> die_upper_y;
    die_size die = {die_lower_x, die_lower_y, die_upper_x, die_upper_y};
    
    fin >> trash >> top_die_max_util >> trash >> bottom_die_max_util;
    fin >> trash >> top_start_x >> top_start_y >> top_row_length >> top_row_height >> top_repeat_count;
    fin >> trash >> bottom_start_x >> bottom_start_y >> bottom_row_length >> bottom_row_height >> bottom_repeat_count;
    die_rows top_die_rows = {top_start_x, top_start_y, top_row_height, top_row_height, top_repeat_count};
    die_rows bottom_die_rows = {bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count};

    fin >> trash >> top_die_tech >> trash >> bottom_die_tech;

    fin >> trash >> terminal_size_x >> terminal_size_y;
    fin >> trash >> terminal_spacing;

    fin >> trash >> Num_instance;
    for(int i = 0; i < Num_instance; i++){
        fin >> trash >> instance_name >> Libcell_name;
        instances[instance_name] = Libcell_name;
    }
    
    fin >> trash >> Num_net;
    for(int i = 0; i < Num_net; i++){
        vector<string> instances;
        vector<string> pins;
        fin >> trash >> net_name >> Num_net_pin;
        for(int j = 0; j < Num_net_pin; j++){
            
            string temp;
            fin >> trash >> temp;
            string temp2 = "/";
            vector<string> temp3 = split(temp, temp2);
            instance_name = temp3[0];
            pin_name = temp3[1];
            instances.push_back(instance_name);
            pins.push_back(pin_name);
        }
        net net;
        net.Net_name = net_name;
        net.instance = instances;
        net.pin = pins;
        nets[net_name] = net;
    }

    cout << nets["N5"].pin[0] << endl;

    fin.close();
    return 0;
}