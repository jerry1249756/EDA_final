#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <random>
#include "module.h"
#include "LA.h"
using namespace std;

extern vector<tech> tech_stack;
extern int die_lower_x, die_lower_y, die_upper_x, die_upper_y;

class Kraftwerk2{
    public:
        Kraftwerk2(int, int, unordered_map<string, instance>&);
        ~Kraftwerk2();
        int parse_inst_name(string);
        void gen_connectivity_matrix(unordered_map<string, net*>); //C
        void print_mat();
        void get_solution(unordered_map<string, instance>&);
        void calc_gradient(vector<vector<float>>, int, unordered_map<string, instance>); //C^O
        void update_pos_diff(); //delta_x
        void Kraftwerk2_global_placement(); //main algorithm

        Vector* demand_x;
        Vector* demand_y; 
    private:
        int size;
        int iter_param;
        Matrix* connectivity_mat;
        Matrix* move_force_mat_x;
        Matrix* move_force_mat_y;
        Vector* solution_x;
        Vector* solution_y;
};

#endif