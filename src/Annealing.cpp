#include "Annealing.h"

void control_alpha(int later_cost, int ori_cost){
    return 0.85; // need to test, linear about later_cost and ori_cost 
}

void annealing(neighborhood nei, unorder_map<string,instance>& ins, unorder_map<string,net*> nets){
    float T; //need to test
    float frozen_T; //need to test
    int innerloop = 0; //need to test
    float alpha; // 0 < alpha < 1
    int num_nei; //number of neighberhood structure
    int original_cost = cal_cost(ins);
    while(T > frozen_T){
        for(int i = 0; i < innerloop; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(1,num_nei);
            int temp = distrib(gen);
            int differ;
            switch(temp){
                default:
                    break;
                case 1:
                    differ = nei.swap_cost();
                    break;
                case 2:
                    differ = nei.reverse_cost();
                    break;
                case 3:
                    differ = nei.move_cost();
                    break;
                case 4:
                    differ = nei.inter_swap_cost();
                    break;
                case 5:
                    differ = nei.z_nei_swap_cost();
                    break;
                case 6:
                    differ = nei.z_nei_move_cost();
                    break;
            }

            random_device rd_2;
            mt19937 gen(rd_2());
            uniform_real_distribution<> distrib(0,1);
            
            if(differ < 0 || distrib(gen) < exp(-double(differ)/double(T))){
                switch(temp){
                    default:
                        break;
                    case 1:
                        nei.swap();
                        break;
                    case 2:
                        nei.reverse();
                        break;
                    case 3:
                        nei.move();
                        break;
                    case 4:
                        nei.inter_swap();
                        break;
                    case 5:
                        nei.z_nei_swap();
                        break;
                    case 6:
                        nei.z_nei_move();
                        break;
                }  
            }
        }
        later_cost = cal_cost(ins);
        alpha = control_alpha(later_cost, original_cost);
        T = alpha * T;
    }
}
