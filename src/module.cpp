#include "module.h"

point point::operator + (point temp){
    point res;
    res.x = x + temp.x;
    res.y = y + temp.y;
    return res;
};

point point::operator - (point temp){
    point res;
    res.x = x - temp.x;
    res.y = y - temp.y;
    return res;
};
/*
libcell::libcell(int w,int h){
    width = w;
    height = h;
};
*/
int libcell::get_area(){
    return width*height;
}

void instance::set_pos(point p){
    instance_pos = p;
}

void net::add_ip(string inst_name, string pin_name, unordered_map<string, instance>& dict){
    net_pin.push_back(make_pair(inst_name, pin_name));
    dict[inst_name].connected_nets.push_back(this);
}

bool net::is_cut(){  //first: top, second: bottom
    if(dist.first==0 || dist.second==0) return false;
    else return true;
}


void net::update_bound(unordered_map<string,instance>& instances, vector<tech>& tech_stack){
    int left = INT32_MAX;
    int right = INT32_MIN;
    int up = INT32_MIN;
    int bottom = INT32_MAX;
    for(int i = 0; i < net_pin.size(); i++){
        if(instances[net_pin[i].first].tech == TECH::TECH_A){
            if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < left){
                left = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                left_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > right){
                right = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                right_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < bottom){
                bottom = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                bottom_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > up){
                up = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                up_index = i;
            }
        }
        else{
            if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < left){
                left = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                left_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > right){
                right = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                right_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < bottom){
                bottom = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                bottom_index = i;
            }
            if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > up){
                up = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                up_index = i;
            }
        }
    }
}

int manhat_dist(point a, point b){
    return abs(a.x-b.x)+abs(a.y-b.y);
}

int split_num(string a){
    int i = 0;
    while(true){
        if(a[i] == '/')
            break;
        i++;
    }
    return i;
}

pair<string, string> split_string(string& s){
    int temp = split_num(s);
    string a = s.substr(0, temp);
    s.erase(0, temp+1);
    return make_pair(a,s);
}
