#include "Legalization.h"

void stretch(vector<vector<int>>& top_die_row, vector<vector<int>>& bottom_die_row){
    Neighborhood nei;
    int toptech, bottomtech;
    if(top_die_tech == "TA" && bottom_die_tech == "TB"){
        toptech = 0;
        bottomtech = 1;
    }
    else if(top_die_tech == "TB" && bottom_die_tech == "TA"){
        toptech = 1;
        bottomtech = 0;
    }
    else{
        toptech = 0;
        bottomtech = 0;
    }
    int probeL, probeR, probeT, probeB;
    int L, R;

    // top legalization
    for(int i=0; i<top_die_row.size(); i++){
        probeB = 0;
        probeT = top_die_row.size()-1-i;
        probeL = die_lower_x;
        probeR = die_upper_x;
        if(probeT >= probeB){
            L = 0;
            R = top_die_row[probeT].size()-1;
            while(probeL < probeR && L <= R){
                if(instances["C"+to_string(top_die_row[probeT][L])].instance_pos.x >= probeL){
                    probeL = instances["C"+to_string(top_die_row[probeT][L])].instance_pos.x + tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][L])].libcell_type].width;
                }
                else{
                    instances["C"+to_string(top_die_row[probeT][L])].instance_pos.x = probeL;
                    probeL += tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][L])].libcell_type].width;
                }
                L++;
            }
            probeL = die_lower_x;
            L = 0;
            while(probeR > probeL && R >= L){
                if(instances["C"+to_string(top_die_row[probeT][R])].instance_pos.x + tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][R])].libcell_type].width <= probeR){
                    probeR = instances["C"+to_string(top_die_row[probeT][R])].instance_pos.x;
                }
                else{
                    instances["C"+to_string(top_die_row[probeT][R])].instance_pos.x = probeR - tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][R])].libcell_type].width;
                    probeR -= tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][R])].libcell_type].width;
                }
                R--;
            }
            if(probeR < probeL && probeT != probeB){
                for(int j=0; j<=R+1; j++){
                    instances["C"+to_string(top_die_row[probeT][0])].instance_pos.y -= top_row_height;
                    top_die_row[probeT-1].push_back(top_die_row[probeT][0]);
                    top_die_row[probeT].erase(top_die_row[probeT].begin());
                }
                nei.sort_single_row(top_die_row, probeT-1);
            }
        }
    }
    for(int i=0; i<top_die_row.size(); i++){
        probeB = i;
        probeT = top_die_row.size()-1;
        probeL = die_lower_x;
        probeR = die_upper_x;
        if(probeB <= probeT){
            L = 0;
            R = top_die_row[probeB].size()-1;
            while(probeL < probeR && L <= R){
                if(instances["C"+to_string(top_die_row[probeB][L])].instance_pos.x >= probeL){
                    probeL = instances["C"+to_string(top_die_row[probeB][L])].instance_pos.x + tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeB][L])].libcell_type].width;
                }
                else{
                    instances["C"+to_string(top_die_row[probeB][L])].instance_pos.x = probeL;
                    probeL += tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeB][L])].libcell_type].width;
                }
                L++;
            }
            probeL = die_lower_x;
            L = 0;
            while(probeR > probeL && R >= L){
                if(instances["C"+to_string(top_die_row[probeB][R])].instance_pos.x + tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeB][R])].libcell_type].width <= probeR){
                    probeR = instances["C"+to_string(top_die_row[probeB][R])].instance_pos.x;
                }
                else{
                    instances["C"+to_string(top_die_row[probeB][R])].instance_pos.x = probeR - tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeB][R])].libcell_type].width;
                    probeR -= tech_stack[toptech].libcells[instances["C"+to_string(top_die_row[probeT][R])].libcell_type].width;
                }
                R--;
            }
            if(probeR < probeL && probeB != probeT){
                for(int j=0; j<=R+1; j++){
                    instances["C"+to_string(top_die_row[probeB][0])].instance_pos.y += top_row_height;
                    top_die_row[probeB+1].push_back(top_die_row[probeB][0]);
                    top_die_row[probeB].erase(top_die_row[probeB].begin());
                }
                nei.sort_single_row(top_die_row, probeB+1);
            }
        }
    }

    // bottom legalization
    for(int i=0; i<bottom_die_row.size(); i++){
        probeB = 0;
        probeT = bottom_die_row.size()-1-i;
        probeL = die_lower_x;
        probeR = die_upper_x;
        if(probeT >= probeB){
            L = 0;
            R = bottom_die_row[probeT].size()-1;
            while(probeL < probeR && L <= R){
                if(instances["C"+to_string(bottom_die_row[probeT][L])].instance_pos.x >= probeL){
                    probeL = instances["C"+to_string(bottom_die_row[probeT][L])].instance_pos.x + tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][L])].libcell_type].width;
                }
                else{
                    instances["C"+to_string(bottom_die_row[probeT][L])].instance_pos.x = probeL;
                    probeL += tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][L])].libcell_type].width;
                }
                L++;
            }
            probeL = die_lower_x;
            L = 0;
            while(probeR > probeL && R >= L){
                if(instances["C"+to_string(bottom_die_row[probeT][R])].instance_pos.x + tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][R])].libcell_type].width <= probeR){
                    probeR = instances["C"+to_string(bottom_die_row[probeT][R])].instance_pos.x;
                }
                else{
                    instances["C"+to_string(bottom_die_row[probeT][R])].instance_pos.x = probeR - tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][R])].libcell_type].width;
                    probeR -= tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][R])].libcell_type].width;
                }
                R--;
            }
            if(probeR < probeL && probeT != probeB){
                for(int j=0; j<=R+1; j++){
                    instances["C"+to_string(bottom_die_row[probeT][0])].instance_pos.y -= bottom_row_height;
                    bottom_die_row[probeT-1].push_back(bottom_die_row[probeT][0]);
                    bottom_die_row[probeT].erase(bottom_die_row[probeT].begin());
                }
                nei.sort_single_row(bottom_die_row, probeT-1);
            }
        }
    }
    for(int i=0; i<bottom_die_row.size(); i++){
        probeB = i;
        probeT = bottom_die_row.size()-1;
        probeL = 0;
        probeR = die_upper_x;
        if(probeB <= probeT){
            L = 0;
            R = bottom_die_row[probeB].size()-1;
            while(probeL < probeR && L <= R){
                if(instances["C"+to_string(bottom_die_row[probeB][L])].instance_pos.x >= probeL){
                    probeL = instances["C"+to_string(bottom_die_row[probeB][L])].instance_pos.x + tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeB][L])].libcell_type].width;
                }
                else{
                    instances["C"+to_string(bottom_die_row[probeB][L])].instance_pos.x = probeL;
                    probeL += tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeB][L])].libcell_type].width;
                }
                L++;
            }
            probeL = die_lower_x;
            L = 0;
            while(probeR > probeL && R >= L){
                if(instances["C"+to_string(bottom_die_row[probeB][R])].instance_pos.x + tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeB][R])].libcell_type].width <= probeR){
                    probeR = instances["C"+to_string(bottom_die_row[probeB][R])].instance_pos.x;
                }
                else{
                    instances["C"+to_string(bottom_die_row[probeB][R])].instance_pos.x = probeR - tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeB][R])].libcell_type].width;
                    probeR -= tech_stack[bottomtech].libcells[instances["C"+to_string(bottom_die_row[probeT][R])].libcell_type].width;
                }
                R--;
            }
            if(probeR < probeL && probeB != probeT){
                for(int j=0; j<=R+1; j++){
                    instances["C"+to_string(bottom_die_row[probeB][0])].instance_pos.y += bottom_row_height;
                    bottom_die_row[probeB+1].push_back(bottom_die_row[probeB][0]);
                    bottom_die_row[probeB].erase(bottom_die_row[probeB].begin());
                }
                nei.sort_single_row(bottom_die_row, probeB+1);
            }
        }
    }
}


