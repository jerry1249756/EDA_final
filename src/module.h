#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
using namespace std;

class point{
    public:
        int x;
        int y;
        point(){ x=0; y=0; }
        point(int a, int b){ x=a; y=b; }
        point(const point& p1){ x = p1.x; y=p1.y; }
        point operator + (point);
        point operator - (point);
};

struct pin{
    string pin_name;
    point pin_pos;
};

class libcell{
    public:
        string cell_name;
        point cell_pos;
        vector<pin> pins;
        libcell();
        libcell(point);
        int width;
        int height;
        int get_area();
        void set_pos(point);
};

struct tech{
    public:
        string tech_name;
        vector<libcell> libcells;
};

int manhat_dist(point, point);

#endif