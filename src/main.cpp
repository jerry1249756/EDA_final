#include <fstream>
#include "FM_alg.h"
#include "module.h"
#include "Kraftwerk2_utility.h"
#include "Kraftwerk2.h"
using namespace std;

vector<tech> tech_stack; //tech_stack[0] is TA; tech_stack[1] is TB if it exists.
unsigned long long int die_area;
unsigned int top_die_max_util, bottom_die_max_util;
unsigned int top_repeat_count, bottom_repeat_count; 
string top_die_tech, bottom_die_tech;
int die_lower_x, die_lower_y, die_upper_x, die_upper_y;

int main(int argc, char* argv[]){
    //read file
    fstream fin(argv[1]);
    //fstream fout(argv[2]);
    
    string trash, Tech_name, Libcell_name, pin_name, instance_name, net_name;
    int NumTechnologies, Num_lib_cell, lib_x, lib_y, Num_pin, pin_x, pin_y;
    int top_start_x, top_start_y, top_row_length, top_row_height;  
    int bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height;  
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
    die_rows top_die_rows = {top_start_x, top_start_y, top_row_height, top_row_height, top_repeat_count};
    die_rows bottom_die_rows = {bottom_start_x, bottom_start_y, bottom_row_length, bottom_row_height, bottom_repeat_count};

    fin >> trash >> top_die_tech >> trash >> bottom_die_tech;
    fin >> trash >> terminal_size_x >> terminal_size_y;
    fin >> trash >> terminal_spacing;
    fin >> trash >> Num_instance;
    unordered_map<string, instance> instances;
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

    delete[] temp_partition;
    delete nodes;
    delete n;

    //test
    Kraftwerk2 k(Num_instance,3,instances);
    k.gen_connectivity_matrix(nets);
    k.print_mat();

    k.get_solution(instances);
    int die_width = die_upper_x - die_lower_x;
    vector<vector<float>> D;
    vector<vector<float>> phi;
    D.reserve(top_repeat_count);
    phi.reserve(top_repeat_count);
    for(int i = 0; i < D.capacity(); i++){
        vector<float> temp;
        vector<float> temp2;
        temp.reserve(die_width/top_row_height);
        temp2.reserve(die_width/top_row_height);
        for(int j = 0; j < temp.capacity(); j++){
            temp.push_back(0);
            temp2.push_back(0);
        }
        D.push_back(temp);
        phi.push_back(temp2);
    }

    for(auto it = instances.begin(); it != instances.end(); ++it){
        cout << it->second.libcell_type << ": ";
        if(it->second.tech == TECH::TECH_A) cout << "TA ";
        else cout << "TB ";
        if(it->second.part == PART::TOP) cout << "top ";
        else cout << "bottom ";
        cout << it->second.instance_pos.x << " " << it->second.instance_pos.y;
        cout << endl;
    }
    cal_D(instances,D,PART::TOP,top_row_height,k.demand_x);
    for(int i = 0; i < D.size(); i++){
        for(int j = 0; j < D[i].size(); j++){
            cout << "i= " << i;
            cout << " j= " << j << "  ";
            cout << D[i][j] << endl;
        }
    }
    cal_phi(D,phi,top_row_height,10);
    k.calc_gradient(phi,top_row_height,instances);
    cout << "cal phi: " << endl;
    for(int i = 0; i < phi.size(); i++){
        for(int j = 0; j < phi[i].size(); j++){
            cout << "i= " << i;
            cout << " j= " << j << "  ";
            cout << phi[i][j] << endl;
        }
    }

    delete[] Net;
    fin.close();
    return 0;
}