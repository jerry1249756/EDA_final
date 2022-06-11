#include "Kraftwerk2_utility.h"

extern int die_lower_x, die_lower_y, die_upper_x, die_upper_y;
extern string top_die_tech, bottom_die_tech;
extern vector<tech> tech_stack;

int row_count = die_upper_x - die_lower_x;
int col_count = die_upper_y - die_lower_y;
int** D = new int*[row_count];
for(int i = 0; i < row_count; i++)
    D[i] = new int[col_count];

void cal_phi(unorder_map<string, instance> ins, PART region){
    //region = 0 represent top, region = 1 represent bottom
    for(int i = 0; i < row_count; i++){
        for(int j = 0; j < col_count; j++){
            D[i][j] = 0;
        }
    }
    for(auto it = ins.begin(); it != ins.end(); ++it){
        if(region == PART::TOP){            
            if(top_die_tech == "TA"){
                if(it->tech == TECH::TECH_A){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[0].libcells[it->libcell_type].width;
                    int height = tech_stack[0].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width)){
                        for(int j = pos_y; j <= (pos_y + height)){
                            D[i][j]++;
                        }
                    }
                }
            }
            else if(top_die_tech == "TB"){
                if(it->tech == TECH::TECH_B){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[1].libcells[it->libcell_type].width;
                    int height = tech_stack[1].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width)){
                        for(int j = pos_y; j <= (pos_y + height)){
                            D[i][j]++;
                        }
                    }
                }
            }
        }
        if(region == PART::BOTTOM){            
            if(bottom_die_tech == "TA"){
                if(it->tech == TECH::TECH_A){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[0].libcells[it->libcell_type].width;
                    int height = tech_stack[0].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width)){
                        for(int j = pos_y; j <= (pos_y + height)){
                            D[i][j]++;
                        }
                    }
                }
            }
            else if(bottom_die_tech == "TB"){
                if(it->tech == TECH::TECH_B){            
                    int pos_x = it->instance_pos.x;
                    int pos_y = it->instance_pos.y;
                    int width = tech_stack[1].libcells[it->libcell_type].width;
                    int height = tech_stack[1].libcells[it->libcell_type].height;
                    for(int i = pos_x; i <= (pos_x + width)){
                        for(int j = pos_y; j <= (pos_y + height)){
                            D[i][j]++;
                        }
                    }
                }
            }
        }
    }

}