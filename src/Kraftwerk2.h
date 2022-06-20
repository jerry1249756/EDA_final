#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <random>
#include <fstream>
#include <unordered_map>
#include "module.h"
#include "LA.h"
#include "extern.h"
#include "enum.h"
using namespace std;

class Kraftwerk2{
    public:
        Kraftwerk2(int, unordered_map<string, instance>&);
        ~Kraftwerk2();
        void input_solution(unordered_map<string, instance>&);
        void cal_D(unordered_map<string,instance>,vector<vector<float>>&,vector<vector<float>>&);//int 0 represent top, int 1 represent bottom
        void cal_phi(vector<vector<float>>, vector<vector<float>>&,vector<vector<float>>, vector<vector<float>>&,int);
        int parse_inst_name(string);
        void gen_connectivity_matrix(unordered_map<string, net*>); //C
        void print_mat();
        void print_solution(fstream&);
        void get_solution(unordered_map<string, instance>&);
        pair<float, float> single_point_gradient(vector<vector<float>>, float, float, PART);
        void calc_gradient(unordered_map<string, instance>, fstream&); //C^O
        void update_pos_diff(int,fstream); //delta_x
        void Kraftwerk2_global_placement(unordered_map<string,instance>&,fstream&); //main algorithm
        
    private:
        int size;
        Matrix* connectivity_mat;
        Matrix* move_force_mat_x;
        Matrix* move_force_mat_y;
        Vector* solution_x;
        Vector* solution_y;
        Vector* demand_x;
        Vector* demand_y;
        vector<vector<float>> D_top;
        vector<vector<float>> D_bottom;
        vector<vector<float>> phi_top;
        vector<vector<float>> phi_bottom;
};

#endif