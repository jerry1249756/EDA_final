#ifndef Annealing_H
#define Annealing_H
#include "Neighborhood.h"
#include "hpwl.h"
#include <unordered_map>

float control_alpha(int,int);
void annealing(Neighborhood, 
               unorder_map<string,instance>&, 
               unorder_map<string,net*>);


#endif