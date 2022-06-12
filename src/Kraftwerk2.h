#include <iostream>
#include <cmath>
#include <vector>
//#include "module.h"
using namespace std;
// unordered_map<string, instance> instances;
// unordered_map<string, net> nets;
// #include <Eigen/Core>
// using namespace Eigen;

class Vector{
    public:
        Vector(int);
        Vector(const Vector&);
        void print_data();
        int size;
        vector<double> data;
};

class Matrix{
    public:
        Matrix(int);
        Matrix(const Matrix&);
        void PLU_decomposition(Matrix&, Matrix&, Matrix&);
        void exchange_rows(int, int);
        void print_data();
        int size;
        vector<vector<double>> data;
};

Vector Matrix_Vector_Prod(Matrix, Vector);
void solve_linear_system(Matrix&, Matrix&, Matrix&, Vector&, Vector&);


class Kraftwerk2{
    public:
        Kraftwerk2(int);
        Matrix* Connectivity_mat;
        int Parse_Inst_Name(string);
        void Generate_Connectivity_matrix(unordered_map<string, net>);
        void Print_Mat();
};