#ifndef LA_H
#define LA_H

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Vector{
    public:
        Vector(int);
        Vector(const Vector&);
        void print_data(fstream&);
        int size;
        vector<double> data;
};

class Matrix{
    public:
        Matrix(int);
        Matrix(const Matrix&);
        void PLU_decomposition(Matrix& L, Matrix& U, Matrix& P);
        void Cholesky_decomposition(Matrix& L, Matrix& L_T);
        void exchange_rows(int, int);
        void print_data();
        int size;
        vector<vector<double>> data;
};

double inner_product(Vector, Vector);
double norm(Vector&);
Vector Vector_addition(Vector& a, Vector& b);
Vector Vector_scalar(Vector& a, float scalar);
void Matrix_scalar(Matrix& A, float scalar);
void Matrix_Addition(Matrix& A, Matrix& B, Matrix& C);
void Matrix_Vector_Prod(Matrix& A, Vector& v, Vector& c);
void solve_linear_system(Matrix& P, Matrix& L, Matrix& U, Vector& b, Vector& x);
void solve_linear_system_via_Cholesky(Matrix& L, Matrix& L_T, Vector& b, Vector& x);
Vector conjugate_gradient(Matrix& A, Vector& b);


#endif
