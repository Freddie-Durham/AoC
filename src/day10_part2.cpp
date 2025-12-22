#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <climits>
#include <stdint.h>
#include <bitset>
using namespace std;

long long FAIL = 1000000;

struct Point {
    public:
        vector<long long> vec;

        Point() {} 
        Point(const vector<long long>& v) : vec(v) {}

        bool operator==(const Point& other) const {
            return vec == other.vec;
        }
};

/*
struct PointHasher {
    std::size_t operator()(const Point& p) const {
        // A simple bit-shifting hash combine
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};*/

struct Node{
    public:
        Point pos;
        long long g; //distance from root
        long long h; //heuristic distance from target
        long long f; //f = g + h

        //construct start and target nodes
        Node(const vector <long long> &p, long long dist, long long goal_dist){
            pos = Point(p);
            g = dist;
            h = goal_dist;
            f = g + h;
        }

        //Euclidian distance
        long long calculate_distance(const vector <long long> &cur_pos, const Node &target){
            long long dist = 0;
            for (int i = 0; i < cur_pos.size(); i++){
                long long diff = (cur_pos[i] - target.pos.vec[i]);
                dist += diff * diff;
            }
            return dist;
        }

        //construct new node from parent
        Node(const Node &prev_node, const Node &target, vector<long long> edge){
            for (int i = 0; i < edge.size(); i++){
                edge[i] += prev_node.pos.vec[i];
            }

            pos = Point(edge);
            g = prev_node.g + 1; //unweighted graph
            h = calculate_distance(edge, target);
            f = g + h;
        }

        bool is_equal(const Node &a) const {
            return a.pos == pos;
        }

        bool is_better(const Node &a) const {
            return f < a.f;
        }

        const void print_node() const {
            cout << "Current position:" << "\n";
            print(pos.vec);
            cout << "Node score = " << f << "\n";
        }
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        return b.is_better(a);
    }
};

void print_list(const vector<Node> &list){
    for (const Node &n : list){
        n.print_node();
    }
}

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

//returns location of node in closed list. -1 if not in list.
int in_closed_list(const vector<Node> &list, const Node &node){
    for (int i = 0; i < list.size(); i++){
        if (list[i].is_equal(node)){
            return i;
        }
    }
    return -1;
}

long long shortest_path(Machine &machine){
    //we want next best node to always be at the top
    priority_queue<Node, vector<Node>, CompareNode> open_list;
    //should be easy to search to see if node already visited - use a hashmap
    vector<Node> closed_list; //unordered_map<vector<long long>, long long> closed_list; 

    Node goal_node(machine.target, 0, 0);
    vector<long long> start_pos(machine.target.size(), 0);
    Node start_node(start_pos, 0, 0);

    open_list.push(start_node);

    while (open_list.size() > 0){
        Node cur_node = open_list.top();
        open_list.pop();

        int closed_ind = in_closed_list(closed_list, cur_node);

        if ((closed_ind < 0) || (cur_node.is_better(closed_list[closed_ind]))){
            closed_list.push_back(cur_node);
            
            if (goal_node.is_equal(cur_node)){
                return cur_node.g;
            }

            for (vector<long long> &edge : machine.buttons){
                Node new_node(cur_node, goal_node, edge);
                int closed_index = in_closed_list(closed_list, new_node);
                if (closed_index < 0){
                    open_list.push(new_node);
                }
                else if (new_node.is_better(closed_list[closed_index])){
                    closed_list.erase(closed_list.begin() + closed_index);
                    open_list.push(new_node);
                }
            }
        }
    }
    return FAIL;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;

    for (int i = 0; i < machines.size(); i++){
        print_machine(machines[i]);
        int len = shortest_path(machines[i]);
        cout << "Found shortest path with length = " << len << "\n";
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}