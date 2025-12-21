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

class Machine{
    public:
        vector <float> target;
        vector<vector<float>> matrix;
        vector<int> state;
        int columns;
        int rows;

        Machine(string str){
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    columns = sec.size() - 2;
                }
                else if (sec[0] == '('){
                    vector<float> initial(columns, 0.0);
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        initial[(stoi(num))] = 1.0;
                    }
                    matrix.push_back(initial);
                }
                else if (sec[0] == '{'){
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        target.push_back(stof(num));
                    }
                }
            }
            rows = matrix.size();
            vector<int> initial(columns, 0.0);
            state = initial;
        }
};

void print_machine(Machine m){
    cout << "Rows = " << m.rows << ", colums = " << m.columns << ". Target: " << "\n";
    print(m.target);
    print(m.matrix);
    cout << "State:" << "\n";
    print(m.state);
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

//swap position of row a and b
void swap_rows(vector<vector<float>> &matrix, vector<float> &target, int a, int b){
    swap(matrix[a], matrix[b]);
    swap(target[a], target[b]);
}

//multiply row a by v
void multiply_row(vector<vector<float>> &matrix, vector<float> &target, int a, float v){
    //could also use: transform(row.begin(), row.end(), row.begin(), [v](float x) { return x * v; });
    for (int i = 0; i < target.size(); i++){
        matrix[a][i] *= v;
    }
    target[a] *= v;
}

//add row b, multiplied by scalar v, to row a
void add_rows(vector<vector<float>> &matrix, vector<float> &target, int a, int b, float v){
    for (int i = 0; i < target.size(); i++){
        matrix[a][i] += matrix[b][i] * v;
    }    
    target[a] += target[b] * v;
}

//find first row where ith column is non-zero and swap that row with ith row
//return false if there are no rows from i to end where matrix[row][i] != 0
bool select_row(vector<vector<float>> &matrix, vector<float> &target, int i){
    int largest_index = i;
    float largest_value = 0;
    for (int row = i; row < target.size(); row++){
        if (matrix[row][i] > largest_value){
            largest_value = matrix[row][i];
            largest_index = row;
        }
    }

    if (largest_value == 0){
        return false;
    }
    else{
        swap_rows(matrix, target, i, largest_index);
        return true;
    }
}

void solve_matrix(vector<vector<float>> &matrix, vector<float> &target){
    for (int i = 0; i < target.size(); i++){
        //next row must have non-zero value at matrix[i][i]
        bool found_row = select_row(matrix, target, i); 

        if (found_row){
            multiply_row(matrix, target, i, 1 / matrix[i][i]);
            for (int j = 0; j < target.size(); j++){
                if (j != i){
                    add_rows(matrix, target, j, i, -matrix[j][i]);
                }
            }
        }
    }
}

void apply_solution(Machine &machine){
    int pos = 0;
    for (int t : machine.target){
        for (float v : machine.matrix[pos]){
            machine.state[pos] += v * t;
        }
        pos ++;
    }
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;
    
    //print_machine(machines[1]);
    solve_matrix(machines[1].matrix, machines[1].target);
    apply_solution(machines[1]);
    print_machine(machines[1]);
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}

//110197 is too high