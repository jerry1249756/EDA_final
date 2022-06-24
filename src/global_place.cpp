#include "global_place.h"

void update_connectivity(instance ins){
    for(int i=0; i<ins.connected_nets.size(); i++){
        for(int j=0; j<ins.connected_nets[i]->net_pin.size(); j++){
            instances[ins.connected_nets[i]->net_pin[j].first].connectivity += 1;
        }
    }
}

bool con_compare(ip i, ip j){
    return (instances[i.first].connected_nets.size() < instances[j.first].connected_nets.size());
}

void placement(){
    int temp = 0;
    unordered_set<string> S;
    string max_net_name;
    for(auto& it : nets){
        if(it.second->net_pin.size() > temp){
            temp = it.second->net_pin.size();
            max_net_name = it.first;
        }
    }
    sort(nets[max_net_name]->net_pin.begin(),nets[max_net_name]->net_pin.end(),con_compare);
    if(nets[max_net_name]->net_pin.size() < 4){
        for(int i = 0; i < nets[max_net_name]->net_pin.size(); i++){        
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> distrib1(die_lower_x,die_upper_x);
            int tempx = distrib1(gen);
            uniform_int_distribution<int> distrib2(die_lower_y,die_upper_y);
            int tempy = distrib2(gen);
            instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = tempx;
            instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = tempy;
        }
    }
    else{
        for(int i = 0; i < nets[max_net_name]->net_pin.size(); i++){
            switch (i){
            case 0:
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = 0;
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = 0;
                break;
            case 1:
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = 0;
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = die_upper_y;
                break;
            case 2:
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = die_upper_x;
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = 0;
                break;
            case 3:
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = die_upper_x;
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = die_upper_y;
                break;
            default:           
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<int> distrib1(die_lower_x,die_upper_x);
                int tempx = distrib1(gen);
                uniform_int_distribution<int> distrib2(die_lower_y,die_upper_y);
                int tempy = distrib2(gen);
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.x = tempx;
                instances[nets[max_net_name]->net_pin[i].first].instance_pos.y = tempy;
                break;
            }
            update_connectivity(instances[nets[max_net_name]->net_pin[i].first]);
            S.insert(nets[max_net_name]->net_pin[i].first);
        }
    }

    while(S.size() != Num_instance){
        int num = 0;
        int temp2 = 0;
        string temp3;
        for(auto& it : instances){
            if(!S.count(it.first)){
                if(it.second.connectivity > temp2){
                    temp2 = it.second.connectivity;
                    temp3 = it.first;
                }
            }
        }

        for(auto& it : instances[temp3].connected_nets){
            for(auto& it2 : it->net_pin){
                if(S.count(it2.first)){
                    instances[temp3].instance_pos.x += instances[it2.first].instance_pos.x;
                    instances[temp3].instance_pos.y += instances[it2.first].instance_pos.y;
                    num++;
                }
            }
        }
        instances[temp3].instance_pos.x /= num;
        instances[temp3].instance_pos.y /= num;   
        S.insert(temp3);
        update_connectivity(instances[temp3]); 
    }
}