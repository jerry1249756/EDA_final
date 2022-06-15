#include "Neighborhood.h"

// extern int Num_net, Num_instance;
// extern string top_die_tech, bottom_die_tech;
// extern vector<tech> tech_stack;
// extern int top_row_height, bottom_row_height, top_repeat_count, bottom_repeat_count, die_upper_x;


Neighborhood::Neighborhood() {
    if(top_die_tech == "TA" && bottom_die_tech == "TB"){
        toptech = 0;
        bottomtech = 1;
    }
    else if(top_die_tech == "TB" && bottom_die_tech == "TA"){
        toptech = 1;
        bottomtech = 0;
    }
    else{
        toptech = 0;
        bottomtech = 0;
    }
}


int Neighborhood::find_pos_x(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].instance_pos.x;
}
int Neighborhood::find_pos_y(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].instance_pos.y;
}
PART Neighborhood::find_part(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].part;
}
TECH Neighborhood::find_tech(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].tech;
}
string Neighborhood::find_libcelltype(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].libcell_type;
}
vector<net*> Neighborhood::find_connected(int num, unordered_map<string, instance> instances){
    return instances["C"+to_string(num)].connected_nets;
}

long long int Neighborhood::calc_cost(unordered_map<string, instance>& instances, unordered_map<string, net*> nets){
    long long int total_cost = 0;
    for(int i=0; i<nets.size(); i++){
        single_net_cost(instances, nets, "N"+to_string(i+1));
        total_cost += nets["N"+to_string(i+1)]->cost;
    }
    return total_cost;
}

void Neighborhood::single_net_cost(unordered_map<string, instance>& instances, unordered_map<string, net*>& nets, string netname){
    int SingleNet_cost = 0;
    int TopDie_rightmost = INT32_MIN;
    int TopDie_leftmost = INT32_MAX;
    int TopDie_topmost = INT32_MIN;
    int TopDie_bottommost = INT32_MAX;
    int BottomDie_rightmost = INT32_MIN;
    int BottomDie_leftmost = INT32_MAX;
    int BottomDie_topmost = INT32_MIN;
    int BottomDie_bottommost = INT32_MAX;
    int netsize = nets[netname]->net_pin.size();
    for(int i=0; i<netsize; i++){
        point ins_pos = instances[nets[netname]->net_pin[i].INSTANCE].instance_pos;
        if(instances[nets[netname]->net_pin[i].INSTANCE].part == PART::TOP){  // this instance belongs to TOP
            point pin_pos = tech_stack[toptech].libcells[instances[nets[netname]->net_pin[i].INSTANCE].libcell_type].pins[nets[netname]->net_pin[i].PIN].pin_pos;
            if(pin_pos.x+ins_pos.x > TopDie_rightmost) TopDie_rightmost = pin_pos.x+ins_pos.x;
            if(pin_pos.x+ins_pos.x < TopDie_leftmost) TopDie_leftmost = pin_pos.x+ins_pos.x;
            if(pin_pos.y+ins_pos.y > TopDie_topmost) TopDie_topmost = pin_pos.y+ins_pos.y;
            if(pin_pos.y+ins_pos.y < TopDie_bottommost) TopDie_bottommost = pin_pos.y+ins_pos.y;
        }
        else{
            point pin_pos = tech_stack[bottomtech].libcells[instances[nets[netname]->net_pin[i].INSTANCE].libcell_type].pins[nets[netname]->net_pin[i].PIN].pin_pos;
            if(pin_pos.x+ins_pos.x > BottomDie_rightmost) BottomDie_rightmost = pin_pos.x+ins_pos.x;
            if(pin_pos.x+ins_pos.x < BottomDie_leftmost) BottomDie_leftmost = pin_pos.x+ins_pos.x;
            if(pin_pos.y+ins_pos.y > BottomDie_topmost) BottomDie_topmost = pin_pos.y+ins_pos.y;
            if(pin_pos.y+ins_pos.y < BottomDie_bottommost) BottomDie_bottommost = pin_pos.y+ins_pos.y;
        }
    }
    if(TopDie_rightmost != INT32_MAX && BottomDie_rightmost != INT32_MAX){
        SingleNet_cost += (TopDie_rightmost-TopDie_leftmost)+(TopDie_topmost-TopDie_bottommost)+(BottomDie_rightmost-BottomDie_leftmost)+(BottomDie_topmost-BottomDie_bottommost);
    }
    else if(TopDie_rightmost == INT32_MAX) SingleNet_cost += (BottomDie_rightmost-BottomDie_leftmost)+(BottomDie_topmost-BottomDie_bottommost);
    else SingleNet_cost += (TopDie_rightmost-TopDie_leftmost)+(TopDie_topmost-TopDie_bottommost);

    if(TopDie_rightmost != INT32_MAX && BottomDie_rightmost != INT32_MAX){
        if(TopDie_rightmost<BottomDie_leftmost) SingleNet_cost += BottomDie_leftmost-TopDie_rightmost;
        if(TopDie_leftmost>BottomDie_rightmost) SingleNet_cost += TopDie_leftmost-BottomDie_rightmost;
        if(TopDie_topmost<BottomDie_bottommost) SingleNet_cost += BottomDie_bottommost-TopDie_topmost;
        if(TopDie_bottommost>BottomDie_topmost) SingleNet_cost += TopDie_bottommost-BottomDie_topmost;
    }
    nets[netname]->cost = SingleNet_cost;
}

