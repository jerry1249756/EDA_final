#include "Kraftwerk2.h"

Matrix::Matrix(int n){
    size = n;
    data = new float* [n];
    for(int i=0; i<n; i++){
        data[i] = new float[n];
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            data[i][j]=0;
        }
    }
}

Matrix::Matrix(const Matrix& m1){//copy constructor
    size = m1.size;
    data = new float* [size]; //allocate memory
    for(int i=0; i<size; i++){
        data[i] = new float[size];
    }
    for(int i=0; i<size; i++){  //copy
        for(int j=0; j<size; j++){
            data[i][j] = m1.data[i][j];
        }
    }

}
Matrix::~Matrix(){
    for(int i=0; i<size; i++){
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::LU_decomp(Matrix& L, Matrix& U){
    for(int k=0; k<size; k++){
        L.data[k][k] = 1; 
    }
    for(int k=0; k<size; k++){
        U.data[k][k] = data[k][k];
        for(int i=k+1; i<size; i++){
            L.data[i][k] = data[i][k]/data[k][k];
            U.data[k][i] = data[k][i];
        } 
        for(int i=k+1; i<size; i++){
            for(int j=k+1; j<size; j++){
                data[i][j] -= L.data[i][k]* U.data[k][j];
            }
        }
    }
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

Kraftwerk2::Kraftwerk2(int n){
    Connectivity_mat = new Matrix(n);
}

int Kraftwerk2::Parse_Inst_Name(string s){ //input: String("M12"), output: int 12
    return stoi(s.substr(1));
}

void Kraftwerk2::Generate_Connectivity_matrix(unordered_map<string, net> map){
    for(auto& it : map){
        int n =it.second.net_pin.size();
        for(int i=0; i<n; i++){
            for(int j = i+1; j<n; j++){
                Connectivity_mat->data[i][j]+=1;
                Connectivity_mat->data[j][i]+=1;
            }
        }
    }
}

