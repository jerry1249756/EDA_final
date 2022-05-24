#include "FM_alg.h"

unsigned int numNodes = 8;
unsigned int numNets = 6;
unsigned int die_area = 900;
unsigned int top_die_max_util = 80, bottom_die_max_util = 90; 

int main(){
    cell_node c1("c1", 70);
    cell_node c2("c2", 160);
    cell_node c3("c3", 160);    
    cell_node c4("c4", 140);    
    cell_node c5("c5", 140);
    cell_node c6("c6", 160);
    cell_node c7("c7", 160);  
    cell_node c8("c8", 70); 
    vector<cell_node> c;
    vector<net> n;
    net net1("n1"), net2("n2"), net3("n3"), net4("n4"), net5("n5"), net6("n6");
    net1.add_node(&c1);
    net1.add_node(&c2);
    net2.add_node(&c2);
    net2.add_node(&c3);
    net2.add_node(&c7);    
    net3.add_node(&c2);
    net3.add_node(&c8);
    net4.add_node(&c3);
    net4.add_node(&c6);
    net4.add_node(&c7);
    net5.add_node(&c4);
    net5.add_node(&c5);
    net5.add_node(&c6);
    net6.add_node(&c4);
    net6.add_node(&c5);
    c.push_back(c1);
    c.push_back(c2);
    c.push_back(c3);
    c.push_back(c4);
    c.push_back(c5);
    c.push_back(c6);
    c.push_back(c7);
    c.push_back(c8);

    FM_algorithm(c, n);
}