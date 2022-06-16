#include "Kraftwerk2.h"

Kraftwerk2::Kraftwerk2(int n, unordered_map<string, instance>& map){ //n: num instances
    connectivity_mat = new Matrix(n);
    move_force_mat_x = new Matrix(n);
    move_force_mat_y = new Matrix(n);
    demand_x = new Vector(n);
    demand_y = new Vector(n);
    solution_x = new Vector(n);
    solution_y = new Vector(n);
    //initial the D and phi array
    int die_width = die_upper_x - die_lower_x;
        D.resize(die_width/min(top_row_height,bottom_row_height));
        phi.resize(die_width/min(top_row_height,bottom_row_height));
    for(int i = 0; i < D.capacity(); i++){
        D[i].resize(max(top_repeat_count, bottom_repeat_count));
        phi[i].resize(max(top_repeat_count, bottom_repeat_count));
    }
    //randomly assign position while maintaining the instances inside the die.
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        int acceptable_x_max=0;
        int acceptable_y_max=0;
        if(it.second.tech == TECH::TECH_A){
            acceptable_x_max = die_upper_x - tech_stack[0].libcells[it.second.libcell_type].width;
            if(it.second.part == PART::TOP){
                acceptable_y_max = top_die_upper_y - tech_stack[0].libcells[it.second.libcell_type].height;   
            }
            else{
                acceptable_y_max = bottom_die_upper_y - tech_stack[0].libcells[it.second.libcell_type].height;   
            }
            
        }else if(it.second.tech == TECH::TECH_B){
            acceptable_x_max = die_upper_x - tech_stack[1].libcells[it.second.libcell_type].width;
            if(it.second.part == PART::TOP){
                acceptable_y_max = top_die_upper_y - tech_stack[1].libcells[it.second.libcell_type].height;   
            }
            else{
                acceptable_y_max = bottom_die_upper_y - tech_stack[1].libcells[it.second.libcell_type].height;   
            }
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
    delete demand_x;
    delete demand_y;
    delete solution_x;
    delete solution_y;
}

void Kraftwerk2::input_solution(unordered_map<string, instance>& map){
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        solution_x->data[n] = it.second.instance_pos.x ;
        solution_y->data[n] = it.second.instance_pos.y ;
    }
}

