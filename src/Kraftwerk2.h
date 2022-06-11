#ifndef KRAFTWERK2_H
#define KRAFTWERK2_H

#include <iostream>
#include "module.h"
using namespace std;
//    unordered_map<string, instance> instances;
//    unordered_map<string, net> nets;

class Matrix{
    public:
        Matrix(int);
        Matrix(const Matrix& );
        ~Matrix();
        void LU_decomp(Matrix&, Matrix&);
        void print_data();
        float **data;
        int size;
};

class Vector{
    public:
        Vector(int);
        Vector(const Vector&);
        ~Vector();
        Vector Matrix_Vector_Mul(Matrix, Vector);

};

class Kraftwerk2{
    public:
        Kraftwerk2(int);
        Matrix* Connectivity_mat;
        int Parse_Inst_Name(string);
        void Generate_Connectivity_matrix(unordered_map<string, net>);
};

#endif