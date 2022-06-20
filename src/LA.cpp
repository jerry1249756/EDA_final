#include "LA.h"

Vector::Vector(int n){
    data.resize(n);
    for(int i=0;i<n; i++){
        data[i] = 0;
    }
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
    for(int i=0;i<data.capacity(); i++){
        for(int j=0; j<data[i].capacity(); j++){
            data[i][j] = 0;
        }
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
        double temp = data[a][i];
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
        double p=0;
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

void Matrix::Cholesky_decomposition(Matrix& L, Matrix& L_T){ //asserting L is zero matrix
    int n = size;
    for(int i=0; i<n; ++i){
        for(int k=0; k<i; ++k){
            float val = data[i][k];
            for(int j=0; j<k ; ++j){
                val -= L.data[i][j]*L.data[k][j];
            }
            L.data[i][k] = val/L.data[k][k];
        }
        float val2 = data[i][i];
        for(int j=0; j<i; ++j){
            val2 -= L.data[i][j]*L.data[i][j];
        }
        L.data[i][i] = sqrt(val2);
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            L_T.data[i][j] = L.data[j][i];
        }
    }
    return;
}

double inner_product(Vector a, Vector b){
    int n = a.size;
    double result = 0;
    for(int i=0; i<n; i++){
        result += static_cast<double>(a.data[i])*static_cast<double>(b.data[i]);
    }
    return result;
}

double norm(Vector& a){
    int n = a.size;
    double result = 0;
    for(int i=0; i<n; i++){
        result += a.data[i]*a.data[i];
    }
    result = sqrt(result);
    return result;
}

Vector Vector_addition(Vector& a, Vector& b){
    int n=a.size;
    Vector C(n);
    for(int i=0; i<n; i++){
        C.data[i] = a.data[i] + b.data[i];
    }
    return C;
}

Vector Vector_scalar(Vector& a, float scalar){
    int n=a.size;
    Vector C(n);
    for(int i=0; i<n; i++){
        C.data[i] = a.data[i]*scalar;
    }
    return C;
}

void Matrix_scalar(Matrix& A, float scalar){
    int n=A.size;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            A.data[i][j] = A.data[i][j] * scalar;
        }
    }
    return ; 
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
            c.data[i] += A.data[i][k]*v.data[k]; 
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
        double sum = 0;
        for(int j=i+1; j<n; j++){
            sum += U.data[i][j]*x.data[j];
        }
        x.data[i] = (y.data[i] - sum)/U.data[i][i]; 
    }
    return;
}

void solve_linear_system_via_Cholesky(Matrix& L, Matrix& L_T, Vector& b, Vector& x){ 
    //P,L,U are size of (n,n), b is of (n,1)
    int n = b.size;
    Vector  y(n);

    for(int i=0; i<n; i++){
        y.data[i] = b.data[i];
        for(int j=0; j<=i-1; j++){
            y.data[i] -= L.data[i][j]*y.data[j];
        } 
    }
    for(int i=n-1; i>=0; i--){
        double sum = 0;
        for(int j=i+1; j<n; j++){
            sum += L_T.data[i][j]*x.data[j];
        }
        x.data[i] = (y.data[i] - sum)/L_T.data[i][i]; 
    }
    return;
}

Vector conjugate_gradient(Matrix& A, Vector& b){
    double TOLERANCE = norm(b) / 1000;
    int n = A.size;
    Vector X(n), R(n), P(n), A_P(n);
    R = b;
    P = R;
    int k=0;
    while(k<n){
        Vector R_hold = R;
        Matrix_Vector_Prod(A, P, A_P);
        double alpha = inner_product(R,R)/max(inner_product(P, A_P),TOLERANCE);
        Vector temp_x = Vector_scalar(P, alpha);
        Vector temp_r = Vector_scalar(A_P, -1*alpha);
        X = Vector_addition(X, temp_x);
        R = Vector_addition(R, temp_r);
        if (norm(R) < TOLERANCE) break;
        double beta = inner_product(R,R)/max(inner_product(R_hold, R_hold),TOLERANCE);
        Vector temp_p = Vector_scalar(P, beta);
        P = Vector_addition(R, temp_p);
        k+=1;
    }
    return X;
}