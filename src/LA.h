#ifndef LA_H
#define LA_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

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
        void PLU_decomposition(Matrix& L, Matrix& U, Matrix& P);
        void exchange_rows(int, int);
        void print_data();
        int size;
        vector<vector<double>> data;
};

Matrix Matrix_Addition(Matrix, Matrix);
Vector Matrix_Vector_Prod(Matrix, Vector);
void solve_linear_system(Matrix&, Matrix&, Matrix&, Vector&, Vector&);

#endif
