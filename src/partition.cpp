#include "partition.h"

cell_node::cell_node(string name, int a){
    node_name = name;
    area = a;
    //avoid using rand()
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0,1);
    part = (distrib(gen))? PART::TECH_B : PART::TECH_A;
    state = LOCK_STATE::UNLOCKED;
    gain = 0;
}

bool cell_node::operator==(const cell_node & c) const{
    return node_name==c.node_name;
}

void cell_node::set_gain(){ //need debug!
    if(part==PART::TECH_A){//from = A, to = B
        //vector use iterator for performance
        for(vector<net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
            if((*it)->Dist.A==1) gain+=1; 
            else if((*it)->Dist.B==0) gain-=1; 
        }
    }else if(part==PART::TECH_B){ //from = B, to = A
        for(vector<net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
            if((*it)->Dist.B==1) gain+=1;
            else if((*it)->Dist.A==0) gain-=1; 
        }
    }
    return;
};

void cell_node::show_data(){
    cout << "node name: " << node_name << "\n";
    cout << "part:      " << (part==PART::TECH_A? "tech_A" : "tech_B") << "\n" ;
    cout << "state:     " << (state==LOCK_STATE::LOCKED? "locked" : "unlocked") << "\n";
    cout << "gain:      " << gain << "\n";
    cout << "area:      " << area << "\n";
    cout << "connected_nets:";
    for(vector<net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
        cout << (*it)->net_name << " ";
    }
    cout << "\n";
    return;
};

net::net(string name){
    net_name = name ;
    Dist.A=0;
    Dist.B=0;
};

void net::add_node(cell_node* c){
    this->connected_nodes.push_back(c);
    c->connected_nets.push_back(this);
    if(c->part==PART::TECH_A) Dist.A+=1;
    else if(c->part==PART::TECH_B) Dist.B+=1;
};

bool net::is_cut(){
    if(Dist.A==0 || Dist.B==0) return false;
    else return true;
};

bool net::is_critical(){
    if(Dist.A==(1 || 0)) return true; 
    else if(Dist.B == (1 || 0))return true; 
    else return false;
};

void net::show_data(){
    cout << "net name:    " << net_name << "\n";
    cout << "Distribution:(" << Dist.A << " " << Dist.B << ")\n";
    cout << "connected_nodes:";
    for(vector<cell_node*>::iterator it=connected_nodes.begin(); it!=connected_nodes.end(); ++it){
        cout << (*it)->node_name << " ";
    }
    cout << "\n";
    return;
};

