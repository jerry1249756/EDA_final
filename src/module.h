#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "partition.h"
using namespace std;

typedef pair<string,string> ip;
#define INSTANCE first 
#define PIN second

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
    string pin_net;
};

class libcell{
    public:
        string cell_name;
        vector<pin> pins;
        libcell(){};
        libcell(int w,int h):width(w),height(h){};
        int width;
        int height;
        int get_area();
};

class instance{
    public: 
        point instance_pos;
        string libcell_type;
        PART tech;
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
    int repeat_count;
};

class net{
    public:
        net(){};
        string Net_name;
        vector<ip> net_pin;
};

int manhat_dist(point, point);


const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}
#endif