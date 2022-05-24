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
};

void libcell::set_pos(point p){
    cell_pos = p;
}

int manhat_dist(point a, point b){
    return abs(a.x-b.x)+abs(a.y-b.y);
}

net::net(string n,vector<string> i, vector<string> p){
            Net_name = n;
            instance = i;
            pin = p;
        }; 