void Neighborhood::place_instance_to_each_row(unordered_map<string, instance>& instances, vector<vector<int>>& topdie_row, vector<vector<int>>& bottomdie_row){
    topdie_row.resize(top_repeat_count);
    bottomdie_row.resize(bottom_repeat_count);
    for(int i=0; i<instances.size(); i++){
        if(instances["C"+to_string(i+1)].part == PART::TOP){
            if(instances["C"+to_string(i+1)].instance_pos.y>=die_upper_y-0.5*top_row_height) instances["C"+to_string(i+1)].instance_pos.y = die_upper_y-top_row_height;
            else if(instances["C"+to_string(i+1)].instance_pos.y<die_lower_y) instances["C"+to_string(i+1)].instance_pos.y = die_lower_y;
            else instances["C"+to_string(i+1)].instance_pos.y = (int)((double)find_pos_y(i+1, instances)+0.5*top_row_height)/top_row_height*top_row_height;
            topdie_row[(int)((double)find_pos_y(i+1, instances)+0.5*top_row_height)/top_row_height].push_back(i+1);
        }
        else{
            if(instances["C"+to_string(i+1)].instance_pos.y>=die_upper_y-0.5*bottom_row_height) instances["C"+to_string(i+1)].instance_pos.y = die_upper_y-bottom_row_height;
            else if(instances["C"+to_string(i+1)].instance_pos.y<die_lower_y) instances["C"+to_string(i+1)].instance_pos.y = die_lower_y;
            else instances["C"+to_string(i+1)].instance_pos.y = (int)((double)find_pos_y(i+1, instances)+0.5*bottom_row_height)/bottom_row_height*bottom_row_height;
            bottomdie_row[(int)((double)find_pos_y(i+1, instances)+0.5*bottom_row_height)/bottom_row_height].push_back(i+1);
        }
    }
}

bool row_compare(int ins1, int ins2){
    return instances["C"+to_string(ins1)].instance_pos.x < instances["C"+to_string(ins2)].instance_pos.x;
}
void Neighborhood::sort_row(vector<vector<int>>& topdie_row, vector<vector<int>>& bottomdie_row){
    for(int i=0; i<top_repeat_count; i++){
        sort(topdie_row[i].begin(), topdie_row[i].end(), row_compare);
    }
    for(int i=0; i<bottom_repeat_count; i++){
        sort(bottomdie_row[i].begin(), bottomdie_row[i].end(), row_compare);
    }
}

void Neighborhood::sort_single_row(vector<vector<int>>& die_row, int row){
    sort(die_row[row].begin(), die_row[row].end(), row_compare);
}

