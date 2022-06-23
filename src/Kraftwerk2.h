#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <random>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include "module.h"
#include "extern.h"
#include "enum.h"
#include <Eigen/Dense>
// #include <Eigen/SparseCholesky>
using namespace std;

class Kraftwerk2{
    public:
        Kraftwerk2(unordered_map<string, instance>&);
        void input_solution(unordered_map<string, instance>&);
        void cal_D(unordered_map<string,instance>,vector<vector<float>>&,vector<vector<float>>&);//int 0 represent top, int 1 represent bottom
        void cal_phi(vector<vector<float>>, vector<vector<float>>&,vector<vector<float>>, vector<vector<float>>&,int);
        int parse_inst_name(string);
        void update_bound(string);
        void update_connectivity_x(int,int,string);
        void update_connectivity_y(int,int,string);
        void gen_connectivity_matrix(unordered_map<string, net*>); //C
        void print_mat();
        void print_solution(fstream&);
        void get_solution(unordered_map<string, instance>&);
        pair<float, float> single_point_gradient(vector<vector<float>>, float, float, PART);
        void calc_gradient(unordered_map<string, instance>, fstream&); //C^O
        void update_pos_diff(fstream&); //delta_x
        void Kraftwerk2_global_placement(unordered_map<string,instance>&,fstream&); //main algorithm
        
    private:
        int size;
        Eigen::MatrixXd connectivity_mat_x = Eigen::MatrixXd::Zero(Num_instance,Num_instance);
        Eigen::MatrixXd connectivity_mat_y = Eigen::MatrixXd::Zero(Num_instance,Num_instance);
        Eigen::MatrixXd move_force_mat_x = Eigen::MatrixXd::Zero(Num_instance,Num_instance);
        Eigen::MatrixXd move_force_mat_y = Eigen::MatrixXd::Zero(Num_instance,Num_instance);
        Eigen::VectorXd solution_x = Eigen::VectorXd::Zero(Num_instance);
        Eigen::VectorXd solution_y = Eigen::VectorXd::Zero(Num_instance);
        Eigen::VectorXd demand_x = Eigen::VectorXd::Zero(Num_instance);
        Eigen::VectorXd demand_y = Eigen::VectorXd::Zero(Num_instance);
        vector<vector<float>> D_top;
        vector<vector<float>> D_bottom;
        vector<vector<float>> phi_top;
        vector<vector<float>> phi_bottom;
        vector<double> w_i;
};

#endif