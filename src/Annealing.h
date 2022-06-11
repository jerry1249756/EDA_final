#ifndef Annealing_H
#define Annealing_H

float control_alpha(int,int);
void annealing(neighborhood, 
               unorder_map<string,instance>&, 
               unorder_map<string,net>);


#endif