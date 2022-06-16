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
