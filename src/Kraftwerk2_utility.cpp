#include "Kraftwerk2_utility.h"

//D should be a m*n zero matrix
void cal_D(unordered_map<string,instance> ins, vector<vector<float>>& D){
    //region = 0 represent top, region = 1 represent bottom
    int delta = min(top_row_height,bottom_row_height);
    for(auto it = ins.begin(); it != ins.end(); ++it){
        int n = stoi(it->first.substr(1))-1;    
        //cout << it->first << endl;
        if(it->second.tech == TECH::TECH_A){         
            int pos_x = 
            int pos_y = it->second.instance_pos.y;
            int width = tech_stack[0].libcells[it->second.libcell_type].width;
            int height = tech_stack[0].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(i >= (die_upper_x-die_lower_x) / delta || j >= (die_upper_y-die_lower_y) / delta){
                        continue;
                    }
                    D[i][j] += 1;
                }
            }
        }   
        if(it->second.tech == TECH::TECH_B){            
            int pos_x = it->second.instance_pos.x;
            int pos_y = it->second.instance_pos.y;
            int width = tech_stack[1].libcells[it->second.libcell_type].width;
            int height = tech_stack[1].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(i >= (die_upper_x-die_lower_x) / delta || j >= (die_upper_y-die_lower_y) / delta){
                        continue;
                    }
                    D[i][j] += 1;
                }
            }
        }
    }
    for(int i = 0; i < D.size(); i++){
        for(int j = 0; j < D[i].size(); j++){
             D[i][j] -= static_cast<float>(TOP_area+BOTTOM_area)/(2*die_area);
        }
    }
}

//phi should be m*n zero matrix
void cal_phi(vector<vector<float>> D, vector<vector<float>>& phi, int times){
    int delta = min(top_row_height,bottom_row_height);
    vector<vector<float>> temp;
    vector<vector<float>> temp3;
    temp.reserve(phi.size());
    temp3.reserve(phi.size());
    for(int i = 0; i < phi.size(); i++){
        vector<float> temp2;
        vector<float> temp4;
        temp2.reserve(phi[i].size());
        temp4.reserve(phi[i].size());
        for(int j = 0; j < phi[i].size(); j++){
            temp2.push_back(0);
            temp4.push_back(0);
        }
        temp.push_back(temp2);
        temp3.push_back(temp4);
    }
    for(int k = 0; k < times; k++){
        if(k%2 == 0){
            for(int i = 0; i < phi.size(); i++){
                for(int j = 0; j < phi[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i+1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi[i].size()-1){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i+1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i+1][j]*2) / 4;
                        }
                    }
                    else if(i == phi.size()-1){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi[i].size()-1){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i-1][j]*2) / 4;
                        }
                    }
                    else if(j == 0){
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j+1]*2 + temp[i+1][j]) / 4;
                    }
                    else if(j == phi[i].size()-1){
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1]*2) / 4; 
                    }
                    else{
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) / 4; 
                    }
                }
            }
        }
        else if(k%2 == 1){
            for(int i = 0; i < phi.size(); i++){
                for(int j = 0; j < phi[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i+1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi[i].size()-1){
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                        }
                    }
                    else if(i == phi.size()-1){
                        if(j == 0){
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i-1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi[i].size()-1){
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i-1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*delta*D[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i-1][j]) / 4;
                        }
                    }
                    else if(j == 0){
                        temp[i][j] = (delta*delta*D[i][j] + temp3[i-1][j] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                    }
                    else if(j == phi[i].size()-1){
                        temp[i][j] = (delta*delta*D[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                    }
                    else{
                        temp[i][j] = (delta*delta*D[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1] + temp3[i][j+1]) / 4; 
                    }
                }
            }
        }
    }
    if(times%2 == 0){
        for(int i = 0; i < phi.size(); i++){
            for(int j = 0; j < phi[i].size(); j++){
                phi[i][j] = temp[i][j];
            }
        }
    }
    else if(times%2 == 1){
        for(int i = 0; i < phi.size(); i++){
            for(int j = 0; j < phi[i].size(); j++){
                phi[i][j] = temp3[i][j];
            }
        }
    }
}