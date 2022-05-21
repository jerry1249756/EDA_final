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

libcell::libcell(point p){
    set_pos(p);
};

int libcell::get_area(){
    return width*height;
};

void libcell::set_pos(point p){
    cell_pos = p;
}

int manhat_dist(point a, point b){
    return abs(a.x-b.x)+abs(a.y-b.y);
}