#ifndef FM_ALG_H
#define FM_ALG_H

#include "partition.h"

void initialize_gain(vector<cell_node>&);
void initialize_area(vector<cell_node>&);
bool check_swap_area_constraint(vector<cell_node>&, cell_node*);
cell_node* find_max_gain_node(vector<cell_node>&);
void swap_and_recalculate(vector<cell_node>&, cell_node*);
void print_current_state(vector<cell_node>&, vector<partition_net>&);
void FM_algorithm(vector<cell_node>&, vector<partition_net>&);

#endif