//D should be a m*n zero matrix
void Kraftwerk2::cal_D(unordered_map<string,instance> ins, vector<vector<float>>& D){
    //region = 0 represent top, region = 1 represent bottom
    int delta = min(top_row_height,bottom_row_height);
    float d_mod = 1;
    for(auto it = ins.begin(); it != ins.end(); ++it){
        int n = stoi(it->first.substr(1))-1;    
        //cout << it->first << endl;
        if(it->second.tech == TECH::TECH_A){         
            float pos_x = solution_x->data[n];
            float pos_y = solution_y->data[n];
            int width = tech_stack[0].libcells[it->second.libcell_type].width;
            int height = tech_stack[0].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(it->second.part == PART::TOP){    
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (top_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D[i][j] += 1*d_mod;
                    }
                    else{
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (bottom_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D[i][j] += 1*d_mod;
                    }
                }
            }
        }   
        if(it->second.tech == TECH::TECH_B){            
            float pos_x = solution_x->data[n];
            float pos_y = solution_y->data[n];
            int width = tech_stack[1].libcells[it->second.libcell_type].width;
            int height = tech_stack[1].libcells[it->second.libcell_type].height;
            int initial_x;
            int initial_y;
            if(((pos_x - float(delta/2)) / delta) - int((pos_x - float(delta)/2) / delta) == 0)
                initial_x = (pos_x - float(delta/2))/delta;
            else
                initial_x = (pos_x - float(delta/2))/delta + 1;
            if(((pos_y - float(delta/2)) / delta) - int((pos_y - float(delta)/2) / delta) == 0)
                initial_y = (pos_y - float(delta/2))/delta;
            else
                initial_y = (pos_y - float(delta/2))/delta + 1;
            for(int i = initial_x; i <= int((pos_x + width)-float(delta)/2)/delta; i++){
                for(int j = initial_y; j <= int((pos_y + height -float(delta)/2)/delta); j++){
                    if(it->second.part == PART::TOP){    
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (top_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D[i][j] += 1*d_mod;
                    }
                    else{
                        if(i >= (die_upper_x-die_lower_x) / delta || j >= (bottom_die_upper_y) / delta || i < 0 || j < 0){
                            continue;
                        }
                        D[i][j] += 1*d_mod;
                    }
                }
            }
        }
    }
    for(int i = 0; i < D.size(); i++){
        for(int j = 0; j < D[i].size(); j++){
             D[i][j] -= d_mod * static_cast<float>(TOP_area+BOTTOM_area)/(2*die_area);
        }
    }
}

//phi should be m*n zero matrix
void Kraftwerk2::cal_phi(vector<vector<float>> D, vector<vector<float>>& phi, int times){
    int delta = min(top_row_height,bottom_row_height);
    vector<vector<float>> temp;
    vector<vector<float>> temp3;
    temp.reserve(phi.size());
    temp3.reserve(phi.size());
    for(int i = 0; i < phi.size(); i++){
        vector<float> temp2;
        vector<float> temp4;
        temp2.reserve(phi[i].size());
        temp4.reserve(phi[i].size());
        for(int j = 0; j < phi[i].size(); j++){
            temp2.push_back(0);
            temp4.push_back(0);
        }
        temp.push_back(temp2);
        temp3.push_back(temp4);
    }
    for(int k = 0; k < times; k++){
        if(k%2 == 0){
            for(int i = 0; i < phi.size(); i++){
                for(int j = 0; j < phi[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i+1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi[i].size()-1){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i+1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i+1][j]*2) / 4;
                        }
                    }
                    else if(i == phi.size()-1){
                        if(j == 0){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j+1]) / 2; 
                        }
                        else if(j == phi[i].size()-1){
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j-1]) / 2; 
                        }
                        else{
                            temp3[i][j] = (delta*delta*D[i][j] + temp[i][j-1] + temp[i][j+1] + temp[i-1][j]*2) / 4;
                        }
                    }
                    else if(j == 0){
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i][j+1]*2 + temp[i+1][j]) / 4;
                    }
                    else if(j == phi[i].size()-1){
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1]*2) / 4; 
                    }
                    else{
                        temp3[i][j] = (delta*delta*D[i][j] + temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) / 4; 
                    }
                }
            }
        }
        else if(k%2 == 1){
            for(int i = 0; i < phi.size(); i++){
                for(int j = 0; j < phi[i].size(); j++){
                    if(i == 0){
                        if(j == 0){
                            temp[i][j] = (delta*D[i][j] + temp3[i+1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi[i].size()-1){
                            temp[i][j] = (delta*D[i][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*D[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                        }
                    }
                    else if(i == phi.size()-1){
                        if(j == 0){
                            temp[i][j] = (delta*D[i][j] + temp3[i-1][j] + temp3[i][j+1]) / 4; 
                        }
                        else if(j == phi[i].size()-1){
                            temp[i][j] = (delta*D[i][j] + temp3[i-1][j] + temp3[i][j-1]) / 4; 
                        }
                        else{
                            temp[i][j] = (delta*D[i][j] + temp3[i][j-1] + temp3[i][j+1] + temp3[i-1][j]) / 4;
                        }
                    }
                    else if(j == 0){
                        temp[i][j] = (delta*D[i][j] + temp3[i-1][j] + temp3[i][j+1] + temp3[i+1][j]) / 4;
                    }
                    else if(j == phi[i].size()-1){
                        temp[i][j] = (delta*D[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1]) / 4; 
                    }
                    else{
                        temp[i][j] = (delta*D[i][j] + temp3[i-1][j] + temp3[i+1][j] + temp3[i][j-1] + temp3[i][j+1]) / 4; 
                    }
                }
            }
        }
    }
    if(times%2 == 0){
        for(int i = 0; i < phi.size(); i++){
            for(int j = 0; j < phi[i].size(); j++){
                phi[i][j] = temp[i][j];
            }
        }
    }
    else if(times%2 == 1){
        for(int i = 0; i < phi.size(); i++){
            for(int j = 0; j < phi[i].size(); j++){
                phi[i][j] = temp3[i][j];
            }
        }
    }
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
    int temp = 1;
    cout << "generating connectivity matrix ..\n";
    for(auto& it : map){
        int n =it.second->net_pin.size();
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                int k = parse_inst_name(it.second->net_pin[i].INSTANCE);
                int l = parse_inst_name(it.second->net_pin[j].INSTANCE);
                connectivity_mat->data[k][l] += temp;
                connectivity_mat->data[l][k] += temp;
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

void Kraftwerk2::print_solution(fstream& fout){
    //cout << "current solution: \n";
    solution_x -> print_data(fout);
    solution_y -> print_data(fout);
    return;
}

pair<float, float> Kraftwerk2::single_point_gradient(vector<vector<float>> phi, float x, float y, PART part){
    int delta = min(top_row_height,bottom_row_height);
    //(x,y) is in (i,j) , (i+1,j+1)
    int i = static_cast<int>(x/delta-0.5); //bounding box
    int j = static_cast<int>(y/delta-0.5);
    float slope = 1;
    //cout << i << " " << j << endl;
    float val, grad_x, grad_y;
    int temp_upper_y = 0;
    if(part == PART::TOP){
        temp_upper_y = top_die_upper_y + die_lower_y;
    }
    else{
        temp_upper_y = bottom_die_upper_y + die_lower_y;
    }
    if(i >= (die_upper_x-die_lower_x)/delta-1 || i<=0 || j >= (temp_upper_y-die_lower_y)/delta-1 || j<=0){
        if(i >= (die_upper_x-die_lower_x)/delta-1){ //right border
            if(j >= (temp_upper_y-die_lower_y)/delta-1){ // top border
                grad_x = slope;
                grad_y = slope;
            }else if(j<=0){
                grad_x = slope;
                grad_y = -1*slope;
            }else{
                grad_x = slope;
                float w1 = y - delta*(j+0.5);
                float w2 = delta*(j+1.5) - y;
                i = (die_upper_x-die_lower_x)/delta-1;
                val = (w1*phi[i][j+1] + w2*phi[i][j])/delta;
                int l = (y/delta-j >= 0.5) ? j+1 : j;
                if(l==j){
                    grad_y = (val - phi[i][l])/(w1*delta);
                }else{
                    grad_y = (phi[i][l] - val)/(w2*delta);
                }
            }
        }else if(i<=0){ //left border
            if(j >= (temp_upper_y-die_lower_y)/delta-1){ // top border
                grad_x = -1*slope;
                grad_y = slope;
            }else if(j<=0){
                grad_x = -1*slope;
                grad_y = -1*slope;
            }else{
                grad_x = -1*slope;
                float w1 = y - delta*(j+0.5);
                float w2 = delta*(j+1.5) - y;
                i = 0;
                val = (w1*phi[i][j+1] + w2*phi[i][j])/delta;
                int l = (y/delta-j >= 0.5) ? j+1 : j;
                if(l==j){
                    grad_y = (val - phi[i][l])/(w1*delta);
                }else{
                    grad_y = (phi[i][l] - val)/(w2*delta);
                }
            }
        }else if(j >= (temp_upper_y-die_lower_y)/delta-1){ // top border mid
            grad_y = slope;
            float l1 = x - delta*(i+0.5);
            float l2 = delta*(i+1.5) - x;
            int k = (x/delta-i >= 0.5) ? i+1 : i;
            j = (temp_upper_y-die_lower_y)/delta-1;
            if(k==i){
                grad_x = (val - phi[k][j])/(delta*l1);
            }else{
                grad_x = (phi[k][j] - val)/(delta*l2);
            }
        }else if(j<=0){
            grad_y = -1*slope;
            float l1 = x - delta*(i+0.5);
            float l2 = delta*(i+1.5) - x;
            int k = (x/delta-i >= 0.5) ? i+1 : i;
            j = 0;
            if(k==i){
                grad_x = (val - phi[k][j])/(delta*l1);
            }else{
                grad_x = (phi[k][j] - val)/(delta*l2);
            }
        }
    }else{ //center
        int k = (x/delta-i >= 0.5) ? i+1 : i; //closest point
        int l = (y/delta-j >= 0.5) ? j+1 : j;
        float l1 = x - delta*(i+0.5);
        float l2 = delta*(i+1.5) - x;
        float w1 = y - delta*(j+0.5);
        float w2 = delta*(j+1.5) - y;
        val = (l2*w2*phi[i][j]+l1*w2*phi[i+1][j]+l2*w1*phi[i][j+1]+l1*w1*phi[i+1][j+1])/(delta*delta);
        if(k==i){
            if(l==j){
                grad_x = (val - phi[k][l])/(delta*l1);
                grad_y = (val - phi[k][l])/(delta*w1);
            }else{
                grad_x = (val - phi[k][l])/(delta*l1);
                grad_y = (phi[k][l] - val)/(delta*w2);
            }
        }else{
            if(l==j){
                grad_x = (phi[k][l] - val)/(delta*l2);
                grad_y = (val - phi[k][l])/(delta*w1);
            }else{
                grad_x = (phi[k][l] - val)/(delta*l2);
                grad_y = (phi[k][l] - val)/(delta*w2);
            }
        }
    }
    
    return make_pair(grad_x, grad_y);
}
// calculate the C^O matrix, which is the gradient of the specified points, val stored on diagonal entries. 
// input params: phi : 2D float array delta: spacing of the phi array(row height)
// x,y: the point of the instances forming a vector, which will later be stored in solution.

// Note that D(i,j) represents the actual coordinate point at h(1/2+i, 1/2+j)
void Kraftwerk2::calc_gradient(unordered_map<string, instance> map, fstream& fout){ 
    for(int j=0; j<D.capacity(); j++){
        for(int k=0; k<D[j].capacity(); k++){
            D[j][k] = 0;
            phi[j][k] = 0;
        }
    }
    cal_D(map, D);
    cal_phi(D, phi, 10);
    /*
    for(int j=0; j<D.capacity(); j++){
        for(int k=0; k<D[j].capacity(); k++){
            cout << D[j][k] << " ";
        }
        cout << endl;
    }
    
    cout << "phi: " << endl;
    for(int j=0; j<D.capacity(); j++){
        for(int k=0; k<D[j].capacity(); k++){
            cout << phi[j][k] << " ";
        }
        cout << endl;
    }
    */
    int delta = min(top_row_height,bottom_row_height);
    for(auto& it : map){
        int n = parse_inst_name(it.first);
        //(x,y) is in (i,j) , (i+1,j+1)
        int width;
        int height;
        if(it.second.tech == TECH::TECH_A){
            width = tech_stack[0].libcells[it.second.libcell_type].width;
            height = tech_stack[0].libcells[it.second.libcell_type].height;
        }
        else{
            width = tech_stack[1].libcells[it.second.libcell_type].width;
            height = tech_stack[1].libcells[it.second.libcell_type].height;
        }

        
        pair<float,float> p = single_point_gradient(phi,solution_x->data[n]+static_cast<float>(width)/2,solution_y->data[n]+static_cast<float>(height)/2,it.second.part);
        
        demand_x->data[n] = p.first;
        demand_y->data[n] = p.second;
        move_force_mat_x->data[n][n] = p.first;
        move_force_mat_y->data[n][n] = p.second;
/*
        fout << it.second.instance_pos.x << " " << it.second.instance_pos.y << " ";
        fout << move_force_mat_x->data[n][n] << " " << move_force_mat_y->data[n][n] << endl;
*/
    }
    //move_force_mat_x->print_data();

}

void Kraftwerk2::update_pos_diff(int phase){
    //Cholesky
    // Matrix C_x(size), C_y(size), LT_x(size), L_x(size), L_y(size), LT_y(size);
    //PLU
    Matrix C_x(size), C_y(size), P_x(size), L_x(size), U_x(size), P_y(size), L_y(size), U_y(size);
    //conjugate
    // Matrix C_x(size), C_y(size);
    Matrix temp = *connectivity_mat;
    // float scalar = 1;
    // if(phase == 1){
    //     Matrix_scalar(*move_force_mat_x,scalar);
    //     Matrix_scalar(*move_force_mat_y,scalar);
    // }
    // else{
    //     Matrix_scalar(temp,scalar);
    // }
    Matrix_Addition(temp, *move_force_mat_x, C_x);
    Matrix_Addition(temp, *move_force_mat_y, C_y);
    // C_x.Cholesky_decomposition(L_x,LT_x);
    // C_y.Cholesky_decomposition(L_y,LT_y);
    C_x.PLU_decomposition(L_x, U_x, P_x);
    C_y.PLU_decomposition(L_y, U_y, P_y);
    /*
    cout << "P_x:" << endl;
    P_x->print_data();
    cout << endl;
    P_y->print_data();
    */
    Vector b_x(size), b_y(size);
    Vector delta_x(size), delta_y(size);
    Matrix_Vector_Prod(*move_force_mat_x, *demand_x, b_x);
    Matrix_Vector_Prod(*move_force_mat_y, *demand_y, b_y);
    //move_force_mat_x->print_data();
    // for(int i = 0; i < size; i++){
    //     cout << demand_x->data[i] << " ";
    // }
    // cout << endl;
    // cout << norm(b_x) << endl;
    // for(int i = 0; i < size; i++){
    //     cout << b_x.data[i] << " ";
    // }
    // delta_x = conjugate_gradient(C_x,b_x);
    // delta_y = conjugate_gradient(C_y,b_y);
    // for(int i = 0; i < size; i++){
    //     cout << delta_x.data[i] << " ";
    // }
    // cout << endl;
     //PLU
    b_x = Vector_scalar(b_x,0.03);
    b_y = Vector_scalar(b_y,0.03);
    solve_linear_system(P_x, L_x, U_x, b_x, delta_x);
    solve_linear_system(P_y, L_y, U_y, b_y, delta_y);
    // solve_linear_system_via_Cholesky(L_x,LT_x,b_x,delta_x);
    // solve_linear_system_via_Cholesky(L_y,LT_y,b_y,delta_y);
    cout << norm(delta_x) << endl;
    for(int i=0; i<size; i++){
        solution_x->data[i] -= delta_x.data[i];
        solution_y->data[i] -= delta_y.data[i];
    }
    /*
    for(int i = 0; i<size; i++)
        cout << delta_x->data[i] << " ";
    cout << endl;
    */
    return;
}

void Kraftwerk2::Kraftwerk2_global_placement(unordered_map<string,instance>& ins, fstream& fout){
    for(int i=0; i<10; i++){
        int phase;
        calc_gradient(ins,fout);
        if(i < 20)
            phase = 1;
        else
            phase = 2;
        update_pos_diff(phase);
        //print_solution(fout);
    }
    cout << "global placement finished \n";
    get_solution(ins);
    
}

//init_penalty
