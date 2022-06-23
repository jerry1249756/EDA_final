#include "Kraftwerk2.h"

Kraftwerk2::Kraftwerk2(unordered_map<string, instance>& map){ //n: num instances
    w_i.resize(Num_instance);
    //initial the D and phi array
    int die_width = die_upper_x - die_lower_x;
        D_top.resize(die_width/min(top_row_height,bottom_row_height));
        D_bottom.resize(die_width/min(top_row_height,bottom_row_height));
        phi_top.resize(die_width/min(top_row_height,bottom_row_height));
        phi_bottom.resize(die_width/min(top_row_height,bottom_row_height));
    for(int i = 0; i < D_top.capacity(); i++){
        D_top[i].resize(max(top_repeat_count, bottom_repeat_count));
        D_bottom[i].resize(max(top_repeat_count, bottom_repeat_count));
        phi_top[i].resize(max(top_repeat_count, bottom_repeat_count));
        phi_bottom[i].resize(max(top_repeat_count, bottom_repeat_count));
    }
    //randomly assign position while maintaining the instances inside the die.
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        int acceptable_x_max=0;
        int acceptable_y_max=0;
        if(it.second.tech == TECH::TECH_A){
            acceptable_x_max = die_upper_x - tech_stack[0].libcells[it.second.libcell_type].width;
            if(it.second.part == PART::TOP){
                w_i[n] = 0.2*static_cast<double>(tech_stack[0].libcells[it.second.libcell_type].width) * static_cast<double>(tech_stack[0].libcells[it.second.libcell_type].height) / static_cast<double>(TOP_area);
                acceptable_y_max = top_die_upper_y - tech_stack[0].libcells[it.second.libcell_type].height;   
            }
            else{
                w_i[n] = 0.2*static_cast<double>(tech_stack[0].libcells[it.second.libcell_type].width) * static_cast<double>(tech_stack[0].libcells[it.second.libcell_type].height) / static_cast<double>(BOTTOM_area);
                acceptable_y_max = bottom_die_upper_y - tech_stack[0].libcells[it.second.libcell_type].height;   
            }
            
        }else if(it.second.tech == TECH::TECH_B){
            acceptable_x_max = die_upper_x - tech_stack[1].libcells[it.second.libcell_type].width;
            if(it.second.part == PART::TOP){
                w_i[n] = 0.2*static_cast<double>(tech_stack[1].libcells[it.second.libcell_type].width) * static_cast<double>(tech_stack[1].libcells[it.second.libcell_type].height) / static_cast<double>(TOP_area);
                acceptable_y_max = top_die_upper_y - tech_stack[1].libcells[it.second.libcell_type].height;   
            }
            else{
                w_i[n] = 0.2*static_cast<double>(tech_stack[1].libcells[it.second.libcell_type].width) * static_cast<double>(tech_stack[1].libcells[it.second.libcell_type].height) / static_cast<double>(BOTTOM_area);
                acceptable_y_max = bottom_die_upper_y - tech_stack[1].libcells[it.second.libcell_type].height;   
            }
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib_x(die_lower_x,acceptable_x_max);
        uniform_int_distribution<> distrib_y(die_lower_y,acceptable_y_max);
        solution_x(n) = distrib_x(gen);
        solution_y(n) = distrib_y(gen);
    }
    size = Num_instance;
}

void Kraftwerk2::input_solution(unordered_map<string, instance>& map){
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        solution_x(n) = it.second.instance_pos.x ;
        solution_y(n) = it.second.instance_pos.y ;
    }
}

