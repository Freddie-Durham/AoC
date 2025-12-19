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
using namespace std;

struct Edge{
    public:
        int a, b;
        long long distance;
        Edge(int v1, int v2, long long dist){
            a = v1;
            b = v2;
            distance = dist;
        }
};

class CircuitBox{
    public:
        vector<long long> pos;
        int parent;
        int size;

        CircuitBox(int index, vector<long long> p, int c){
            pos = p;
            parent = index;
            size = 1;
        }

        void printbox(){
            cout << "Parent = " << parent << ". Pos = ";
            print(pos);
        }

        long long sq_dist(const CircuitBox &c){
            long long dist = 0;
            for (int i = 0; i < pos.size(); i++){
                long long d = pos[i] - c.pos[i];
                dist += d * d;
            }
            return dist;
        }
};

int find_set(vector<CircuitBox>& c, int i) {
    if (i == c[i].parent)
        return i;
    return c[i].parent = find_set(c, c[i].parent);
}

int union_sets(vector<CircuitBox>& c, int a, int b) {
    a = find_set(c, a);
    b = find_set(c, b);
    if (a != b){
        if (c[a].size < c[b].size){
            swap(a, b);
        }
        c[b].parent = a;
        c[a].size += c[b].size;
        return c[a].size;
    }
    else {
        return 0;
    }
}

vector<CircuitBox> copy_vec(vector<CircuitBox> vec){
    vector<CircuitBox> vec_copy;
    for (CircuitBox v : vec){
        vec_copy.push_back(v);
    }
    return vec_copy;
}

void connect_boxes(vector<CircuitBox>& boxes, vector<Edge>& edges){
    for (Edge edge : edges){
        int t = edge.a;
        int n = edge.b;
        CircuitBox& target = boxes[t];
        CircuitBox& neighbour = boxes[n];

        int cur_size = union_sets(boxes, t, n);
        if (cur_size == boxes.size()){
            target.printbox();
            neighbour.printbox();
            cout << "Score = " << target.pos[0] * neighbour.pos[0];
            break;
        }
    }
}

bool compare(Edge e1, Edge e2){
    return e1.distance < e2.distance;
}

vector<Edge> get_edges(vector<CircuitBox>& c_boxes){
    vector<Edge> edges;
    for (int i = 0; i < c_boxes.size(); i++){
        for (int j = i + 1; j < c_boxes.size(); j++){
            long long dist = LLONG_MAX;
            dist = c_boxes[i].sq_dist(c_boxes[j]);
            edges.push_back(Edge(i, j, dist));
        }
    }
    return edges;
}

void analyse(string file){
    vector<vector<long long>> boxes = get_array(file, ',');
    vector<CircuitBox> c_boxes;

    for (int i = 0; i < boxes.size(); i++){
        CircuitBox c_box = CircuitBox(i, boxes[i], i);
        c_boxes.push_back(c_box);
    }

    vector<Edge> edges = get_edges(c_boxes);
    sort(edges.begin(), edges.end(), compare);
    connect_boxes(c_boxes, edges);

    /*
    unordered_map<int, int> dict; //first is set parent ID, second is number of members
    for (int i = 0; i < c_boxes.size(); i++){
        int set = find_set(c_boxes, i);
        dict[set]++;
    }

    vector<long long> count_vec;
    for (const auto& [key, value] : dict) {
        count_vec.push_back(value);
    }
    sort(count_vec.begin(), count_vec.end(), greater<long long>());
    print(count_vec);
    long long score = count_vec[0] * count_vec[1] * count_vec[2];
    cout << "Score = " << score;
    */
}

int main() {
    string file = "../input/day8.txt";
    analyse(file);
    return 0;
}