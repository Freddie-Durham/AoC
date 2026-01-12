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

struct Corner{
    public:
        long long x, y;
        Corner(long long v1, long long v2){
            x = v1;
            y = v2;
        }
        Corner(string str){
            vector<string> nums = split(str, ',');
            x = stoll(nums[0]);
            y = stoll(nums[1]);
        }

        long long area(const Corner& other){
            return (abs(x - other.x) + 1) * (abs(y - other.y) + 1);
        }

        bool is_vertical(const Corner& other){
            if (x == other.x){
                return true;
            }
            else{
                return false;
            }
        }
};

struct Edge{
    public:
        int pos, upper, lower;

        Edge(const Corner& prev, const Corner& next){
            pos = prev.x;
            upper = max(prev.y, next.y);
            lower = min(prev.y, next.y);
        }
};

vector<Corner> get_corners(string file){
    string line;
    string sequence;
    ifstream data(file);
    vector<Corner> corners;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                corners.push_back(Corner(sequence));
            }
        }
    data.close();
    }    
    else{
    cout << "Unable to open data file";
    }
    return corners;
}

vector<Edge> get_edges(const vector<Corner>& corners){
    vector<Edge> edges;
    Corner cur_corner = corners[corners.size()-1];

    for (int i = 0; i < corners.size(); i++){
        if (cur_corner.is_vertical(corners[i])){
            edges.push_back(Edge(cur_corner, corners[i]));
        }
        cur_corner = corners[i];
    }
    cout << "Num edges = " << edges.size() << ". Num corners = " << corners.size() << "\n";
    return edges;
}

bool inside_perimeter(const Corner& c, const vector<Edge>& edges){
    int edge_intersect = 0;
    for (const Edge& edge : edges){
       if ((edge.pos > c.x) && (edge.upper >= c.y) && (edge.lower <= c.y)){
            edge_intersect ++;
       }
    }
    return edge_intersect % 2 != 0;
}

bool inside(const Corner& c, const vector<Edge>& edges){
    for (const Edge& edge : edges){
        if ((c.x == edge.pos) && ((c.y == edge.upper) || (c.y == edge.lower))){
            return true;
        }
    }
    return false;
}

void print_tiles(const vector<Edge>& edges, int width, int height){
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            if (inside_perimeter(Corner(i, j), edges)){ //(inside(Corner(i, j), edges)){
                cout << "X";
            }
            else{
                cout << ".";
            }
        }
        cout << "\n";
    }
}

void analyse(string file){
    vector<Corner> corners = get_corners(file);
    vector<Edge> edges = get_edges(corners);

    for (const Edge& edge : edges){
        cout << edge.upper << ":" << edge.lower << ", " << edge.pos << "\n";
    }
    print_tiles(edges, 14, 9);

    /*
    long long max_area = 0;

    for (int i = 0; i < corners.size(); i++){
        for (int j = i + 1; j < corners.size(); j++){
            max_area = max(max_area, corners[i].area(corners[j]));
        }
    }
    cout << "Max area = " << max_area << "\n";
    */
}

int main() {
    string file = "../input/day9_test.txt";
    analyse(file);
    return 0;
}