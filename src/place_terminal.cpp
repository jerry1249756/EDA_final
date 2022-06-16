#include "place_terminal.h"

Terminal_Placment::Terminal_Placment(int terminal_x, int terminal_y, int terminal_spacing){
    border_width = float(terminal_spacing)/2;
    grid_width = terminal_x + terminal_spacing;
    grid_height = terminal_y + terminal_spacing;
    int m = (die_upper_x - die_lower_x - terminal_spacing)/grid_width;
    int n = (min(top_die_upper_y, bottom_die_upper_y) - die_lower_y - terminal_spacing)/grid_height;
    data.resize(m);
    for(int i=0; i<data[i].size(); i++){
        data[i].resize(n);
    }
    if(top_die_tech == "TA" && bottom_die_tech == "TB"){
        toptech = 0;
        bottomtech = 1;
    }
    else if(top_die_tech == "TB" && bottom_die_tech == "TA"){
        toptech = 1;
        bottomtech = 0;
    }
    else{
        toptech = 0;
        bottomtech = 0;
    }
}

point Terminal_Placment::transform_from_grid_to_pos(int i , int j){
    int x = border_width + (i + 0.5)*grid_width;
    int y = border_width + (j + 0.5)*grid_height;
    return point(x,y);
}

point Terminal_Placment::transform_from_pos_to_grid(int x , int y){
    int i = static_cast<int>((x-border_width)/grid_width-0.5);
    int j = static_cast<int>((y-border_width)/grid_height-0.5);
    return point(i,j);
}

point Terminal_Placment::find_pos(unordered_map<string, instance>& instances, unordered_map<string, net*>& nets, string netname){
    int TopDie_rightmost = INT32_MIN;
    int TopDie_leftmost = INT32_MAX;
    int TopDie_topmost = INT32_MIN;
    int TopDie_bottommost = INT32_MAX;
    int BottomDie_rightmost = INT32_MIN;
    int BottomDie_leftmost = INT32_MAX;
    int BottomDie_topmost = INT32_MIN;
    int BottomDie_bottommost = INT32_MAX;
    int netsize = nets[netname]->net_pin.size();
    for(int i=0; i<netsize; i++){
        point ins_pos = instances[nets[netname]->net_pin[i].INSTANCE].instance_pos;
        if(instances[nets[netname]->net_pin[i].INSTANCE].part == PART::TOP){  // this instance belongs to TOP
            point pin_pos = tech_stack[toptech].libcells[instances[nets[netname]->net_pin[i].INSTANCE].libcell_type].pins[nets[netname]->net_pin[i].PIN].pin_pos;
            if(pin_pos.x+ins_pos.x > TopDie_rightmost) TopDie_rightmost = pin_pos.x+ins_pos.x;
            if(pin_pos.x+ins_pos.x < TopDie_leftmost) TopDie_leftmost = pin_pos.x+ins_pos.x;
            if(pin_pos.y+ins_pos.y > TopDie_topmost) TopDie_topmost = pin_pos.y+ins_pos.y;
            if(pin_pos.y+ins_pos.y < TopDie_bottommost) TopDie_bottommost = pin_pos.y+ins_pos.y;
        }
        else{
            point pin_pos = tech_stack[bottomtech].libcells[instances[nets[netname]->net_pin[i].INSTANCE].libcell_type].pins[nets[netname]->net_pin[i].PIN].pin_pos;
            if(pin_pos.x+ins_pos.x > BottomDie_rightmost) BottomDie_rightmost = pin_pos.x+ins_pos.x;
            if(pin_pos.x+ins_pos.x < BottomDie_leftmost) BottomDie_leftmost = pin_pos.x+ins_pos.x;
            if(pin_pos.y+ins_pos.y > BottomDie_topmost) BottomDie_topmost = pin_pos.y+ins_pos.y;
            if(pin_pos.y+ins_pos.y < BottomDie_bottommost) BottomDie_bottommost = pin_pos.y+ins_pos.y;
        }
    }
    //take center point and return the grid point
    int x, y, i, j;
    if(TopDie_bottommost < BottomDie_topmost){  //overlap case
        y = (TopDie_bottommost + BottomDie_topmost)/2;
        if(TopDie_leftmost < BottomDie_rightmost){ //if x overlap
            x = (TopDie_leftmost + BottomDie_rightmost)/2;
        }else if(TopDie_rightmost > BottomDie_leftmost){
            x = (TopDie_rightmost + BottomDie_leftmost)/2;
        }
    }else if(TopDie_topmost > BottomDie_bottommost){
        y = (TopDie_topmost + BottomDie_bottommost)/2;
        if(TopDie_leftmost < BottomDie_rightmost){ //if x overlap
            x = (TopDie_leftmost + BottomDie_rightmost)/2;
        }else if(TopDie_rightmost > BottomDie_leftmost){
            x = (TopDie_rightmost + BottomDie_leftmost)/2;
        }
    }else{ //no overlap in the bounding box
        x = (min(TopDie_topmost, BottomDie_topmost) + max(TopDie_bottommost, BottomDie_bottommost) )/2;
        y = (min(TopDie_rightmost, BottomDie_rightmost) + max(TopDie_leftmost, BottomDie_leftmost) )/2;
    }

    point temp = transform_from_pos_to_grid(x , y);
    return temp;
}

point Terminal_Placment::lee_algorithm(int i, int j){ //find the closest point whose value is 0 from the point i,j
    queue<point> Q;
    Q.push(point(i,j));
    while(!Q.empty()){
        point p = Q.front();
        if(data[p.x][p.y]==0) return p;
        else{
            if(i<data.size()-2) Q.push(point(i+1,j)); //right propagate
            if(i>1) Q.push(point(i-1,j)); //left propagate
            if(j<data[0].size()-2) Q.push(point(i,j+1)); // up propagate
            if(j>1) Q.push(point(i,j-1)); //down propagate
        }   
        Q.pop();
    }
}

void Terminal_Placment::Thorolfsson_via_assignment(unordered_map<string, instance>& instances, unordered_map<string, net*>& nets){
    for(auto& it : nets){
        if(it.second->is_cut()){
            point p = find_pos(instances, nets, it.second->Net_name);
            if(data[p.x][p.y]==0){ //if there is no terminal
                data[p.x][p.y]=1;
                point real_pos = transform_from_grid_to_pos(p.x,p.y);
                it.second->terminal_pos = real_pos;
            }else{ //there is a terminal, find a closest place to put
                point q = lee_algorithm(p.x, p.y);
                data[q.x][q.y]=1;
                point real_pos = transform_from_grid_to_pos(q.x,q.y);
                it.second->terminal_pos = real_pos;
            }
        }
    }
    return;
}