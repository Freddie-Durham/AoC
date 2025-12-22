//Failed? attempt to solve day 10

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

//assume square matrix
vector<vector<float>> transpose_matrix(vector<vector<float>> &matrix, int columns, int rows){
    vector<float> new_row(rows, 0.0);
    vector<vector<float>> new_matrix(columns, new_row);
    for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            new_matrix[i][j] = matrix[j][i];
        }
    }
    return new_matrix;
}

class Machine{
    public:
        vector <float> target;
        vector<vector<float>> matrix;
        vector<vector<float>> buttons;
        vector<int> button_order;
        vector<int> state;
        int columns;
        int rows;

        Machine(string str){
            vector<vector<float>> newM;
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
                    newM.push_back(initial);
                }
                else if (sec[0] == '{'){
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        target.push_back(stof(num));
                    }
                }
            }
            rows = newM.size();
            vector<int> initial(columns, 0.0);
            state = initial;

            vector<int> pos(rows, 0.0);
            for (int i = 0; i < rows; i++){
                pos[i] = i;
            }
            button_order = pos;
            buttons = newM;
            matrix = transpose_matrix(newM, columns, rows);
        }
};

void print_machine(Machine m){
    cout << "Rows = " << m.rows << ", colums = " << m.columns << ". Target: " << "\n";
    print(m.target);
    print(m.matrix);
    //cout << "State:" << "\n";
    //print(m.state);
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

void push_button(Machine &m, vector<float> button, int n){
    //loop through joltage counters, push button n times
    for (int i; i < m.columns; i++){
        if (button[i] > 0){
            m.state[i] += n;
        }
    }
}

void apply_solution(Machine &m){
    //loop through buttons (they have been re-ordered)
    for (int i = 0; i < m.rows; i++){
        push_button(m, m.buttons[i], m.target[i]);
    }
}

//swap position of row a and b
void swap_rows(vector<vector<float>> &matrix, vector<float> &target, int a, int b){
    if (a != b){
        swap(matrix[a], matrix[b]);
        swap(target[a], target[b]);
    }
}

//multiply row a by v
void multiply_row(vector<vector<float>> &matrix, vector<float> &target, int a, float v){
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
bool select_row(vector<vector<float>> &matrix, vector<float> &target, const int &i, const int &rows){
    int largest_index = i;
    float largest_value = 0;
    for (int row = i; row < rows; row++){
        if (abs(matrix[row][i]) > largest_value){
            largest_value = matrix[row][i];
            largest_index = row;
        }
    }

    if (largest_value == 0){
        return false;
    }
    else{
        cout << "Largest index = " << largest_index << "\n";
        swap_rows(matrix, target, i, largest_index);
        return true;
    }
}

void solve_matrix(vector<vector<float>> &matrix, vector<float> &target, const int &rows){
    for (int i = 0; i < rows; i++){
        //next row must have non-zero value at matrix[i][i]
        bool found_row = select_row(matrix, target, i, rows); 

        if (found_row){
            multiply_row(matrix, target, i, 1 / matrix[i][i]);
            for (int j = 0; j < rows; j++){
                if (j != i){
                    add_rows(matrix, target, j, i, -matrix[j][i]);
                }
            }
        }
        print(matrix);
        cout << i << "\n";
    }
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;

    for (int i = 0; i < 3; i++){
        solve_matrix(machines[i].matrix, machines[i].target, machines[i].rows);
        print_machine(machines[i]);

        //apply_solution(machines[i]);
        //print(machines[i].state);
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}

//110197 is too high