int Neighborhood::single_row_penalty(vector<vector<int>>& die_row, int row){
    int init_penalty = 0;
    if(die_row[row].size()>0){
        if(die_row[row][0].part == PART::TOP){
            for(int j=0; j<die_row[row].size()-1; j++){
                for(int k=j+1; k<die_row[row].size(); k++){
                    if(find_pos_x(die_row[row][j], instances)+tech_stack[toptech].libcells[find_libcelltype(die_row[row][j], instances)].width>find_Pos_x(die_row[row][k], instances)){
                        init_penalty += (find_pos_x(die_row[row][j], instances)+tech_stack[toptech].libcells[find_libcelltype(die_row[row][j], instances)].width-find_pos_x(die_row[row][k], instances))*top_row_height;
                    }
                    else break;
                }
                // if(find_pos_x(die_row[row][j], instances)+tech_stack[toptech].libcells[find_libcelltype(die_row[i][j], instances)].width>die_upper_x){
                //     init_penalty += 1000;
                // }
            }
        }
        else{
            for(int j=0; j<die_row[row].size()-1; j++){
                for(int k=j+1; k<die_row[row].size(); k++){
                    if(find_pos_x(die_row[row][j], instances)+tech_stack[bottomtech].libcells[find_libcelltype(die_row[row][j], instances)].width>find_Pos_x(die_row[row][k], instances)){
                        init_penalty += (find_pos_x(die_row[row][j], instances)+tech_stack[bottomtech].libcells[find_libcelltype(die_row[row][j], instances)].width-find_pos_x(die_row[row][k], instances))*bottom_row_height;
                    }
                    else break;
                }
                // if(find_pos_x(die_row[row][j], instances)+tech_stack[bottomtech].libcells[find_libcelltype(die_row[i][j], instances)].width>die_upper_x){
                //     init_penalty += 1000;
                // }
            }
        }
    }
    return init_penalty;
}

long long int Neighborhood::init_penalty(unordered_map<string, instance>& ins, vector<vector<int>>& topdie_row, vector<vector<int>>& bottomdie_row){
    long long int init_penalty = 0;
    for(int i=0; i<top_repeat_count; i++){
        cout << endl;
        for(int j=0; j<topdie_row[i].size()-1; j++){
            for(int k=j+1; k<topdie_row[i].size(); k++){
                cout << "top: " << i <<" "<< j <<" "<<k;
                if(find_pos_x(topdie_row[i][j], ins)+tech_stack[toptech].libcells[find_libcelltype(topdie_row[i][j], ins)].width>find_pos_x(topdie_row[i][k], ins)){
                    init_penalty += (find_pos_x(topdie_row[i][j], ins)+tech_stack[toptech].libcells[find_libcelltype(topdie_row[i][j], ins)].width-find_pos_x(topdie_row[i][k], ins))*top_row_height;
                }
                else break;
            }
            // if(find_pos_x(topdie_row[row][j], ins)+tech_stack[toptech].libcells[find_libcelltype(topdie_row[i][j], ins)].width>die_upper_x){
            //     init_penalty += 1000;
            // }
        }
    }
    for(int i=0; i<bottom_repeat_count; i++){
        cout << endl;
        for(int j=0; j<bottomdie_row[i].size()-1; j++){
            for(int k=j+1; k<bottomdie_row[i].size(); k++){
                cout << "top: " << i <<" "<< j <<" "<<k;
                if(find_pos_x(bottomdie_row[i][j], ins)+tech_stack[bottomtech].libcells[find_libcelltype(bottomdie_row[i][j], ins)].width>find_pos_x(bottomdie_row[i][k], ins)){
                    init_penalty += (find_pos_x(bottomdie_row[i][j], ins)+tech_stack[bottomtech].libcells[find_libcelltype(bottomdie_row[i][j], ins)].width-find_pos_x(bottomdie_row[i][k], ins))*bottom_row_height;
                }
                else break;
            }
            // if(find_pos_x(bottomdie_row[row][j], ins)+tech_stack[bottomtech].libcells[find_libcelltype(bottomdie_row[i][j], ins)].width>die_upper_x){
            //     init_penalty += 1000;
            // }
        }
    }
    return init_penalty;
}

