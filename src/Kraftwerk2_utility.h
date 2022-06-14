#ifndef KRAFTWERK2_UTILITY_H
#define KRAFTWERK2_UTILITY_H
#include <unordered_map>
#include "module.h"
#include "enum.h"
#include "extern.h"
#include "LA.h"
#include <iostream>

using namespace std;

void cal_D(unordered_map<string,instance>,vector<vector<float>>&,Vector*);//int 0 represent top, int 1 represent bottom
void cal_phi(vector<vector<float>>, vector<vector<float>>&,int);


#endif