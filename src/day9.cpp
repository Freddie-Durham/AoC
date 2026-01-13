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

        bool is_vertical(const Corner& other) const{
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
        bool is_vertical;

        Edge(const Corner& prev, const Corner& next){
            is_vertical = prev.is_vertical(next);
            if (is_vertical){
                pos = prev.x;
                upper = max(prev.y, next.y);
                lower = min(prev.y, next.y);
            }
            else{
                pos = prev.y;
                upper = max(prev.x, next.x);
                lower = min(prev.x, next.x);
            }
        }

        bool on_boundary(const Corner& c) const{
            if (is_vertical){
                if ((c.x == pos) && (c.y <= upper) && (c.y >= lower)){
                    return true;
                }
            }
            else{
                if ((c.y == pos) && (c.x <= upper) && (c.x >= lower)){
                    return true;
                }
            }
            return false;
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
        edges.push_back(Edge(cur_corner, corners[i]));
        cur_corner = corners[i];
    }
    cout << "Num edges = " << edges.size() << ". Num corners = " << corners.size() << "\n";
    return edges;
}

bool inside_perimeter(const Corner& c, const vector<Edge>& edges){
    int edge_intersect = 0;
    for (const Edge& edge : edges){
        if (edge.on_boundary(c)){
            return true;
        }
        else if (edge.is_vertical && (edge.pos > c.x) && (edge.upper > c.y) && (edge.lower <= c.y)){
            edge_intersect ++;
        }
    }
    return edge_intersect % 2 != 0;
}

bool inside(const Corner& c, const vector<Edge>& edges){
    for (const Edge& edge : edges){
        if (edge.on_boundary(c)){
            return true;
        }
    }
    return false;
}

void print_tiles(const vector<Edge>& edges, int width, int height){
    for (int j = 1; j < height; j++){
        for (int i = 1; i < width; i++){
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

bool edge_not_intersect(const vector<Edge>& edges, const Edge& new_edge){
    for (const Edge& edge : edges){
        if (edge.is_vertical ^ new_edge.is_vertical){
            if ((new_edge.lower < edge.pos) && (new_edge.upper > edge.pos)){
                if ((edge.lower < new_edge.pos) && (edge.upper > new_edge.pos)){
                   return false; 
                }
            }
        }
    }
    return true;
}

bool rectangle_inside(const vector<Edge>& edges, const Corner& a, const Corner& b){
    Corner c1(a.x, b.y);
    Corner c2(b.x, a.y);

    if (inside_perimeter(c1, edges) && inside_perimeter(c2, edges)){
        Edge ea1(a, c1);
        Edge ea2(a, c2);
        Edge eb1(b, c1);
        Edge eb2(b, c2);

        if (edge_not_intersect(edges, ea1) && edge_not_intersect(edges, ea2) &&
            edge_not_intersect(edges, eb1) && edge_not_intersect(edges, eb2)){
            return true;
        }
    }
    return false;
}
 
void analyse(string file){
    vector<Corner> corners = get_corners(file);
    vector<Edge> edges = get_edges(corners);

    //print_tiles(edges, 1000, 1000);
    long long max_area = 0;
    for (int i = 0; i < corners.size(); i++){
        for (int j = i + 1; j < corners.size(); j++){
            if (rectangle_inside(edges, corners[i], corners[j])){
                max_area = max(max_area, corners[i].area(corners[j]));
            }
        }
    }
    cout << "Max area = " << max_area << "\n";
}

int main() {
    string file = "../input/day9.txt";
    analyse(file);
    return 0;
}

//4638696212 too high
//1325340522 too low