#ifndef _TECH_H
#define _TECH_H

#include <vector>
#include <string>

struct pin
{
    std::string pin_name;
    int pin_x;
    int pin_y;
};

struct libcell
{
    std::string libcell_name;
    int lib_x;
    int lib_y;
    std::vector<pin> pin;
};

struct tech
{
    std::string tech_name;
    std::vector<libcell> libcell;
};

struct die_size
{
    int lower_x;
    int lower_y;
    int upper_x;
    int upper_y;
};

struct die_rows
{
    int start_x;
    int start_y;
    int row_length;
    int row_height;
    int repeat_count;
};

#endif