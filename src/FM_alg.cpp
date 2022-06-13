#include "FM_alg.h"

//I don't know where to define this!!!
long long int TOP_area=0, BOTTOM_area=0;

void initialize_gain(vector<cell_node>& v){
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        (*it).set_gain();
    }
    return;
}

void initialize_area(vector<cell_node>& v){
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        if((*it).part == PART::TOP){
            TOP_area+=(*it).area;
        }else{
            BOTTOM_area+=(*it).area;
        }
    }
    cout << "\ninitialize_area with TOP:" << TOP_area << " , BOTTOM:" << BOTTOM_area << "\n";
    return;
}

bool check_swap_area_constraint(vector<cell_node>& v, cell_node* c){
    if(c->part == PART::TOP){
        int swap_area;
        if(top_die_tech == "TA")
            swap_area = tech_stack[0].libcells[c->libcell_type].get_area();
        else if(top_die_tech == "TB")
            swap_area = tech_stack[1].libcells[c->libcell_type].get_area();
        if(BOTTOM_area + swap_area > die_area/100*bottom_die_max_util){
            //cout << c->node_name << " not valid\n";
            return false;
        }
    }
    else{
        int swap_area;
        if(bottom_die_tech == "TA")
            swap_area = tech_stack[0].libcells[c->libcell_type].get_area();
        else if(bottom_die_tech == "TB")    
            swap_area = tech_stack[1].libcells[c->libcell_type].get_area();
        if(TOP_area + swap_area > die_area/100*top_die_max_util){
            //cout << c->node_name << " not valid\n";
            return false;
        }
    }
    return true;
}

cell_node* find_max_gain_node(vector<cell_node>& v){
    cell_node* temp=NULL;
    int max_gain = INT32_MIN;
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        if((*it).state == LOCK_STATE::UNLOCKED){ 
            if(check_swap_area_constraint(v,&(*it))){
                if((*it).gain>max_gain){
                    temp = &(*it);
                    max_gain = (*it).gain;
                }
            }
        }
    }
    return temp;
}

void swap_and_recalculate(vector<cell_node>& v, cell_node* c){ //TODO
    PART from;
    PART to;
    if (c->part==PART::TOP){
        from = PART::TOP;
        to = PART::BOTTOM;
    }else{
        from = PART::BOTTOM;
        to = PART::TOP;
    }
    //iterate for all nets connected to this cell and update gain
    for(vector<partition_net*>::iterator it=(*c).connected_nets.begin(); it!=(*c).connected_nets.end(); ++it){
        int F_n=0, T_n=0;
        if (from == PART::TOP){
            F_n = (*it)->Dist.T;
            T_n = (*it)->Dist.B;
        }else{
            F_n = (*it)->Dist.B;
            T_n = (*it)->Dist.T;
        }
        if(T_n==0){
            //update for each other unlocked-cell in this net
            for(vector<cell_node*>::iterator it2=(*it)->connected_nodes.begin(); it2!=(*it)->connected_nodes.end(); ++it2){
                if((*it2)->state == LOCK_STATE::UNLOCKED){
                    (*it2)->gain+=1;
                }
            }
        }else if(T_n==1){
            for(vector<cell_node*>::iterator it2=(*it)->connected_nodes.begin(); it2!=(*it)->connected_nodes.end(); ++it2){
                if((*it2)->state == LOCK_STATE::UNLOCKED){
                    if((*it2)->part==to){
                        (*it2)->gain-=1;
                    }
                }
            }
        }
        F_n -= 1;
        T_n += 1;
        if (from == PART::TOP){
            (*it)->Dist.T = F_n;
            (*it)->Dist.B = T_n;
        }else{
            (*it)->Dist.B = F_n;
            (*it)->Dist.T = T_n;
        }
        if(F_n==0){
            for(vector<cell_node*>::iterator it2=(*it)->connected_nodes.begin(); it2!=(*it)->connected_nodes.end(); ++it2){
                if((*it2)->state == LOCK_STATE::UNLOCKED){
                    (*it2)->gain-=1;
                }
            }
        }else if(F_n==1){
            for(vector<cell_node*>::iterator it2=(*it)->connected_nodes.begin(); it2!=(*it)->connected_nodes.end(); ++it2){
                if((*it2)->state == LOCK_STATE::UNLOCKED){
                    if((*it2)->part==from){
                        (*it2)->gain+=1;
                    }
                }
            }
        }
    }
    //swap the node and lock it, update relevant data
    if (c->part==PART::TOP){
        TOP_area -= c->area;
        c->part = PART::BOTTOM;
        c->update_area();
        BOTTOM_area += c->area;
    }
    else{
        BOTTOM_area -= c->area;
        c->part = PART::TOP;
        c->update_area();
        TOP_area += c->area;
    }
    c->state = LOCK_STATE::LOCKED;
    return;
}

int get_cutsize(vector<partition_net*> n){
    int cutsize=0;
    for(vector<partition_net*>::iterator it=n.begin(); it!=n.end(); ++it){
        if((*it)->is_cut()){
            cutsize+=1;
        }
    }
    return cutsize;
}

void print_current_state(vector<cell_node>& v, vector<partition_net*> n){
    vector<cell_node> TOP_part_nodes;
    vector<cell_node> BOTTOM_part_nodes;
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        if((*it).part==PART::TOP) TOP_part_nodes.push_back((*it));
        else BOTTOM_part_nodes.push_back((*it));
    }
    cout << "TOP part nodes: (total area: " << TOP_area << ")\n";
    cout << "BOTTOM part nodes: (total area: " << BOTTOM_area << ")\n";
    cout << TOP_part_nodes.size() << " " << BOTTOM_part_nodes.size();
    cout << " cutsize: " << get_cutsize(n) << "\n\n";
}

void FM_algorithm(vector<cell_node>& v, vector<partition_net*>& n){
    int mincut = INT32_MAX;
    cout << "partitioning...\n";
    initialize_gain(v);
    initialize_area(v);
    print_current_state(v,n);
    cell_node* m = find_max_gain_node(v);
    /*
    if(m!=NULL){
        cout << (*m).node_name;
        swap_and_recalculate(v,m);
    }
    */
    while(m!=NULL){ //there is still valid node to move
        int cutsize = get_cutsize(n);
        if(mincut > cutsize){
            mincut = cutsize;
            //print_current_state(v,n);
            swap_and_recalculate(v,m);
            m = find_max_gain_node(v);
        }
        else break;
    }
    print_current_state(v,n);
    return;
}