void Neighborhood::swap_instance(int from, int to){
    int temp_x = find_pos_x(from);
    int temp_y = find_Pos_x(from);
    instances["C"+to_string(from)].instance_pos.x = find_pos_x(to);
    instances["C"+to_string(from)].instance_pos.y = find_pos_y(to);
    instances["C"+to_string(to)].instance_pos.x = temp_x;
    instances["C"+to_string(to)].instance_pos.y = temp_y;
}

void Neighborhood::two_die_swap_instance(int from, int to){
    int temp_x = find_pos_x(from);
    int temp_y = find_Pos_x(from);
    TECH temp_tech = find_tech(from);
    PART temp_part = find_part(from);
    instances["C"+to_string(from)].instance_pos.x = find_pos_x(to);
    instances["C"+to_string(from)].instance_pos.y = find_pos_y(to);
    instances["C"+to_string(to)].instance_pos.x = temp_x;
    instances["C"+to_string(to)].instance_pos.y = temp_y;
    instances["C"+to_string(from)].part = find_part(to);
    instances["C"+to_string(to)].part = temp_part;
    instances["C"+to_string(from)].tech = find_tech(to);
    instances["C"+to_string(to)].tech = temp_tech;
}

int Neighborhood::swap_cost(unordered_map<string, instance>& ins, unordered_map<string, net*>& nets, vector<vector<int>>& die_row, int from_row, int from_col, int to_row, int to_col){
    int cost = 0;
    int before, after;
    vector<string> netname;
    for(int i=0; i<find_connected(die_row[from_row][from_col], ins).size(); i++){
        if(find(netname.begin(), netname.end(), find_connected(die_row[from_row][from_col], ins)[i]->Net_name) == netname.end()){
            netname.push_back(find_connected(die_row[from_row][from_col], ins)[i]->Net_name);
        }
    }
    for(int i=0; i<find_connected(die_row[to_row][to_col], ins).size(); i++){
        if(find(netname.begin(), netname.end(), find_connected(die_row[to_row][to_col], ins)[i]->Net_name) == netname.end()){
            netname.push_back(find_connected(die_row[to_row][to_col], ins)[i]->Net_name);
        }
    }
    swap_instance(die_row[from_row][from_col], die_row[to_row][to_col]);
    for(int i=0; i<netname.size(); i++){
        before = nets[netname[i]]->cost;
        single_net_cost(ins, nets, netname[i]);
        after = nets[netname[i]]->cost;
        cost += after - before;
    }
    swap_instance(die_row[from_row][from_col], die_row[to_row][to_col]);
    return cost;
}

int Neighborhood::two_die_swap_cost(unordered_map<string, instance>& ins, unordered_map<string, net*>& nets, vector<vector<int>>& from_die_row, vector<vector<int>>& to_die_row, int from_row, int from_col, int to_row, int to_col){
    int cost = 0;
    int before, after;
    vector<string> netname;
    for(int i=0; i<find_connected(from_die_row[from_row][from_col], ins).size(); i++){
        if(find(netname.begin(), netname.end(), find_connected(from_die_row[from_row][from_col], ins)[i]->Net_name) == netname.end()){
            netname.push_back(find_connected(from_die_row[from_row][from_col], ins)[i]->Net_name);
        }
    }
    for(int i=0; i<find_connected(to_die_row[to_row][to_col], ins).size(); i++){
        if(find(netname.begin(), netname.end(), find_connected(to_die_row[to_row][to_col], ins)[i]->Net_name) == netname.end()){
            netname.push_back(find_connected(to_die_row[to_row][to_col], ins)[i]->Net_name);
        }
    }
    two_die_swap_instance(from_die_row[from_row][from_col], to_die_row[to_row][to_col]);
    for(int i=0; i<netname.size(); i++){
        before = nets[netname[i]]->cost;
        single_net_cost(ins, nets, netname[i]);
        after = nets[netname[i]]->cost;
        cost += after - before;
    }
    two_die_swap_instance(to_die_row[to_row][to_col], from_die_row[from_row][from_col]);
    return cost;
}

