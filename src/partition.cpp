#include "partition.h"

extern vector<tech> tech_stack;
extern unsigned int top_repeat_count, bottom_repeat_count;

cell_node::cell_node(string name, string type){
    node_name = name;
    libcell_type = type;
    //avoid using rand()
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0,100);
    int temp = distrib(gen);
    part = (temp < 100*top_repeat_count*top_repeat_count/(top_repeat_count*top_repeat_count+bottom_repeat_count*bottom_repeat_count))? PART::TOP : PART::BOTTOM; 
    
    if(part == PART::TOP){
        area = tech_stack[0].libcells[libcell_type].get_area();
    }
    else{
        if(tech_stack.size() !=1) area = tech_stack[1].libcells[libcell_type].get_area();
        else area = tech_stack[0].libcells[libcell_type].get_area();
    }
        
    state = LOCK_STATE::UNLOCKED;
    gain = 0;
}

bool cell_node::operator==(const cell_node & c) const{
    return node_name==c.node_name;
}

void cell_node::set_gain(){ //need debug!
    if(part==PART::TOP){//from = A, to = B
        //vector use iterator for performance
        for(vector<partition_net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
            if((*it)->Dist.T==1) gain+=1; 
            else if((*it)->Dist.B==0) gain-=1; 
        }
    }else if(part==PART::BOTTOM){ //from = B, to = A
        for(vector<partition_net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
            if((*it)->Dist.B==1) gain+=1;
            else if((*it)->Dist.T==0) gain-=1; 
        }
    }
    return;
};

void cell_node::show_data(){
    cout << "node name: " << node_name << "\n";
    cout << "part:      " << (part==PART::TOP? "top" : "bottom") << "\n" ;
    cout << "state:     " << (state==LOCK_STATE::LOCKED? "locked" : "unlocked") << "\n";
    cout << "gain:      " << gain << "\n";
    cout << "area:      " << area << "\n";
    cout << "connected_nets:";
    for(vector<partition_net*>::iterator it=connected_nets.begin(); it!=connected_nets.end(); ++it){
        cout << (*it)->net_name << " ";
    }
    cout << "\n";
    return;
};

void cell_node::update_area(){
    if(part == PART::TOP){
        area = tech_stack[0].libcells[libcell_type].get_area();
    }
    else{
        if(tech_stack.size() !=1) area = tech_stack[1].libcells[libcell_type].get_area();
        else area = tech_stack[0].libcells[libcell_type].get_area();
    }
    return;
}

partition_net::partition_net(){
    Dist.T=0;
    Dist.B=0;
};

partition_net::partition_net(string name){
    net_name = name ;
    Dist.T=0;
    Dist.B=0;
};

void partition_net::add_node(cell_node* c){
    connected_nodes.push_back(c);
    c->connected_nets.push_back(this);
    if(c->part==PART::TOP) Dist.T+=1;
    else if(c->part==PART::BOTTOM) Dist.B+=1;
};

bool partition_net::is_cut(){
    if(Dist.T==0 || Dist.B==0) return false;
    else return true;
};

bool partition_net::is_critical(){
    if(Dist.T==(1 || 0)) return true; 
    else if(Dist.B == (1 || 0))return true; 
    else return false;
};

void partition_net::show_data(){
    cout << "net name:    " << net_name << "\n";
    cout << "Distribution:(" << Dist.T << " " << Dist.B << ")\n";
    cout << "connected_nodes:";
    for(vector<cell_node*>::iterator it=connected_nodes.begin(); it!=connected_nodes.end(); ++it){
        cout << (*it)->node_name << " ";
    }
    cout << "\n";
    return;
};

