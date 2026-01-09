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
};

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

struct Edge{
    public:
        int type, pos;

        Edge(const Corner& prev, const Corner& next){
            if (prev.x == next.x){
                pos = prev.x;
                if (prev.y - next.y < 0){
                    type = UP;
                }
                else{
                    type = DOWN;
                }
            }
            else if (prev.y == next.y){
                pos = prev.y;
                if (prev.x - next.x < 0){
                    type = RIGHT;
                }
                else{
                    type = LEFT;
                }
            }
            else{
                cout << "Error - points not aligned" << "\n";
            }
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

    for (int i = 1; i < corners.size(); i++){
        edges.push_back(Edge(corners[i-1], corners[i]));
    }
    edges.push_back(Edge(corners[corners.size()-1], corners[0]));
    return edges;
}

bool inside_perimeter(const Corner& c, const vector<Edge>& edges){
    int above = 0;
    int below = 0;
    int left = 0;
    int right = 0;

    for (const Edge& edge : edges){
        if (edge.type == UP && c.x <= edge.pos){
            right ++;
        }
        else if (edge.type == DOWN && c.x >= edge.pos){
            left ++;
        }
        else if (edge.type == LEFT && c.y <= edge.pos){
            above ++;
        }
        else if (edge.type == RIGHT && c.y >= edge.pos){
            below ++;
        }
    }
    return right % 2 != 0;
}

void print_tiles(const vector<Edge>& edges, int width, int height){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if (inside_perimeter(Corner(i, j), edges)){
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

    for (Edge& e : edges){
        cout << e.type << ", " << e.pos << "\n";
    }
    print_tiles(edges, 12, 12);

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