//D should be a m*n zero matrix
void Kraftwerk2::cal_D(unordered_map<string,instance> ins, vector<vector<float>>& D_top, vector<vector<float>>& D_bottom){
    //region = 0 represent top, region = 1 represent bottom
    int delta = min(top_row_height,bottom_row_height);
    float d_mod = 1;
    for(auto it = ins.begin(); it != ins.end(); ++it){
        int n = stoi(it->first.substr(1))-1;    
        //cout << it->first << endl;
        if(it->second.tech == TECH::TECH_A){         
            float pos_x = solution_x(n);
            float pos_y = solution_y(n);
            int width = tech_stack[0].libcells[it->second.libcell_type].width;
            int height = tech_stack[0].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(it->second.part == PART::TOP){    
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (top_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D_top[i][j] += 1*d_mod;
                    }
                    else{
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (bottom_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D_bottom[i][j] += 1*d_mod;
                    }
                }
            }
        }   
        if(it->second.tech == TECH::TECH_B){            
            float pos_x = solution_x(n);
            float pos_y = solution_y(n);
            int width = tech_stack[1].libcells[it->second.libcell_type].width;
            int height = tech_stack[1].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(it->second.part == PART::TOP){    
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (top_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D_top[i][j] += 1*d_mod;
                    }
                    else{
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (bottom_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D_bottom[i][j] += 1*d_mod;
                    }
                }
            }
        }
    }
    for(int i = 0; i < D_top.size(); i++){
        for(int j = 0; j < D_top[i].size(); j++){
            D_top[i][j] -= d_mod * static_cast<float>(TOP_area+BOTTOM_area)/(2*die_area);
            D_bottom[i][j] -= d_mod * static_cast<float>(TOP_area+BOTTOM_area)/(2*die_area);
        }
    }
}

