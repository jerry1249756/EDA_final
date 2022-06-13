#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <random>
#include "module.h"
#include "Kraftwerk2_utility.h"
#include "LA.h"
using namespace std;

extern vector<tech> tech_stack;
extern int top_row_height, bottom_row_height;
extern unsigned int top_repeat_count, bottom_repeat_count; 
extern int die_lower_x, die_lower_y, die_upper_x, die_upper_y;

class Kraftwerk2{
    public:
        Kraftwerk2(int, unordered_map<string, instance>&, PART);
        ~Kraftwerk2();
        int parse_inst_name(string);
        void gen_connectivity_matrix(unordered_map<string, net*>); //C
        void print_mat();
        void print_solution();
        void get_solution(unordered_map<string, instance>&);
        void calc_gradient(int, unordered_map<string, instance>); //C^O
        void update_pos_diff(); //delta_x
        void Kraftwerk2_global_placement(unordered_map<string,instance>&); //main algorithm

        Vector* demand;
    private:
        int size;
        PART part;
        Matrix* connectivity_mat;
        Matrix* move_force_mat_x;
        Matrix* move_force_mat_y;
        Vector* solution_x;
        Vector* solution_y;
        vector<vector<float>> D;
        vector<vector<float>> phi;
};

#endif