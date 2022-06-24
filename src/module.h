#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "enum.h"

using namespace std;

//<instance_name, pin_name>
typedef pair<string,string> ip;
#define INSTANCE first 
#define PIN second

typedef pair<int,int> distribution;

class net;

class point{
    public:
        float x;
        float y;
        point(){ x=0; y=0; }
        point(int a, int b){ x=a; y=b; }
        point(const point& p1){ x = p1.x; y=p1.y; }
        point operator + (point);
        point operator - (point);
};

struct pin{
    string pin_name;
    point pin_pos;
    string pin_net;
};

class libcell{
    public:
        string cell_name;
        unordered_map<string, pin> pins;
        libcell(){};
        libcell(int w,int h):width(w),height(h){};
        int width;
        int height;
        int get_area();
};

class instance{
    public: 
        instance(){};
        instance(string t):libcell_type(t){};
        point instance_pos = point(0,0);
        string libcell_type;
        vector<net*> connected_nets;
        PART part; 
        TECH tech;
        void set_pos(point);
};

struct tech{
    string tech_name;
    unordered_map<string,libcell> libcells;
};

struct die_size{
    int lower_x;
    int lower_y;
    int upper_x;
    int upper_y;
};

struct die_rows{
    int start_x;
    int start_y;
    int row_length;
    int row_height;
    unsigned int repeat_count;
};

class net{
    public:
        net(){};
        string Net_name;
        vector<ip> net_pin;
        void add_ip(string, string, unordered_map<string, instance>&);
        void update_bound(unordered_map<string,instance>&, vector<tech>&);
        int cost;
        distribution dist;
        bool is_cut();
        point terminal_pos = point(0,0);
        string top_left;
        string top_right;
        string top_up;
        string top_bottom;
        string bottom_left;
        string bottom_right;
        string bottom_up;
        string bottom_bottom;
};

int manhat_dist(point, point);
int split_num(string);
pair<string, string> split_string(string&);
#endif