#include "Kraftwerk2.h"

Kraftwerk2::Kraftwerk2(int n, int times, unordered_map<string, instance>& map){ //n: num instances
    connectivity_mat = new Matrix(n);
    move_force_mat_x = new Matrix(n);
    move_force_mat_y = new Matrix(n);
    demand_x = new Vector(n);
    demand_y = new Vector(n);
    solution_x = new Vector(n);
    solution_y = new Vector(n);
    //randomly assign position while maintaining the instances inside the die.
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        int acceptable_x_max=0;
        int acceptable_y_max=0;
        if(it.second.tech == TECH::TECH_A){
            acceptable_x_max = die_upper_x - tech_stack[0].libcells[it.second.libcell_type].width;
            acceptable_y_max = die_upper_y - tech_stack[0].libcells[it.second.libcell_type].height;   
        }else if(it.second.tech == TECH::TECH_B){
            acceptable_x_max = die_upper_x - tech_stack[1].libcells[it.second.libcell_type].width;
            acceptable_y_max = die_upper_y - tech_stack[1].libcells[it.second.libcell_type].height;
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib_x(die_lower_x,acceptable_x_max);
        uniform_int_distribution<> distrib_y(die_lower_y,acceptable_y_max);
        solution_x->data[n] = distrib_x(gen);
        solution_y->data[n] = distrib_y(gen);
    }
    size = n;
    iter_param = times;
}

Kraftwerk2::~Kraftwerk2(){ 
    delete connectivity_mat;
    delete move_force_mat_x;
    delete move_force_mat_y;
    delete demand_x;
    delete demand_y;
    delete solution_x;
    delete solution_y;
}

int Kraftwerk2::parse_inst_name(string s){ //input: String("M12"), output: int 11
    return stoi(s.substr(1))-1;
}

void Kraftwerk2::get_solution(unordered_map<string, instance>& map){
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        it.second.instance_pos.x = solution_x->data[n] ;
        it.second.instance_pos.y = solution_y->data[n] ;
    }
}

void Kraftwerk2::gen_connectivity_matrix(unordered_map<string, net*> map){
    for(auto& it : map){
        int n =it.second->net_pin.size();
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                int k = parse_inst_name(it.second->net_pin[i].INSTANCE);
                int l = parse_inst_name(it.second->net_pin[j].INSTANCE);
                connectivity_mat->data[k][l] +=1 ;
                connectivity_mat->data[l][k] +=1 ;
            }
        }
    }
    return;
}

void Kraftwerk2::print_mat(){
    connectivity_mat->print_data();
    return;
}

// calculate the C^O matrix, which is the gradient of the specified points, val stored on diagonal entries. 
// input params: phi : 2D float array delta: spacing of the phi array(row height)
// x,y: the point of the instances forming a vector, which will later be stored in solution.

// Note that D(i,j) represents the actual coordinate point at h(1/2+i, 1/2+j)
void Kraftwerk2::calc_gradient(vector<vector<float>> phi, int delta, unordered_map<string, instance> map){ 
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        //(x,y) is in (i,j) , (i+1,j+1)
        int i = static_cast<int>(static_cast<float>(it.second.instance_pos.x)/delta-0.5);
        int j = static_cast<int>(static_cast<float>(it.second.instance_pos.y)/delta-0.5);
        move_force_mat_x->data[n][n] = (phi[i+1][j] + phi[i+1][j+1] - phi[i][j] - phi[i][j+1])/(2*delta);
        move_force_mat_y->data[n][n] = (phi[i][j+1] + phi[i+1][j+1] - phi[i][j] - phi[i+1][j])/(2*delta);
    }
}

/*
void Kraftwerk2::update_pos_diff(Vector demand){
    Matrix C_x(size), C_y(size), P_x(size), L_x(size), U_x(size), P_y(size), L_y(size), U_y(size);

    C_x = Matrix_Addition(*connectivity_mat, *move_force_mat_x);
    C_y = Matrix_Addition(*connectivity_mat, *move_force_mat_y);
    C_x.PLU_decomposition(L_x, U_x, P_x);
    C_y.PLU_decomposition(L_y, U_y, P_y);


}

void Kraftwerk2::Kraftwerk2_global_placement(){

}
*/