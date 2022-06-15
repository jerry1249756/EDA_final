#include "LA.h"

Vector::Vector(int n){
    data.resize(n);
    size = n;
}

Vector::Vector(const Vector& v1){
    int n = v1.size;
    data.resize(n);
    for(int i=0;i<n; i++){
        data[i] = v1.data[i];
    }
    size = n;
}

void Vector::print_data(fstream& fout){
    for(int i=0; i<size; i++){
        fout << data[i] << " ";
    }
    fout << "\n";
}


Matrix::Matrix(int n){
    data.resize(n);
    for(int i=0; i<n; i++){
        data[i].resize(n);
    }
    size = n;
}

Matrix::Matrix(const Matrix& m1){
    int n = m1.size;
    data.resize(n);
    for(int i=0; i<n; i++){
        data[i].resize(n);
    }
    for(int i=0;i<n; i++){
        for(int j=0; j<n; j++){
            data[i][j] = m1.data[i][j];
        }
    }
    size = n;
}

void Matrix::exchange_rows(int a, int b){
    for(int i=0; i<size; i++){
        float temp = data[a][i];
        data[a][i] = data[b][i];
        data[b][i] = temp;
    }
    return;
}

void Matrix::print_data(){
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << data[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Matrix::PLU_decomposition(Matrix& L, Matrix& U, Matrix& P){ //PA=LU
    //L U P will be modified after this
    int n=size; 
    for(int i=0; i<n; i++){
        P.data[i][i] = 1;
    }
    for(int k=0; k<n; k++){
        float p=0;
        int k_prime = k;
        for(int i=k; i<n; i++){
            if(abs(data[i][k])>p){
                p = abs(data[i][k]);
                k_prime = i;
            }
        }
        if(p==0) cout << "Error in PLU_Decomposition: singular matrix\n";
        else{
            P.exchange_rows(k, k_prime);
            exchange_rows(k, k_prime);  
        }
        for(int i=k+1; i<n; i++){
            data[i][k] = data[i][k] /data[k][k];
            for(int j=k+1; j<n; j++){
                data[i][j] = data[i][j] - data[i][k]*data[k][j];
            } 
        }
    }
    for(int i=0; i<n;i++){
        for(int j=0; j<n; j++){
            if(i>j)L.data[i][j] = data[i][j];
            else{
                if(i==j) L.data[i][j] = 1;           
                U.data[i][j] = data[i][j];
            }
        }
    }
    return;
}

void Matrix_Addition(Matrix& A, Matrix& B, Matrix& C){
    int n=A.size;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            C.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }
    return ;
}

void Matrix_Vector_Prod(Matrix& A, Vector& v, Vector& c){ //A(n*n) B(n*1) output C(n*1)
    int n = A.size;
    for(int i=0; i<n; i++){
        c.data[i] = 0;
        for(int k=0; k<n; k++){
            c.data[i] = A.data[i][k]*v.data[k]; 
        }
    }
    return;
}

void solve_linear_system(Matrix& P, Matrix& L, Matrix& U, Vector& b, Vector& x){ //PAx=Pb -> LUx = Pb
    //P,L,U are size of (n,n), b is of (n,1)
    int n = b.size;
    Vector b_prime(n), y(n);
    for(int i=0; i<n; i++){
        for(int k=0; k<n; k++){
            b_prime.data[i] += P.data[i][k]*b.data[k]; 
        }
    }
    
    Matrix_Vector_Prod(P, b, b_prime);
    for(int i=0; i<n; i++){
        y.data[i] = b_prime.data[i];
        for(int j=0; j<=i-1; j++){
            y.data[i] -= L.data[i][j]*y.data[j];
        } 
    }
    for(int i=n-1; i>=0; i--){
        float sum = 0;
        for(int j=i+1; j<n; j++){
            sum += U.data[i][j]*x.data[j];
        }
        x.data[i] = (y.data[i] - sum)/U.data[i][i]; 
    }
    return;
}