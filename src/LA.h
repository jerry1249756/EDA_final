#ifndef LA_H
#define LA_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Vector{
    public:
        Vector(int);
        Vector(const Vector&);
        void print_data(fstream&);
        int size;
        vector<float> data;
};

class Matrix{
    public:
        Matrix(int);
        Matrix(const Matrix&);
        void PLU_decomposition(Matrix& L, Matrix& U, Matrix& P);
        void exchange_rows(int, int);
        void print_data();
        int size;
        vector<vector<float>> data;
};

void Matrix_scalar(Matrix& A, float scalar);
void Matrix_Addition(Matrix& A, Matrix& B, Matrix& C);
void Matrix_Vector_Prod(Matrix& A, Vector& v, Vector& c);
void solve_linear_system(Matrix& P, Matrix& L, Matrix& U, Vector& b, Vector& x);

#endif
