#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <climits>
#include <stdint.h>
#include <bitset>
using namespace std;

long long FAIL = 1000000;

struct Node{
    public:
        vector <long long> pos;
        long long g; //distance from root
        long long h; //heuristic distance from target
        long long f; //f = g + h

        //construct start and target nodes
        Node(vector <long long> &p, long long dist, long long goal_dist){
            pos = p;
            g = dist;
            h = goal_dist;
            f = g + h;
        }

        //Euclidian distance
        long long calculate_distance(vector <long long> &cur_pos, Node &target){
            long long dist = 0;
            for (int i = 0; i < cur_pos.size(); i++){
                long long diff = (cur_pos[i] - target.pos[i]);
                dist += diff * diff;
            }
            return dist;
        }

        //construct new node from parent
        Node(Node &prev_node, Node &target, vector <long long> edge){
            for (int i = 0; i < edge.size(); i++){
                edge[i] += prev_node.pos[i];
            }

            pos = edge;
            g = prev_node.g + 1; //unweighted graph
            h = calculate_distance(pos, target);
            f = g + h;
        }

        bool is_equal(Node &a){
            return a.pos == pos;
        }

        bool is_better(Node &a){
            return f < a.f;
        }

        void print_node(){
            cout << "Current position:" << "\n";
            print(pos);
            cout << "Node score = " << f << "\n";
        }
};

class Machine{
    public:
        vector <long long> target;
        vector<vector<long long>> buttons;
        int counter_num;
        int button_num;

        Machine(string str){
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    counter_num = sec.size() - 2;
                }
                else if (sec[0] == '('){
                    vector<long long> initial(counter_num, 0);
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        initial[(stoi(num))] = 1;
                    }
                    buttons.push_back(initial);
                }
                else if (sec[0] == '{'){
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        target.push_back(stof(num));
                    }
                }
            }
            button_num = buttons.size();
        }
};

void print_machine(Machine m){
    cout << "Target:" << "\n";
    print(m.target);
    cout << "Buttons:" << "\n";
    print(m.buttons);
}

vector<Machine> get_machines(string file){
    string line;
    string sequence;
    ifstream data(file);
    vector<Machine> array;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                array.push_back(Machine(sequence));
    }}
    data.close();
    }    
    else{
    cout << "Unable to open data file";
    }
    return array;
}

//returns false if node is not in open list, or if node is better
bool check_open_list(vector<Node> &list, Node &node){
    for (int i = 0; i < list.size(); i++){
        if (list[i].is_equal(node)){
            if (node.is_better(list[i])){
                list[i].f = node.f;
                list[i].g = node.g;
            }
            return true;
        }
    }
    return false;
}

long long shortest_path(Machine &machine){
    vector<Node> open_list; //we want next best node to always be at the top
    vector<Node> closed_list; //should be easy to search to see if node already visited

    Node goal_node(machine.target, 0, 0);
    vector<long long> start_pos(machine.target.size(), 0);
    Node start_node(start_pos, 0, 0);

    open_list.push_back(start_node);

    while (open_list.size() > 0){
        Node cur_node = move(open_list.back());
        open_list.pop_back();

        closed_list.push_back(cur_node);

        if (goal_node.is_equal(cur_node)){
            return cur_node.g;
        }

        for (vector<long long> &edge : machine.buttons){
            Node new_node(cur_node, goal_node, edge);
            new_node.print_node();
        }
        break;
    }
    return FAIL;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;

    for (int i = 0; i < machines.size() - 2; i++){
        print_machine(machines[i]);
        shortest_path(machines[i]);
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}