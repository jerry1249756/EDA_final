#include "Annealing.h"

float control_alpha(int later_cost, int ori_cost){
    if(ori_cost - later_cost > 50000){
        return 0.95;
    }
    else
        return 0.85;
    // return 1 + 0.01*(ori_cost - later_cost)/instances.size(); // need to test, linear about later_cost and ori_cost 
    // return 0.85;
}

void annealing(Neighborhood nei, unordered_map<string,instance>& ins, unordered_map<string,net*>& nets, fstream& fout){
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
    float T = 3000; //need to test
    float frozen_T = 0.25; //need to test
    int innerloop = 200; //need to test
    float alpha; // 0 < alpha < 1
    int num_nei = 3; //number of neighberhood structure
    double to_x;
    long long int original_cost = nei.calc_cost(ins, nets);
    for(auto& it : nets){
        it.second->update_bound(instances, tech_stack);
    }
    while(T > frozen_T){
        for(int i = 0; i < innerloop; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> distrib1(1,2);
            int temp = distrib1(gen);
            uniform_int_distribution<int> distrib2(0,toptop2.size()-1);
            int int1 = distrib2(gen);
            int int2 = distrib2(gen);
            uniform_int_distribution<int> distrib3(0,toptop2[int1].size()-1);
            int int3 = distrib3(gen);
            uniform_int_distribution<int> distrib4(0,toptop2[int2].size()-1);
            int int4 = distrib4(gen);

            uniform_int_distribution<int> distrib5(0,botbot2.size()-1);
            int int5 = distrib5(gen);
            int int6 = distrib5(gen);
            uniform_int_distribution<int> distrib6(0,botbot2[int5].size()-1);
            int int7 = distrib6(gen);
            uniform_int_distribution<int> distrib7(0,botbot2[int6].size()-1);
            int int8 = distrib7(gen);

            int differ;
            pair<int,double> p1;
            switch(temp){
                default:
                    break;
                case 1:
                    differ = nei.swap_cost(ins, nets, toptop2, int1, int3, int2, int4);
                           //+ nei.swap_penalty(toptop2, toptop2, int1, int3, int2, int4);
                    break;
                case 2:
                    differ = nei.swap_cost(ins, nets, botbot2, int5, int7, int6, int8);
                           //+ nei.swap_penalty(botbot2, botbot2, int5, int7, int6, int8);
                    break;
                // case 3:
                //     differ = nei.two_die_swap_cost(ins, nets, toptop2, botbot2, int1, int3, int5, int7);
                //            //+ nei.swap_penalty(toptop2, botbot2, int1, int3, int5, int7);
                //     break;
                case 3:
                    p1 = nei.move_in_row_cost(instances,nets,toptop2,int1,int3);
                    differ = p1.first;
                    to_x = p1.second;
                    break;
                case 4:
                    p1 = nei.move_in_row_cost(instances,nets,botbot2,int5,int7);
                    differ = p1.first;
                    to_x = p1.second;
                    break;
                // case 5:
                //     differ = nei.left_move_in_row_cost(instances,nets,botbot2,int5,int7);
                //     break;
                // case 6:
                //     differ = nei.right_move_in_row_cost(instances,nets,botbot2,int5,int7);
                //     break;
            }

            random_device rd_2;
            mt19937 gen2(rd_2());
            uniform_real_distribution<double> distrib8(0,1);
            double temp_rand = distrib8(gen2);
            if(differ < 0 || temp_rand < exp(-double(differ)/double(T))){
                switch(temp){
                    default:
                        break;
                    case 1:
                        nei.update_swap(toptop2, int1, int3, int2, int4);
                        break;
                    case 2:
                        nei.update_swap(botbot2, int5, int7, int6, int8);
                        break;
                    // case 3:
                    //     if(TOP_area-tech_stack[toptech].libcells[instances["C"+to_string(toptop2[int1][int3])].libcell_type].width*top_row_height+tech_stack[toptech].libcells[instances["C"+to_string(botbot2[int5][int7])].libcell_type].width*top_row_height <= die_area/100*top_die_max_util && BOTTOM_area+tech_stack[bottomtech].libcells[instances["C"+to_string(toptop2[int1][int3])].libcell_type].width*bottom_row_height-tech_stack[bottomtech].libcells[instances["C"+to_string(botbot2[int5][int7])].libcell_type].width*bottom_row_height <= die_area/100*bottom_die_max_util){
                    //         for(auto& it : instances[('C' + to_string(toptop2[int1][int3]))].connected_nets){
                    //             it->dist.first -= 1;
                    //             it->dist.second += 1;
                    //         }
                    //         for(auto& it : instances[('C' + to_string(botbot2[int5][int7]))].connected_nets){
                    //             it->dist.first += 1;
                    //             it->dist.second -= 1;
                    //         }
                    //         TOP_area = TOP_area-tech_stack[toptech].libcells[instances["C"+to_string(toptop2[int1][int3])].libcell_type].width*top_row_height+tech_stack[toptech].libcells[instances["C"+to_string(botbot2[int5][int7])].libcell_type].width*top_row_height;
                    //         BOTTOM_area = BOTTOM_area+tech_stack[bottomtech].libcells[instances["C"+to_string(toptop2[int1][int3])].libcell_type].width*bottom_row_height-tech_stack[bottomtech].libcells[instances["C"+to_string(botbot2[int5][int7])].libcell_type].width*bottom_row_height;
                    //         nei.update_two_die_swap(toptop2, botbot2, int1, int3, int5, int7);
                    //     }  
                    //     break;
                    case 3:
                        nei.update_move_in_row(toptop2,int1,int3,to_x);
                        break;
                    case 4:
                        nei.update_move_in_row(botbot2,int5,int7,to_x);
                        break;
                    // case 5:
                    //     nei.update_left_move_in_row(botbot2,int5,int7);
                    //     break;
                    // case 6:
                    //     nei.update_right_move_in_row(botbot2,int5,int7);
                    //     break;
                }  
            }
        }
        long long int later_cost = nei.calc_cost(ins, nets);
        // fout << later_cost << endl;
        cout << later_cost << endl;
        alpha = control_alpha(later_cost, original_cost);
        T = alpha * T;
    }
}
