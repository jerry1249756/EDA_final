#include <fstream>
#include "FM_alg.h"
#include "module.h"
//#include "Kraftwerk2.h"
using namespace std;

vector<tech> tech_stack; //tech_stack[0] is TA; tech_stack[1] is TB if it exists.
unsigned long long int die_area;
unsigned int top_die_max_util, bottom_die_max_util;
unsigned int top_repeat_count, bottom_repeat_count; 

int main(int argc, char* argv[]){
    //read file
    fstream fin(argv[1]);
    //fstream fout(argv[2]);
    
    string trash, Tech_name, Libcell_name, pin_name, top_die_tech, bottom_die_tech, instance_name, net_name;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y, die_lower_x, die_lower_y;
    int die_upper_x, die_upper_y;
    int top_start_x, top_start_y, top_row_length, top_row_height;  
    int bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height;  
    int terminal_size_x, terminal_size_y, terminal_spacing;
    int Num_instance, Num_net, Num_net_pin;

    unordered_map<string, instance> instances;
    unordered_map<string, net*> nets;
    instances.reserve(Num_instance);
    nets.reserve(Num_net);

    fin >> trash >> NumTechnologies;
    tech_stack.reserve(NumTechnologies);

    for(int i = 0; i < NumTechnologies; i++){
        unordered_map<string,libcell> libcells;
        libcells.reserve(Num_lib_cell);
        fin >> trash >> Tech_name >> Num_lib_cell;
        libcells.reserve(Num_lib_cell);
        for(int j = 0; j < Num_lib_cell; j++){
            fin >> trash >> Libcell_name >> lib_x >> lib_y >> Num_pin;
            unordered_map<string, pin> pin_stack;
            pin_stack.reserve(Num_pin);
            for(int k = 0; k < Num_pin; k++){
                fin >> trash >> pin_name >> pin_x >> pin_y;
                point pin_point(pin_x,pin_y);
                pin cell_pin = {pin_name, pin_point};
                pin_stack[pin_name] = cell_pin;
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

    vector<cell_node>* nodes = new vector<cell_node>; 
    nodes->reserve(Num_instance);

    for(int i = 0; i < Num_instance; i++){
        fin >> trash >> instance_name >> Libcell_name;
        instance c(Libcell_name);
        instances[instance_name] = c;
        cell_node C(instance_name, Libcell_name);
        nodes->push_back(C);
    }
    
    fin >> trash >> Num_net;
    partition_net* temp_partition = new partition_net[Num_net];
    vector<partition_net*>* n = new vector<partition_net*>;
    
    for(int i = 0; i < Num_net; i++){
        fin >> trash >> net_name >> Num_net_pin;
        temp_partition[i] = partition_net(net_name); //assign net name 
        net* Net = new net;
        Net->Net_name = net_name;
        for(int j = 0; j < Num_net_pin; j++){
            string temp;
            fin >> trash >> temp;
            pair<string, string> p = split_string(temp);
            Net->add_ip(p.first, p.second, instances);
            int num = stoi(p.first.substr(1))-1;
            temp_partition[i].add_node(&(nodes->at(num)));
        }
        nets[net_name] = Net;
        n->push_back(&(temp_partition[i]));
    }
    
    die_area = (die_upper_x - die_lower_x) * (die_upper_y - die_lower_y);
    FM_algorithm(*nodes,*n);
    //assign back to main data structure
    for(vector<cell_node>::iterator it = nodes->begin(); it != nodes->end(); ++it){
        string name = it->node_name; 
        if(it->part == PART::TOP){
            instances[name].part = PART::TOP;
            if(top_die_tech == "TA")
                instances[name].tech = TECH::TECH_A;
            else
                instances[name].tech = TECH::TECH_B;
        }
        else{
            instances[name].part = PART::BOTTOM;
            if(bottom_die_tech == "TA")
                instances[name].tech = TECH::TECH_A;
            else
                instances[name].tech = TECH::TECH_B;
        }
    }
    
    delete[] temp_partition;
    delete nodes;
    delete n;
    
    fin.close();
    return 0;
}