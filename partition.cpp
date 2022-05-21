#include "partition.h"

void cell_node::add_net(net* n){
    this->connected_nets.push_back(n);
}

void cell_node::set_gain(){
    int FS=0;
    int TE=0;
    if(part==PART::TECH_A){//from = A, to = B
        //vector use iterator for performance
        for(vector<net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); it++){
            if((*it)->Dist.A==1){ FS+=1; }   
            else if((*it)->Dist.B==0){ TE+=1; }
        }
    }else if(part==PART::TECH_B){ //from = B, to = A
        for(vector<net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); it++){
            if((*it)->Dist.B==1){ FS+=1;}
            else if((*it)->Dist.A==0){ TE+=1; }
        }
    }else{
        printf("Error in cell_node.set_gain(): cell_node not specified part.\n");
    }
    gain = FS - TE;
    return;
};


void net::add_node(cell_node* c){
    this->connected_nodes.push_back(c);
    if(c->part==PART::TECH_A){
        Dist.A+=1;
    }else if(c->part==PART::TECH_B){
        Dist.B+=1;
    }else{
        printf("Error in net.add_node(): cell_node not specified part.\n");
    }
};

bool net::is_critical(){
    if(Dist.A==(1 || 0)){ return true; }
    else if(Dist.B == (1 || 0)){return true; }
    else return false;
}
