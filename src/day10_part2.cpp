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

struct PointHasher {
    size_t operator()(const Point& p) const {
        size_t seed = 0;
        for (long long val : p.vec) {
            seed ^= hash<long long>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct Node{
    public:
        Point pos;
        double g; //distance from root
        double h; //heuristic distance from target
        double f; //f = g + h

        //construct start and target nodes
        Node(const vector <long long> &p, double dist, double goal_dist){
            pos = Point(p);
            g = dist;
            h = goal_dist;
            f = g + h;
        }

        //Euclidian distance
        double calculate_distance(const vector <long long> &cur_pos, const Node &target){
            double dist = 0;
            for (int i = 0; i < cur_pos.size(); i++){
                double diff = (cur_pos[i] - target.pos.vec[i]);
                dist += diff * diff;
            }
            return sqrt(dist);
        }

        //construct new node from parent
        Node(const Node &prev_node, const Node &target, vector<long long> edge){
            for (int i = 0; i < edge.size(); i++){
                edge[i] += prev_node.pos.vec[i];
            }

            pos = Point(edge);
            g = prev_node.g + 1.0; //unweighted graph
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

bool not_visited(unordered_map<Point, double, PointHasher> &list, const Node &node){
    //lookup node in closed list    
    auto iter = list.find(node.pos);
    //check if node is in closed list or current node is better than node in closed list
    return ((iter == list.end()) || (node.g < iter->second));
}

double shortest_path(Machine &machine){
    //we want next best node to always be at the top
    priority_queue<Node, vector<Node>, CompareNode> open_list;
    //should be easy to search to see if node already visited - use a hashmap
    unordered_map<Point, double, PointHasher> closed_list;

    Node goal_node(machine.target, 0, 0);
    vector<long long> start_pos(machine.target.size(), 0);
    Node start_node(start_pos, 0, 0);

    open_list.push(start_node);

    while (!open_list.empty()){
        //grab next best node to search and remove from open list
        Node cur_node = open_list.top();
        open_list.pop();

        //check if node is in closed list or current node is better than node in closed list
        if (not_visited(closed_list, cur_node)){
            //add current node to closed list (or update existing g score)
            closed_list[cur_node.pos] = cur_node.g;
            
            //success condition
            if (goal_node.is_equal(cur_node)){
                return cur_node.g;
            }

            //loop through children of current node
            for (const vector<long long> &edge : machine.buttons){
                Node new_node(cur_node, goal_node, edge);
                //if not in closed list, add to open list
                if (not_visited(closed_list, new_node)){
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

        double len = shortest_path(machines[i]);
        cout << "Found shortest path with length = " << len << "\n";
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}