//phi should be m*n zero matrix
void Kraftwerk2::cal_phi(vector<vector<float>> D_top, vector<vector<float>>& phi_top,vector<vector<float>> D_bottom, vector<vector<float>>& phi_bottom, int times){
    int delta = min(top_row_height,bottom_row_height);
    vector<vector<float>> temp;
    vector<vector<float>> temp3;
    vector<vector<float>> temp5;
    vector<vector<float>> temp7;
    temp.reserve(phi_top.size());
    temp3.reserve(phi_top.size());
    temp5.reserve(phi_top.size());
    temp7.reserve(phi_top.size());
    for(int i = 0; i < phi_top.size(); i++){
        vector<float> temp2;
        vector<float> temp4;
        vector<float> temp6;
        vector<float> temp8;
        temp2.reserve(phi_top[i].size());
        temp4.reserve(phi_top[i].size());
        temp6.reserve(phi_top[i].size());
        temp8.reserve(phi_top[i].size());
        for(int j = 0; j < phi_top[i].size(); j++){
            temp2.push_back(0);
            temp4.push_back(0);
            temp6.push_back(0);
            temp8.push_back(0);
        }
        temp.push_back(temp2);
        temp3.push_back(temp4);
        temp5.push_back(temp6);
        temp7.push_back(temp8);
    }
    for(int k = 0; k < times; k++){
        if(k%2 == 0){
            for(int i = 0; i < phi_top.size(); i++){
                for(int j = 0; j < phi_top[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i+1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi_top[i].size()-1){
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i+1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i+1][j]*2) / 4;
                        }
                    }
                    else if(i == phi_top.size()-1){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i-1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi_top[i].size()-1){
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i-1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D_top[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i-1][j]*2) / 4;
                        }
                    }
                    else if(j == 0){
                        temp3[i][j] = (delta*delta*D_top[i][j] + temp[i-1][j] + temp[i][j+1]*2 + temp[i+1][j]) / 4;
                    }
                    else if(j == phi_top[i].size()-1){
                        temp3[i][j] = (delta*delta*D_top[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1]*2) / 4; 
                    }
                    else{
                        temp3[i][j] = (delta*delta*D_top[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) / 4; 
                    }
                }
            }
        }
        else if(k%2 == 1){
            for(int i = 0; i < phi_top.size(); i++){
                for(int j = 0; j < phi_top[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp[i][j] = (delta*D_top[i][j] + temp3[i+1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi_top[i].size()-1){
                            temp[i][j] = (delta*D_top[i][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*D_top[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                        }
                    }
                    else if(i == phi_top.size()-1){
                        if(j == 0){
                            temp[i][j] = (delta*D_top[i][j] + temp3[i-1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi_top[i].size()-1){
                            temp[i][j] = (delta*D_top[i][j] + temp3[i-1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*D_top[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i-1][j]) / 4;
                        }
                    }
                    else if(j == 0){
                        temp[i][j] = (delta*D_top[i][j] + temp3[i-1][j] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                    }
                    else if(j == phi_top[i].size()-1){
                        temp[i][j] = (delta*D_top[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                    }
                    else{
                        temp[i][j] = (delta*D_top[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1] + temp3[i][j+1]) / 4; 
                    }
                }
            }
        }
    }
    if(times%2 == 0){
        for(int i = 0; i < phi_top.size(); i++){
            for(int j = 0; j < phi_top[i].size(); j++){
                phi_top[i][j] = temp[i][j];
            }
        }
    }
    else if(times%2 == 1){
        for(int i = 0; i < phi_top.size(); i++){
            for(int j = 0; j < phi_top[i].size(); j++){
                phi_top[i][j] = temp3[i][j];
            }
        }
    }
    for(int k = 0; k < times; k++){
        if(k%2 == 0){
            for(int i = 0; i < phi_bottom.size(); i++){
                for(int j = 0; j < phi_bottom[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i+1][j] + temp5[i][j+1]) / 2; 
                        }
                        else if(j == phi_bottom[i].size()-1){
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i+1][j] + temp5[i][j-1]) / 2; 
                        }
                        else{
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i][j-1] + temp5[i][j+1] + temp5[i+1][j]*2) / 4;
                        }
                    }
                    else if(i == phi_bottom.size()-1){
                        if(j == 0){
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i-1][j] + temp5[i][j+1]) / 2; 
                        }
                        else if(j == phi_bottom[i].size()-1){
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i-1][j] + temp5[i][j-1]) / 2; 
                        }
                        else{
                            temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i][j-1] + temp5[i][j+1] + temp5[i-1][j]*2) / 4;
                        }
                    }
                    else if(j == 0){
                        temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i-1][j] + temp5[i][j+1]*2 + temp5[i+1][j]) / 4;
                    }
                    else if(j == phi_bottom[i].size()-1){
                        temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i-1][j] + temp5[i+1][j] + temp5[i][j-1]*2) / 4; 
                    }
                    else{
                        temp7[i][j] = (delta*delta*D_bottom[i][j] + temp5[i-1][j] + temp5[i+1][j] + temp5[i][j-1] + temp5[i][j+1]) / 4; 
                    }
                }
            }
        }
        else if(k%2 == 1){
            for(int i = 0; i < phi_bottom.size(); i++){
                for(int j = 0; j < phi_bottom[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i+1][j] + temp7[i][j+1]) / 4; 
                        }
                        else if(j == phi_bottom[i].size()-1){
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i+1][j] + temp7[i][j-1]) / 4; 
                        }
                        else{
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i][j-1] + temp7[i][j+1] + temp7[i+1][j]) / 4;
                        }
                    }
                    else if(i == phi_bottom.size()-1){
                        if(j == 0){
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i-1][j] + temp7[i][j+1]) / 4; 
                        }
                        else if(j == phi_bottom[i].size()-1){
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i-1][j] + temp7[i][j-1]) / 4; 
                        }
                        else{
                            temp5[i][j] = (delta*D_bottom[i][j] + temp7[i][j-1] + temp7[i][j+1] + temp7[i-1][j]) / 4;
                        }
                    }
                    else if(j == 0){
                        temp5[i][j] = (delta*D_bottom[i][j] + temp7[i-1][j] + temp7[i][j+1] + temp7[i+1][j]) / 4;
                    }
                    else if(j == phi_bottom[i].size()-1){
                        temp5[i][j] = (delta*D_bottom[i][j] + temp7[i-1][j] + temp7[i+1][j] + temp7[i][j-1]) / 4; 
                    }
                    else{
                        temp5[i][j] = (delta*D_bottom[i][j] + temp7[i-1][j] + temp7[i+1][j] + temp7[i][j-1] + temp7[i][j+1]) / 4; 
                    }
                }
            }
        }
    }
    if(times%2 == 0){
        for(int i = 0; i < phi_bottom.size(); i++){
            for(int j = 0; j < phi_bottom[i].size(); j++){
                phi_bottom[i][j] = temp5[i][j];
            }
        }
    }
    else if(times%2 == 1){
        for(int i = 0; i < phi_bottom.size(); i++){
            for(int j = 0; j < phi_bottom[i].size(); j++){
                phi_bottom[i][j] = temp7[i][j];
            }
        }
    }
}