int Neighborhood::move_in_row_cost(unordered_map<string, instance>& ins, unordered_map<string, net*>& nets, vector<vector<int>>& die_row, int from_row, int from_col, int to_x){
    int cost = 0;
    int before, after;
    int temp_x;
    temp_x = find_pos_x(die_row[from_row][from_col], ins);
    instances["C"+to_string(die_row[from_row][from_col])].instance_pos.x = to_x;
    for(int i=0; i<find_connected(die_row[from_row][from_col], ins).size(); i++){
        before = nets[find_connected(die_row[from_row][from_col], ins)[i]]->cost;
        single_net_cost(ins, nets, find_connected(die_row[from_row][from_col], ins)[i]);
        after = nets[find_connected(die_row[from_row][from_col], ins)[i]]->cost;
        cost += after - before;
    }
    instances["C"+to_string(die_row[from_row][from_col])].instance_pos.x = temp_x;
    return cost;
}

int Neighborhood::move_between_row_cost(unordered_map<string, instance>& ins, unordered_map<string, net*>& nets, vector<vector<int>>& die_row, int from_row, int from_col, int to_y){
    int cost = 0;
    int before, after;
    int temp_y;
    temp_y = find_pos_y(die_row[from_row][from_col], ins);
    if(find_part(die_row[from_row][from_col], ins) == PART::TOP) instances["C"+to_string(die_row[from_row][from_col])].instance_pos.y = to_y/top_row_height*top_row_height;
    else instances["C"+to_string(die_row[from_row][from_col])].instance_pos.y = to_y/bottom_row_height*bottom_row_height;
    for(int i=0; i<find_connected(die_row[from_row][from_col], ins).size(); i++){
        before = nets[find_connected(die_row[from_row][from_col], ins)[i]]->cost;
        single_net_cost(ins, nets, find_connected(die_row[from_row][from_col], ins)[i]);
        after = nets[find_connected(die_row[from_row][from_col], ins)[i]]->cost;
        cost += after - before;
    }
    instances["C"+to_string(die_row[from_row][from_col])].instance_pos.y = temp_y;
    return cost;
}

int Neighborhood::two_die_move_cost(unordered_map<string, instance>& ins, unordered_map<string, net*>& nets, vector<vector<int>>& from_die_row, vector<vector<int>>& to_die_row, int from_row, int from_col){
    int cost = 0;
    int before, after;
    int temp_y;
    temp_y = find_pos_y(from_die_row[from_row][from_col], ins);
    if(find_part(from_die_row[from_row][from_col], ins) == PART::TOP){
        ins["C"+to_string(from_die_row[from_row][from_col])].part = PART::BOTTOM;
        ins["C"+to_string(from_die_row[from_row][from_col])].instance_pos.y = find_pos_y(from_die_row[from_row][from_col])/bottom_row_height*bottom_row_height;
    }
    else{
        ins["C"+to_string(from_die_row[from_row][from_col])].part = PART::TOP;
        ins["C"+to_string(from_die_row[from_row][from_col])].instance_pos.y = find_pos_y(from_die_row[from_row][from_col])/top_row_height*top_row_height;
    }
    for(int i=0; i<find_connected(from_die_row[from_row][from_col], ins).size(); i++){
        before = nets[find_connected(from_die_row[from_row][from_col], ins)[i]]->cost;
        single_net_cost(ins, nets, find_connected(from_die_row[from_row][from_col], ins)[i]);
        after = nets[find_connected(from_die_row[from_row][from_col], ins)[i]]->cost;
        cost += after - before;
    }
    if(find_part(from_die_row[from_row][from_col], ins) == PART::TOP)  ins["C"+to_string(from_die_row[from_row][from_col])].part = PART::BOTTOM;
    else ins["C"+to_string(from_die_row[from_row][from_col])].part = PART::TOP;
    ins["C"+to_string(from_die_row[from_row][from_col])].instance_pos.y = temp_y;
    return cost;
} 

