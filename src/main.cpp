#include <fstream>
#include "FM_alg.h"

using namespace std;

vector<tech> tech_stack;
int die_area;
int top_die_max_util, bottom_die_max_util;

int main(int argc, char* argv[]){
    //read file
    fstream fin(argv[1]);
    //fstream fout(argv[2]);
    
    unordered_map<string, instance> instances;
    unordered_map<string, net> nets;
    string trash, Tech_name, Libcell_name, pin_name, top_die_tech, bottom_die_tech, instance_name, net_name;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y, die_lower_x, die_lower_y;
    int die_upper_x, die_upper_y;
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
        instance c(Libcell_name);
        instances[instance_name] = c;
    }
    
    fin >> trash >> Num_net;
    for(int i = 0; i < Num_net; i++){
        vector<string> instances;
        vector<string> pins;
        fin >> trash >> net_name >> Num_net_pin;
        net net;
        net.Net_name = net_name;
        for(int j = 0; j < Num_net_pin; j++){
            ip temp_pair;
            string temp;
            fin >> trash >> temp;
            int temp2 = split1(temp);
            string c = temp.substr(0,temp2);
            temp.erase(0,temp2+1);
            temp_pair.INSTANCE = c;
            temp_pair.PIN = temp;
            net.net_pin.push_back(temp_pair);
        }
        nets[net_name] = net;
    }
    //read file finish

    //partition
    vector<cell_node> nodes;
    for(auto it = instances.begin(); it != instances.end(); ++it){
        cell_node C(it->first, (it->second).libcell_type);
        nodes.push_back(C);
    }

    vector<partition_net*> n;
    for(auto it = nets.begin(); it != nets.end(); ++it){
        partition_net net1(it->first);
        for(auto it1 = nodes.begin(); it1 != nodes.end(); ++it1){
            for(auto it2 = (it->second).net_pin.begin(); it2 != (it->second).net_pin.end(); ++it2){
                if(it2->INSTANCE == it1->node_name){
                    net1.add_node(&(*it1));
                }
            }
        }
        n.push_back(&net1);
    }
    die_area = (die_upper_x - die_lower_x) * (die_upper_y - die_lower_y);
    FM_algorithm(nodes,n);

    fin.close();
    return 0;
}