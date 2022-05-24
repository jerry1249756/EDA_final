#ifndef PARTITION_H
#define PARTITION_H

//#include "module.h"
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <unordered_map>

using namespace std;

enum class PART{TECH_A=0, TECH_B=1};
enum class LOCK_STATE{UNLOCKED=0, LOCKED=1};

typedef pair<int,int> distribution;
#define A first 
#define B second

class net; //forward declaration for two types pointer using together

class cell_node{
    public:
        cell_node(string, int); //cell_node(string, string);
        string node_name;
        // string libcell_type; //TODO
        PART part;
        LOCK_STATE state;
        int area;
        int gain;
        vector<net*> connected_nets;
        bool operator==(const cell_node &) const;
        void set_gain();
        void show_data();
};

class net{
    public:
        net(string);
        string net_name;
        distribution Dist;
        vector<cell_node*> connected_nodes;
        void add_node(cell_node*);
        bool is_cut();
        bool is_critical();
        void recalc_dist();
        void show_data();
};

//dictionary: key: <cell_name, tech_name> value: corresponding libcell
//unordered_map<pair<string, string>, libcell> dictionary; //TODO

#endif