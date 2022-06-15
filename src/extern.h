#ifndef EXTERN_H
#define EXTERN_H

#include <vector>
#include <string>
#include "module.h"
using namespace std;

extern vector<tech> tech_stack; //tech_stack[0] is TA; tech_stack[1] is TB if it exists.
extern unsigned long long int die_area;
extern unsigned int top_die_max_util, bottom_die_max_util;
extern unsigned int top_repeat_count, bottom_repeat_count; 
extern string top_die_tech, bottom_die_tech;
extern int die_lower_x, die_lower_y, die_upper_x, die_upper_y;
extern int top_row_height, bottom_row_height;
extern unsigned long long int TOP_area, BOTTOM_area;

#endif