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
point point::operator = (point temp){
    point res;
    res.x = temp.x;
    res.y = temp.y;
    return res;
}
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
    int top_left_num = INT32_MAX;
    int top_right_num = INT32_MIN;
    int top_up_num = INT32_MIN;
    int top_bottom_num = INT32_MAX;
    int bottom_left_num = INT32_MAX;
    int bottom_right_num = INT32_MIN;
    int bottom_up_num = INT32_MIN;
    int bottom_bottom_num = INT32_MAX;
    
    for(int i = 0; i < net_pin.size(); i++){
        if(instances[net_pin[i].first].part == PART::TOP){
            if(instances[net_pin[i].first].tech == TECH::TECH_A){
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < top_left_num){
                    top_left_num = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    top_left = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > top_right_num){
                    top_right_num = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    top_right = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < top_bottom_num){
                    top_bottom_num = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    top_bottom = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > top_up_num){
                    top_up_num = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    top_up = net_pin[i].first;
                }
            }
            else{
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < top_left_num){
                    top_left_num = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    top_left = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > top_right_num){
                    top_right_num = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    top_right = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < top_bottom_num){
                    top_bottom_num = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    top_bottom = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > top_up_num){
                    top_up_num = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    top_up = net_pin[i].first;
                }
            }
        }
        else{
            if(instances[net_pin[i].first].tech == TECH::TECH_A){
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < bottom_left_num){
                    bottom_left_num = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    bottom_left = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > bottom_right_num){
                    bottom_right_num = instances[net_pin[i].first].instance_pos.x + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    bottom_right = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < bottom_bottom_num){
                    bottom_bottom_num = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    bottom_bottom = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > bottom_up_num){
                    bottom_up_num = instances[net_pin[i].first].instance_pos.y + tech_stack[0].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    bottom_up = net_pin[i].first;
                }
            }
            else{
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) < bottom_left_num){
                    bottom_left_num = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    bottom_left = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x) > bottom_right_num){
                    bottom_right_num = instances[net_pin[i].first].instance_pos.x + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.x;
                    bottom_right = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) < bottom_bottom_num){
                    bottom_bottom_num = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    bottom_bottom = net_pin[i].first;
                }
                if((instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y) > bottom_up_num){
                    bottom_up_num = instances[net_pin[i].first].instance_pos.y + tech_stack[1].libcells[instances[net_pin[i].first].libcell_type].pins[net_pin[i].second].pin_pos.y;
                    bottom_up = net_pin[i].first;
                }
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






