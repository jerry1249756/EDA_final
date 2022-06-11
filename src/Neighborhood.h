#ifndef Neighborhood_H
#define Neighborhood_H

class Neighborhood{
    public:
    Neighborhood(){};
    void swap();
    void reverse();
    void move();
    void inter_swap();
    void z_nei_swap();
    void z_nei_move();
    int swap_cost();
    int reverse_cost();
    int move_cost();
    int inter_swap_cost();
    int z_nei_swap_cost();
    int z_nei_move_cost();
};

#endif