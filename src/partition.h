#ifndef PARTITION_H
#define PARTITION_H

#include <iostream>
#include <utility>
#include <random>
#include "module.h"

using namespace std;

enum class LOCK_STATE{UNLOCKED=0, LOCKED=1};

typedef pair<int,int> distribution;
#define A first 
#define B second

class partition_net; //forward declaration for two types pointer using together

class cell_node{
    public:
        cell_node(string, string);
        string node_name;
        string libcell_type; //TODO
        PART part;
        LOCK_STATE state;
        int area;
        int gain;
        vector<partition_net*> connected_nets;
        bool operator==(const cell_node &) const;
        void set_gain();
        void show_data();
        void update_area();
};

class partition_net{
    public:
        partition_net(string);
        string net_name;
        distribution Dist;
        vector<cell_node*> connected_nodes;
        
        void add_node(cell_node*);
        bool is_cut();
        bool is_critical();
        void recalc_dist();
        void show_data();
};

#endif