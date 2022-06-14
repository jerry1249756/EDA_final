#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <random>
#include "module.h"
#include "Kraftwerk2_utility.h"
#include "LA.h"
#include "extern.h"
using namespace std;

class Kraftwerk2{
    public:
        Kraftwerk2(int, unordered_map<string, instance>&);
        ~Kraftwerk2();
        int parse_inst_name(string);
        void gen_connectivity_matrix(unordered_map<string, net*>); //C
        void print_mat();
        void print_solution();
        void get_solution(unordered_map<string, instance>&);
        void calc_gradient(unordered_map<string, instance>); //C^O
        void update_pos_diff(); //delta_x
        void Kraftwerk2_global_placement(unordered_map<string,instance>&); //main algorithm

        Vector* demand;
    private:
        int size;
        Matrix* connectivity_mat;
        Matrix* move_force_mat_x;
        Matrix* move_force_mat_y;
        Vector* solution_x;
        Vector* solution_y;
        vector<vector<float>> D;
        vector<vector<float>> phi;
};

#endif