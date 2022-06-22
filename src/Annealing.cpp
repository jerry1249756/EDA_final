#include "Annealing.h"

float control_alpha(int later_cost, int ori_cost){
    if(ori_cost - later_cost > 30000){
        return 0.95;
    }
    else
        return 0.8;
    // return 1 + 0.01*(ori_cost - later_cost)/instances.size(); // need to test, linear about later_cost and ori_cost 
    // return 0.85;
}

void annealing(Neighborhood nei, unordered_map<string,instance>& ins, unordered_map<string,net*>& nets, fstream& fout){
    float T = 2000; //need to test
    float frozen_T = 0.5; //need to test
    int innerloop = 200; //need to test
    float alpha; // 0 < alpha < 1
    int num_nei = 3; //number of neighberhood structure
    long long int original_cost = nei.calc_cost(ins, nets);
    while(T > frozen_T){
        for(int i = 0; i < innerloop; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib1(1,num_nei);
            int temp = distrib1(gen);
            uniform_int_distribution<> distrib2(0,toptop2.size()-1);
            int int1 = distrib2(gen);
            int int2 = distrib2(gen);
            uniform_int_distribution<> distrib3(0,toptop2[int1].size()-1);
            int int3 = distrib3(gen);
            uniform_int_distribution<> distrib4(0,toptop2[int2].size()-1);
            int int4 = distrib4(gen);

            uniform_int_distribution<> distrib5(0,botbot2.size()-1);
            int int5 = distrib5(gen);
            int int6 = distrib5(gen);
            uniform_int_distribution<> distrib6(0,botbot2[int5].size()-1);
            int int7 = distrib6(gen);
            uniform_int_distribution<> distrib7(0,botbot2[int6].size()-1);
            int int8 = distrib7(gen);

            int differ;
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
                case 3:
                    differ = nei.two_die_swap_cost(ins, nets, toptop2, botbot2, int1, int3, int5, int7);
                           //+ nei.swap_penalty(toptop2, botbot2, int1, int3, int5, int7);
                    break;
                // case 4:
                //     differ = nei.two_die_swap_cost(ins, nets, botbot2, toptop2, int5, int7, int1, int3);
                //     break;
                // case 5:
                //     differ = nei.z_nei_swap_cost();
                //     break;
                // case 6:
                //     differ = nei.z_nei_move_cost();
                //     break;
            }

            random_device rd_2;
            mt19937 gen2(rd_2());
            uniform_real_distribution<> distrib8(0,1);
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
                    case 3:
                        for(auto& it : instances[('C' + to_string(toptop2[int1][int3]))].connected_nets){
                            cout << it->Net_name << endl;
                            it->dist.first -= 1;
                            it->dist.second += 1;
                            cout << "dist.first: " << it->dist.first << endl;
                            cout << nets[it->Net_name]->dist.first << endl;
                        }
                        for(auto& it : instances[('C' + to_string(botbot2[int5][int7]))].connected_nets){
                            it->dist.first += 1;
                            it->dist.second -= 1;
                            cout << "dist: " << it->dist.first << endl;
                        }
                        nei.update_two_die_swap(toptop2, botbot2, int1, int3, int5, int7);
                        break;
                    // case 4:
                    //     nei.update_swap(botbot2, toptop2, int5, int7, int1, int3);
                    //     break;
                    // case 5:
                    //     nei.z_nei_swap();
                    //     break;
                    // case 6:
                    //     nei.z_nei_move();
                    //     break;
                }  
            }
        }
        long long int later_cost = nei.calc_cost(ins, nets);
        fout << later_cost << endl;
        cout << later_cost << endl;
        alpha = control_alpha(later_cost, original_cost);
        T = alpha * T;
    }
}
