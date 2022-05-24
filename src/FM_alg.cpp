#include "partition.h"
//extern variables
extern unsigned int numNodes;
extern unsigned int numNets;
extern unsigned int die_area;
extern unsigned int top_die_max_util, bottom_die_max_util; //top: tech A, bottom: tech B

//I don't know where to define this!!!
int A_area=0, B_area=0;

void initialize_gain(vector<cell_node>& v){
    cout << "initialize gain:\n";
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        (*it).set_gain();
    }
    return;
}

void initialize_area(vector<cell_node> v){
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        if((*it).part == PART::TECH_A){
            A_area+=(*it).area;
        }else{
            B_area+=(*it).area;
        }
    }
    cout << "\ninitialize_area with A:" << A_area << " , B:" << B_area << "\n";
    return;
}

bool check_swap_area_constraint(vector<cell_node> v, cell_node* c){
    if((*c).part == PART::TECH_A){
        if(B_area + (*c).area > die_area*bottom_die_max_util/100){
            cout << (*c).node_name << " not valid\n";
            return false;
        }
    }else{
        if(A_area + (*c).area >die_area*top_die_max_util/100){
            cout << (*c).node_name << " not valid\n";
            return false;
        }
    }
    return true;
}

cell_node* find_max_gain_node(vector<cell_node> v){
    cell_node* temp=NULL;
    int max_gain = -1000000;
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
    if ((*c).part==PART::TECH_A){
        from = PART::TECH_A;
        to = PART::TECH_B;
    }else{
        from = PART::TECH_B;
        to = PART::TECH_A;
    }
    //iterate for all nets connected to this cell and update gain
    for(vector<net*>::iterator it=(*c).connected_nets.begin(); it!=(*c).connected_nets.end(); ++it){
        int F_n=0, T_n=0;
        if (from == PART::TECH_A){
            F_n = (*it)->Dist.A;
            T_n = (*it)->Dist.B;
        }else{
            F_n = (*it)->Dist.B;
            T_n = (*it)->Dist.A;
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
        if (from == PART::TECH_A){
            (*it)->Dist.A = F_n;
            (*it)->Dist.B = T_n;
        }else{
            (*it)->Dist.B = F_n;
            (*it)->Dist.A = T_n;
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
    if ((*c).part==PART::TECH_A){
        (*c).part = PART::TECH_B;
        B_area += (*c).area;
        A_area -= (*c).area;
    }
    else{
        (*c).part = PART::TECH_A;
        A_area += (*c).area;
        B_area -= (*c).area;

    }
    (*c).state = LOCK_STATE::LOCKED;
    return;
}

void print_current_state(vector<cell_node> v, vector<net> n){
    vector<cell_node> A_part_nodes;
    vector<cell_node> B_part_nodes;
    vector<cell_node> locked_nodes;
    for(vector<cell_node>::iterator it=v.begin(); it!=v.end(); ++it){
        if((*it).part==PART::TECH_A) A_part_nodes.push_back((*it));
        else B_part_nodes.push_back((*it));
        if((*it).state==LOCK_STATE::LOCKED){
            locked_nodes.push_back((*it));
        }
    }
    cout << "A part nodes: (total area: " << A_area << ")\n";
    for(vector<cell_node>::iterator it=A_part_nodes.begin(); it!=A_part_nodes.end(); ++it){
        cout << (*it).node_name << "(" << (*it).gain << ") ";
    }
    cout << "\nB part nodes: (total area: " << B_area << ")\n";
    for(vector<cell_node>::iterator it=B_part_nodes.begin(); it!=B_part_nodes.end(); ++it){
        cout << (*it).node_name << "(" << (*it).gain << ") ";
    }
    cout << "\nlocked nodes:";
    for(vector<cell_node>::iterator it=locked_nodes.begin(); it!=locked_nodes.end(); ++it){
        cout << (*it).node_name << " ";
    }
    cout << "\ncurrent net structure:\n";
    for(vector<net>::iterator it=n.begin(); it!=n.end(); ++it){
        cout << (*it).net_name << " (" << (*it).Dist.A << "," << (*it).Dist.B << ")\n";
    }
}

void FM_algorithm(vector<cell_node> v, vector<net> n){
    initialize_gain(v);
    initialize_area(v);
    print_current_state(v,n);
    cell_node* m = find_max_gain_node(v);
    if(m!=NULL){
        cout << (*m).node_name;
        //swap_and_recalculate(v,m);
    }
    /*while(m!=NULL){ //there is still valid node to move
        print_current_state(v,n);
        swap_and_recalculate(v,m);
        m = find_max_gain_node(v);
    }*/
    //print_current_state(v,n);
    return;
}
