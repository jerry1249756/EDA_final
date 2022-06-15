#ifndef Neighborhood_H
#define Neighborhood_H

class Neighborhood{
    public:
    long long int calc_cost(unordered_map<string, instance>&, unordered_map<string, net*>);
    void single_net_cost(unordered_map<string, instance>&, unordered_map<string, net*>, string);
    void place_instance_to_each_row(unordered_map<string, instance>&, vector<vector<int>>&, vector<vector<int>>&);
    bool row_compare(int, int);
    void sort_row(vector<vector<int>>&, vector<vector<int>>&);
    void sort_single_row(vector<vector<int>>&, int);
    int single_row_penalty(vector<vector<int>>&, int);
    long long int init_penalty(vector<vector<int>>&, vector<vector<int>>&);
    int swap_cost(unordered_map<string, instance>&, unordered_map<string, net*>, vector<vector<int>>&, int, int, int, int);
    int two_die_swap_cost(unordered_map<string, instance>&, unordered_map<string, net*>, vector<vector<int>>&, vector<vector<int>>&, int, int, int, int);
    void swap_instance(int, int);
    void two_die_swap_instance(int, int);
    int move_in_row_cost(unordered_map<string, instance>&, unordered_map<string, net*>, vector<vector<int>>&, int, int, int);
    int move_between_row_cost(unordered_map<string, instance>&, unordered_map<string, net*>, vector<vector<int>>&, int, int, int);
    int two_die_move_cost(unordered_map<string, instance>&, unordered_map<string, net*>, vector<vector<int>>&, vector<vector<int>>&, int, int, int, int);
    void update_swap(vector<vector<int>>&, vector<vector<int>>&, int, int, int, int);
    void update_move_in_row(vector<vector<int>>&, int, int, int);
    void update_move_between_row(vector<vector<int>>&, int, int, int);
    void update_two_die_move(vector<vector<int>>&, vector<vector<int>>&, int, int);
};

#endif