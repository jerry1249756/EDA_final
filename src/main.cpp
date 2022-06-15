#include <fstream>
#include "FM_alg.h"
#include "module.h"
#include "Neighborhood.h"
#include "Kraftwerk2.h"
using namespace std;

unordered_map<string, instance> instances;
vector<tech> tech_stack; //tech_stack[0] is TA; tech_stack[1] is TB if it exists.
unsigned long long int die_area;
unsigned int top_die_max_util, bottom_die_max_util;
unsigned int top_repeat_count, bottom_repeat_count; 
string top_die_tech, bottom_die_tech;
int die_lower_x, die_lower_y, die_upper_x, die_upper_y;
int top_row_height;
int bottom_row_height;
unsigned long long int TOP_area=0, BOTTOM_area=0;
int top_die_upper_y, bottom_die_upper_y;

int main(int argc, char* argv[]){
    //read file
    fstream fin(argv[1]);
    fstream fout(argv[2]);
    
    string trash, Tech_name, Libcell_name, pin_name, instance_name, net_name;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y;
    int top_start_x, top_start_y, top_row_length;  
    int bottom_start_x, bottom_start_y, bottom_row_length;  
    int terminal_size_x, terminal_size_y, terminal_spacing;
    int Num_instance, Num_net, Num_net_pin;

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
    die_rows top_die_rows = {top_start_x, top_start_y, top_row_length, top_row_height, top_repeat_count};
    die_rows bottom_die_rows = {bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count};
    top_die_upper_y = top_repeat_count * top_row_height;
    bottom_die_upper_y = bottom_repeat_count * bottom_row_height;
    fin >> trash >> top_die_tech >> trash >> bottom_die_tech;
    fin >> trash >> terminal_size_x >> terminal_size_y;
    fin >> trash >> terminal_spacing;
    fin >> trash >> Num_instance;
    
    unordered_map<string, net*> nets;
    instances.reserve(Num_instance);
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
    nets.reserve(Num_net);
    partition_net* temp_partition = new partition_net[Num_net];
    vector<partition_net*>* n = new vector<partition_net*>;
    net* Net = new net[Num_net];

    for(int i = 0; i < Num_net; i++){
        fin >> trash >> net_name >> Num_net_pin;
        temp_partition[i] = partition_net(net_name); //assign net name 
        Net[i].Net_name = net_name;
        for(int j = 0; j < Num_net_pin; j++){
            string temp;
            fin >> trash >> temp;
            pair<string, string> p = split_string(temp);
            Net[i].add_ip(p.first, p.second, instances);
            int num = stoi(p.first.substr(1))-1;
            temp_partition[i].add_node(&(nodes->at(num)));
        }
        nets[net_name] = &(Net[i]);
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
    

    //fout
    

    delete[] temp_partition;
    delete nodes;
    delete n;

    //test

    // cost test
    Neighborhood nei;

    Kraftwerk2 k1(Num_instance,instances);
    //k1.print_solution();
    k1.get_solution(instances);    
    vector<vector<int>> toptop;
    vector<vector<int>> botbot;
    nei.place_instance_to_each_row(instances, toptop, botbot);
    nei.sort_row(toptop, botbot);
    long long int temp3 = nei.calc_cost(instances ,nets);
    long long int temp4 = nei.init_penalty(instances, toptop, botbot);
    k1.input_solution(instances);
    k1.gen_connectivity_matrix(nets);
    //k1.print_mat();
    //k1.calc_gradient(instances);
    //k1.update_pos_diff();
    
    for(auto it = instances.begin(); it != instances.end(); ++it){
        fout << it->first << " ";
        if(it->second.part == PART::TOP){
            fout << "top ";
        }
        else{
            fout << "bottom ";
        }
        if(it->second.tech == TECH::TECH_A){
            fout << tech_stack[0].libcells[it->second.libcell_type].width << " ";
            fout << tech_stack[0].libcells[it->second.libcell_type].height << " ";
        }
        else{
            fout << tech_stack[1].libcells[it->second.libcell_type].width << " ";
            fout << tech_stack[1].libcells[it->second.libcell_type].height << " ";
        }
        fout << it->second.instance_pos.x << " " << it->second.instance_pos.y;
        fout << endl;
    }
    
    for(int i = 0; i < Num_instance; i++){
        fout << instances["C"+to_string(i+1)].instance_pos.x << " ";
    }
    fout << endl;
    for(int i = 0; i < Num_instance; i++){
        fout << instances["C"+to_string(i+1)].instance_pos.y << " ";
    }
    fout << endl;
    k1.Kraftwerk2_global_placement(instances,fout);
    //k1.print_solution();
    //k.get_solution(instances);
    k1.print_solution(fout);
    // cost test
    vector<vector<int>> toptop2;
    vector<vector<int>> botbot2;
    nei.place_instance_to_each_row(instances, toptop2, botbot2);
    nei.sort_row(toptop2, botbot2);
    
    for(int i = 0; i < Num_instance; i++){
        fout << instances["C"+to_string(i+1)].instance_pos.x << " ";
    }
    fout << endl;
    for(int i = 0; i < Num_instance; i++){
        fout << instances["C"+to_string(i+1)].instance_pos.y << " ";
    }
    cout << endl;
    long long int temp = nei.calc_cost(instances ,nets);
    long long int temp2 = nei.init_penalty(instances, toptop2, botbot2);
    cout << endl;
    cout << "before cost: " << temp3 << endl;
    cout << "after cost: " << temp << endl;
    cout << "before penalty: " << temp4 << endl;
    cout << "after penalty: " << temp2 << endl;


    delete[] Net;
    fin.close();
    fout.close();
    return 0;
}