void Neighborhood::update_swap(vector<vector<int>>& from_die_row, vector<vector<int>>& to_die_row, int from_row, int from_col, int to_row, int to_col){
    two_die_swap_instance(from_die_row[from_row][from_col], to_die_row[to_row][to_col]);
    from_die_row[from_row].push_back(to_die_row[to_row][to_col]);
    to_die_row[to_row].push_back(from_die_row[from_row][from_col]);
    from_die_row[from_row].erase(from_die_row[from_row].begin()+from_col);
    to_die_row[to_row].erase(to_die_row[to_row].begin()+to_col);
    sort_single_row(from_die_row, from_row);
    sort_single_row(to_die_row, to_row);
}

void Neighborhood::update_move_in_row(vector<vector<int>>& die_row, int from_row, int from_col, int to_x){
    instances["C"+to_string(die_row[from_row][from_col])].instance_pos.x = to_x;
    sort_single_row(die_row, from_row);
}

void Neighborhood::update_move_between_row(vector<vector<int>>& die_row, int from_row, int from_col, int to_y){
    if(find_part(die_row[from_row][from_col], instances) == PART::TOP){
        instances["C"+to_string(die_row[from_row][from_col])].instance_pos.y = to_y/top_row_height*top_row_height;
        die_row[find_pos_y(die_row[from_row][from_col], instances)/top_row_height].push_back(die_row[from_row][from_col]);
        sort(die_row, find_pos_y(die_row[from_row][from_col], instances)/top_row_height);
    }
    else{
        instances["C"+to_string(die_row[from_row][from_col])].instance_pos.y = to_y/bottom_row_height*bottom_row_height;
        die_row[find_pos_y(die_row[from_row][from_col], instances)/bottom_row_height].push_back(die_row[from_row][from_col]);
        sort(die_row, find_pos_y(die_row[from_row][from_col], instances)/bottom_row_height);
    }
    die_row[from_row].erase(die_row[from_row].begin()+from_col);
    sort(die_row, from_row);
}

void Neighborhood::update_two_die_move(vector<vector<int>>& from_die_row, vector<vector<int>>& to_die_row, int from_row, int from_col){
    if(find_part(from_die_row[from_row][from_col], instances) == PART::TOP){
        instances["C"+to_string(from_die_row[from_row][from_col])].instance_pos.y = to_y/bottom_row_height*bottom_row_height;
        to_die_row[find_pos_y(from_die_row[from_row][from_col], instances)/bottom_row_height].push_back(from_die_row[from_row][from_col]);
        sort(to_die_row, find_pos_y(from_die_row[from_row][from_col], instances)/bottom_row_height);
    }
    else{
        instances["C"+to_string(from_die_row[from_row][from_col])].instance_pos.y = to_y/top_row_height*top_row_height;
        to_die_row[find_pos_y(from_die_row[from_row][from_col], instances)/top_row_height].push_back(from_die_row[from_row][from_col]);
        sort(to_die_row, find_pos_y(from_die_row[from_row][from_col], instances)/to_row_height);
    }
    if(find_part(from_die_row[from_row][from_col], instances) == PART::TOP) instances["C"+to_string(from_die_row[from_row][from_col])].part = PART::BOTTOM;
    else instances["C"+to_string(from_die_row[from_row][from_col])].part = PART::TOP;
    for(int i=0; i<to_die.size(); i++){
        if(to_die[i].size>0){
            if(find_tech(to_die_row[i][0], instances) == TECH::TECH_A) instances["C"+to_string(from_die_row[from_row][from_col])].tech = TECH::TECH_A;
            else instances["C"+to_string(from_die_row[from_row][from_col])].tech = TECH::TECH_B;
            break;
        }
    }
    from_die_row[from_row].erase(from_die_row[from_row].begin()+from_col);
    sort(from_die_row, from_row);
}