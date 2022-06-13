#include "Kraftwerk2.h"

Kraftwerk2::Kraftwerk2(int n, unordered_map<string, instance>& map, PART part){ //n: num instances
    connectivity_mat = new Matrix(n);
    move_force_mat_x = new Matrix(n);
    move_force_mat_y = new Matrix(n);
    demand = new Vector(n);
    solution_x = new Vector(n);
    solution_y = new Vector(n);
    this->part = part;
    //initial the D and phi array
    int die_width = die_upper_x - die_lower_x;
    if(part == PART::TOP){
        D.reserve(top_repeat_count);
        phi.reserve(top_repeat_count);
    } else{
        D.reserve(bottom_repeat_count);
        phi.reserve(bottom_repeat_count);
    } 
    for(int i = 0; i < D.capacity(); i++){
        vector<float> temp;
        vector<float> temp2;
        temp.reserve(die_width/top_row_height);
        temp2.reserve(die_width/top_row_height);
        for(int j = 0; j < temp.capacity(); j++){
            temp.push_back(0);
            temp2.push_back(0);
        }
        D.push_back(temp);
        phi.push_back(temp2);
    }

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
}

Kraftwerk2::~Kraftwerk2(){ 
    delete connectivity_mat;
    delete move_force_mat_x;
    delete move_force_mat_y;
    delete demand;
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
    cout << "generating connectivity matrix ..\n";
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
    cout << "connectivity mat: \n"; 
    connectivity_mat->print_data();
    cout << " move_force_mat_x: \n"; 
    move_force_mat_x->print_data();
    return;
}

void Kraftwerk2::print_solution(){
    cout << "current solution: \n";
    solution_x -> print_data();
    solution_y -> print_data();
    return;
}

// calculate the C^O matrix, which is the gradient of the specified points, val stored on diagonal entries. 
// input params: phi : 2D float array delta: spacing of the phi array(row height)
// x,y: the point of the instances forming a vector, which will later be stored in solution.

// Note that D(i,j) represents the actual coordinate point at h(1/2+i, 1/2+j)
void Kraftwerk2::calc_gradient(int delta, unordered_map<string, instance> map){ 
    for(int j=0; j<D.size(); j++){
        for(int k=0; k<D[j].size(); k++){
            D[j][k] = 0;
            phi[j][k] = 0;
        }
    }
    cal_D(map, D, part, delta, demand);
    demand->print_data();
    cal_phi(D, phi, delta, 10);
    
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        //(x,y) is in (i,j) , (i+1,j+1)
        int i = static_cast<int>(static_cast<float>(it.second.instance_pos.x)/delta-0.5);
        int j = static_cast<int>(static_cast<float>(it.second.instance_pos.y)/delta-0.5);
        move_force_mat_x->data[n][n] = (phi[i+1][j] + phi[i+1][j+1] - phi[i][j] - phi[i][j+1])/(2*delta);
        move_force_mat_y->data[n][n] = (phi[i][j+1] + phi[i+1][j+1] - phi[i][j] - phi[i+1][j])/(2*delta);
    }
}

void Kraftwerk2::update_pos_diff(){
    Matrix C_x(size), C_y(size), P_x(size), L_x(size), U_x(size), P_y(size), L_y(size), U_y(size);
    Matrix_Addition(*connectivity_mat, *move_force_mat_x, C_x);
    Matrix_Addition(*connectivity_mat, *move_force_mat_y, C_y);
    C_x.PLU_decomposition(L_x, U_x, P_x);
    C_y.PLU_decomposition(L_y, U_y, P_y);
    Vector b_x(size), b_y(size), delta_x(size), delta_y(size);
    Matrix_Vector_Prod(*move_force_mat_x, *demand, b_x);
    Matrix_Vector_Prod(*move_force_mat_y, *demand, b_y);
    solve_linear_system(P_x, L_x, U_x, b_x, delta_x);
    solve_linear_system(P_y, L_y, U_y, b_y, delta_y);
    for(int i=0; i<size; i++){
        solution_x->data[i] -= delta_x.data[i];
        solution_y->data[i] -= delta_y.data[i];
    }
    return;
}

void Kraftwerk2::Kraftwerk2_global_placement(unordered_map<string,instance>& ins){
    int delta =0;
    if(part == PART::TOP){
        delta = top_row_height;
    }else{
        delta = bottom_row_height;
    }
    for(int i=0; i<10; i++){
        calc_gradient(delta, ins);
        update_pos_diff();
    }
}

//init_penalty
