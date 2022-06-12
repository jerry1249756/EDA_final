#include "Kraftwerk2_utility.h"

extern unsigned long long int die_area;
extern int die_lower_x, die_lower_y, die_upper_x, die_upper_y;
extern string top_die_tech, bottom_die_tech;
extern vector<tech> tech_stack;

int row_count = die_upper_x - die_lower_x;
int col_count = die_upper_y - die_lower_y;

void cal_D(unorder_map<string, instance> ins, vector<vector<float>>& D, PART region){
    //region = 0 represent top, region = 1 represent bottom
    for(auto it = ins.begin(); it != ins.end(); ++it){
        if(region == PART::TOP){            
            if(it->part == PART::TOP){
                if(it->tech == TECH::TECH_A){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[0].libcells[it->libcell_type].width;
                    int height = tech_stack[0].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width); i++){
                        for(int j = pos_y; j <= (pos_y + height); j++){
                            D[i][j] = D[i][j] + 1 - width*height/die_area;
                        }
                    }
                }
                if(it->tech == TECH::TECH_B){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[1].libcells[it->libcell_type].width;
                    int height = tech_stack[1].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width); i++){
                        for(int j = pos_y; j <= (pos_y + height); j++){
                            D[i][j] = D[i][j] + 1 - width*height/die_area;
                        }
                    }
                }
            }
        }
        if(region == PART::BOTTOM){            
            if(it->part == PART::BOTTOM){
                if(it->tech == TECH::TECH_A){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[0].libcells[it->libcell_type].width;
                    int height = tech_stack[0].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width); i++){
                        for(int j = pos_y; j <= (pos_y + height); j++){
                            D[i][j] = D[i][j] + 1 - width*height/die_area;
                        }
                    }
                }
            }
            else if(it->part == PART::BOTTOM){
                if(it->tech == TECH::TECH_B){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[1].libcells[it->libcell_type].width;
                    int height = tech_stack[1].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width); i++){
                        for(int j = pos_y; j <= (pos_y + height); j++){
                            D[i][j] = D[i][j] + 1 - width*height/die_area;
                        }
                    }
                }
            }
        }
    }
}