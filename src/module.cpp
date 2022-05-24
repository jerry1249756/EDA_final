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

void instance::set_pos(point p){
    instance_pos = p;
};

int manhat_dist(point a, point b){
    return abs(a.x-b.x)+abs(a.y-b.y);
};

int split1(string a){
    int i = 0;
    while(true){
        if(a[i] == '/')
            break;
        i++;
    }
    return i;
};