int Kraftwerk2::parse_inst_name(string s){ //input: String("M12"), output: int 11
    return stoi(s.substr(1))-1;
}

void Kraftwerk2::get_solution(unordered_map<string, instance>& map){
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        it.second.instance_pos.x = solution_x(n) ;
        it.second.instance_pos.y = solution_y(n) ;
    }
}

void Kraftwerk2::update_bound(string net_name){
    int left = INT32_MAX;
    int right = INT32_MIN;
    int up = INT32_MIN;
    int bottom = INT32_MAX;
    for(int i = 0; i < nets[net_name]->net_pin.size(); i++){
        int j = parse_inst_name(nets[net_name]->net_pin[i].first);
        if(instances[nets[net_name]->net_pin[i].first].tech == TECH::TECH_A){
            if((solution_x(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x) < left){
                left = solution_x(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
                nets[net_name]->left_index = i;
            }
            if((solution_x(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x) > right){
                right = solution_x(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
                nets[net_name]->right_index = i;
            }
            if((solution_y(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y) < bottom){
                bottom = solution_y(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
                nets[net_name]->bottom_index = i;
            }
            if((solution_y(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y) > up){
                up = solution_y(j) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
                nets[net_name]->up_index = i;
            }
        }
        else{
            if((solution_x(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x) < left){
                left = solution_x(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
                nets[net_name]->left_index = i;
            }
            if((solution_x(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x) > right){
                right = solution_x(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
                nets[net_name]->right_index = i;
            }
            if((solution_y(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y) < bottom){
                bottom = solution_y(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
                nets[net_name]->bottom_index = i;
            }
            if((solution_y(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y) > up){
                up = solution_y(j) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
                nets[net_name]->up_index = i;
            }
        }
    }
}

void Kraftwerk2::update_connectivity_y(int i, int j, string net_name){
    int P = nets[net_name]->net_pin.size();
    double y_p;
    double y_q;
    int m = parse_inst_name(nets[net_name]->net_pin[i].first);
    int n = parse_inst_name(nets[net_name]->net_pin[j].first);
    if(instances[nets[net_name]->net_pin[i].first].tech == TECH::TECH_A)
        y_p = solution_y(m) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
    else
        y_p = solution_y(m) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.y;
    
    if(instances[nets[net_name]->net_pin[j].first].tech == TECH::TECH_A)
        y_q = solution_y(n) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[j].first].libcell_type].pins[nets[net_name]->net_pin[j].second].pin_pos.y;
    else
        y_q = solution_y(n) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[j].first].libcell_type].pins[nets[net_name]->net_pin[j].second].pin_pos.y;
    if(y_p != y_q){
        double temp = 2/((P-1)*abs(y_p-y_q));
        connectivity_mat_y(m,n) -= temp;
        connectivity_mat_y(n,m) -= temp;
        connectivity_mat_y(m,m) += temp;
        connectivity_mat_y(n,n) += temp;
    }
}

void Kraftwerk2::update_connectivity_x(int i, int j, string net_name){
    int P = nets[net_name]->net_pin.size();
    double x_p;
    double x_q;
    int m = parse_inst_name(nets[net_name]->net_pin[i].first);
    int n = parse_inst_name(nets[net_name]->net_pin[j].first);
    if(instances[nets[net_name]->net_pin[i].first].tech == TECH::TECH_A)
        x_p = solution_x(m) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
    else
        x_p = solution_x(m) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[i].first].libcell_type].pins[nets[net_name]->net_pin[i].second].pin_pos.x;
    
    if(instances[nets[net_name]->net_pin[j].first].tech == TECH::TECH_A)
        x_q = solution_x(n) + tech_stack[0].libcells[instances[nets[net_name]->net_pin[j].first].libcell_type].pins[nets[net_name]->net_pin[j].second].pin_pos.x;
    else
        x_q = solution_x(n) + tech_stack[1].libcells[instances[nets[net_name]->net_pin[j].first].libcell_type].pins[nets[net_name]->net_pin[j].second].pin_pos.x;
    
    if(x_p != x_q){
        double temp = 2/((P-1)*abs(x_p-x_q));
        connectivity_mat_x(m,n) -= temp;
        connectivity_mat_x(n,m) -= temp;
        connectivity_mat_x(m,m) += temp;
        connectivity_mat_x(n,n) += temp;
    }
}

void Kraftwerk2::gen_connectivity_matrix(unordered_map<string, net*> map){
    cout << "generating connectivity matrix ..\n";
    connectivity_mat_x.setZero();
    connectivity_mat_y.setZero();
    for(auto& it : map){
        update_bound(it.first);
        int k = it.second->left_index;
        int l = it.second->right_index;
        int i = it.second->up_index;
        int j = it.second->bottom_index;
        for(int m = 0; m < it.second->net_pin.size(); m++){
            if(k != m && k != l){
                update_connectivity_x(k,m,it.first);
            }
            if(l != m){
                update_connectivity_x(l,m,it.first);
            }
            if(i != m && i != j){
                update_connectivity_y(i,m,it.first);
            }
            if(j != m){
                update_connectivity_y(j,m,it.first);
            }
        }
    }
    return;
}

void Kraftwerk2::print_mat(){
    cout << "connectivity mat: \n"; 
    for(int i = 0; i < Num_instance; i++){
        for(int j = 0; j < Num_instance; j++){
            cout << connectivity_mat_x(i,j) << " ";
        }
        cout << endl;
    }
    
    cout << " move_force_mat_x: \n"; 
    for(int i = 0; i < Num_instance; i++){
        for(int j = 0; j < Num_instance; j++){
            cout << move_force_mat_x(i,j) << " ";
        }
        cout << endl;
    }
    return;
}

void Kraftwerk2::print_solution(fstream& fout){
    for(int i = 0; i < Num_instance; i++){
        fout << solution_x(i) << " ";    
    }
    fout << endl;
    for(int i = 0; i < Num_instance; i++){
        fout << solution_y(i) << " ";    
    }
    fout << endl;
    return;
}

pair<float, float> Kraftwerk2::single_point_gradient(vector<vector<float>> phi, float x, float y, PART part){
    int delta = min(top_row_height,bottom_row_height);
    //(x,y) is in (i,j) , (i+1,j+1)
    int i = static_cast<int>(x/delta-0.5); //bounding box
    int j = static_cast<int>(y/delta-0.5);
    float slope = 1;

    float val=0, grad_x, grad_y;
    int temp_upper_y = 0;
    if(part == PART::TOP){
        temp_upper_y = top_die_upper_y + die_lower_y;
    }
    else{
        temp_upper_y = bottom_die_upper_y + die_lower_y;
    }
    
    int k = (x/delta-i >= 0.5) ? i+1 : i; //closest point
    int l = (y/delta-j >= 0.5) ? j+1 : j;
    float l1 = x - delta*(i+0.5);
    float l2 = delta*(i+1.5) - x;
    float w1 = y - delta*(j+0.5);
    float w2 = delta*(j+1.5) - y;

    if(i+1<phi.size()){
        if(j+1<phi[0].size()){
            val += l1*w1*phi[i+1][j+1];
        }
        if(j<phi[0].size()){
            val += l1*w2*phi[i+1][j];
        }
    }
    if(i< phi.size()){
        if(j+1<phi[0].size()){
            val += l2*w1*phi[i][j+1];
        }
        if(j<phi[0].size()){
            val += l2*w2*phi[i][j];
        }
    }
    val/=(delta*delta);

    if(k==i){
        if(l==j){
            if(k>=phi.size() || l>phi[0].size()){
                grad_x = val/(delta*l1);
                grad_y = val/(delta*w1);
            }else{
                grad_x = (val - phi[k][l])/(delta*l1);
                grad_y = (val - phi[k][l])/(delta*w1);
            }
        }else{
            if(k>=phi.size() || l>phi[0].size()){
                grad_x = val/(delta*l1);
                grad_y =  -val/(delta*w2);
            }else{
                grad_x = (val - phi[k][l])/(delta*l1);
                grad_y = (phi[k][l] - val)/(delta*w2);
            }
        }
    }else{
        if(l==j){
            if(k>=phi.size() || l>phi[0].size()){
                grad_x = -val/(delta*l2);
                grad_y = val/(delta*w1);
            }else{
                grad_x = (phi[k][l] - val)/(delta*l2);
                grad_y = (val - phi[k][l])/(delta*w1);
            }
        }else{
            if(k>=phi.size() || l>phi[0].size()){
                grad_x =  - val/(delta*l2);
                grad_y =  - val/(delta*w2);
            }else{
                grad_x = (phi[k][l] - val)/(delta*l2);
                grad_y = (phi[k][l] - val)/(delta*w2);
            }   
        }
    }
    return make_pair(grad_x, grad_y);
}
// calculate the C^O matrix, which is the gradient of the specified points, val stored on diagonal entries. 
// input params: phi : 2D float array delta: spacing of the phi array(row height)
// x,y: the point of the instances forming a vector, which will later be stored in solution.

// Note that D(i,j) represents the actual coordinate point at h(1/2+i, 1/2+j)
void Kraftwerk2::calc_gradient(unordered_map<string, instance> map, fstream& fout){ 
    for(int j=0; j<D_top.capacity(); j++){
        for(int k=0; k<D_top[j].capacity(); k++){
            D_top[j][k] = 0;
            D_bottom[j][k] = 0;
            phi_top[j][k] = 0;
            phi_bottom[j][k] = 0;
        }
    }
    cal_D(map, D_top, D_bottom);
    cal_phi(D_top, phi_top, D_bottom, phi_bottom, 10);
    int delta = min(top_row_height,bottom_row_height);
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        //(x,y) is in (i,j) , (i+1,j+1)
        int width;
        int height;
        if(it.second.tech == TECH::TECH_A){
            width = tech_stack[0].libcells[it.second.libcell_type].width;
            height = tech_stack[0].libcells[it.second.libcell_type].height;
        }
        else{
            width = tech_stack[1].libcells[it.second.libcell_type].width;
            height = tech_stack[1].libcells[it.second.libcell_type].height;
        }       

        pair<float,float> p;
        if(it.second.part == PART::TOP)
            p = single_point_gradient(phi_top,solution_x(n)+static_cast<float>(width)/2,solution_y(n)+static_cast<float>(height)/2,it.second.part);
        else
            p = single_point_gradient(phi_bottom,solution_x(n)+static_cast<float>(width)/2,solution_y(n)+static_cast<float>(height)/2,it.second.part);
        
        demand_x(n) = p.first;
        demand_y(n) = p.second;
        move_force_mat_x(n,n) = w_i[n];
        move_force_mat_y(n,n) = w_i[n];
    }
    return;
}

void Kraftwerk2::update_pos_diff(fstream& fout){
    Eigen::MatrixXd C_x(size,size), C_y(size,size);

    C_x = connectivity_mat_x + move_force_mat_x;
    C_y = connectivity_mat_y + move_force_mat_y;
    Eigen::VectorXd b_x(size), b_y(size);
    Eigen::VectorXd delta_x(size), delta_y(size);
    b_x = move_force_mat_x * demand_x;
    b_y = move_force_mat_y * demand_y;
/*
    SimplicialLLT<SparseMatrix<double> > solver1;
    SimplicialLLT<SparseMatrix<double> > solver2;
    solver1.compute(C_x);
    solver2.compute(C_y);
    delta_x = solver1.solve(b_x);
    delta_y = solver2.solve(b_y);
*/
    delta_x = C_x.llt().solve(b_x);
    delta_y = C_y.llt().solve(b_y);
    solution_x = solution_x - delta_x;
    solution_y = solution_y - delta_y;
    cout << delta_x.norm() << endl;
    return;
}

void Kraftwerk2::Kraftwerk2_global_placement(unordered_map<string,instance>& ins, fstream& fout){
    cout << "global placement started\n";
    for(int i=0; i<50; i++){
        if(i%10 == 0){
            gen_connectivity_matrix(nets);
        }
        calc_gradient(ins,fout);
        update_pos_diff(fout);
        //print_solution(fout);
    }
    cout << "\nglobal placement finished. \n";
    get_solution(ins);
    
}

