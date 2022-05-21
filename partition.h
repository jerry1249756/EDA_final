#ifndef PARTITION_H
#define PARTITION_H

#include "module.h"
#include <utility>
#include <vector>
#include <unordered_map>

enum class PART{TECH_A=0, TECH_B=1};
enum class LOCK_STATE{UNLOCKED=0, LOCKED=1};

typedef pair<int,int> distribution;
#define A first 
#define B second

enum class FM_RESULT{FM_ERROR=-1, FM_SUCCESS=1};

class cell_node{
    public:
        cell_node();
        string node_name;
        PART part;
        LOCK_STATE state;
        int area;
        int gain;
        vector<net*> connected_nets;
        void add_net(net*);
        void set_gain();
        void show_data();
};

class net{
    public:
        net();
        string net_name;
        distribution Dist;
        vector<cell_node*> connected_nodes;
        void add_node(cell_node*);
        bool is_critical();
        // void recalc_dist();
        void show_data();
};

//dictionary: key: <cell_name, tech_name> value: corresponding libcell
unordered_map<pair<string, string>, libcell> dictionary;

#endif