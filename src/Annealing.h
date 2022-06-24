#ifndef Annealing_H
#define Annealing_H
#include "Neighborhood.h"
#include "extern.h"
#include "iostream"
#include <unordered_map>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <utility>
using namespace std;

float control_alpha(int,int);
void annealing(Neighborhood, 
               unordered_map<string,instance>&, 
               unordered_map<string,net*>&,
               fstream&);


#endif