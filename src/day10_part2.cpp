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
        vector<vector<float>> buttons;
        vector<int> button_order;
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
                    buttons.push_back(initial);
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

            vector<int> pos(rows, 0.0);
            for (int i = 0; i < rows; i++){
                pos[i] = i;
            }
            button_order = pos;
        }
};

void print_machine(Machine m){
    cout << "Rows = " << m.rows << ", colums = " << m.columns << ". Target: " << "\n";
    print(m.target);
    print(m.matrix);
    cout << "State:" << "\n";
    print(m.state);
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

//swap position of row a and b
void swap_rows(vector<vector<float>> &matrix, vector<float> &target, vector<int> &buttons, int a, int b){
    swap(matrix[a], matrix[b]);
    swap(target[a], target[b]);
    swap(buttons[a], buttons[b]);
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
bool select_row(vector<vector<float>> &matrix, vector<float> &target, vector<int> &buttons, int i){
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
        swap_rows(matrix, target, buttons, i, largest_index);
        return true;
    }
}

void solve_matrix(vector<vector<float>> &matrix, vector<float> &target, vector<int> &buttons){
    for (int i = 0; i < target.size(); i++){
        //next row must have non-zero value at matrix[i][i]
        bool found_row = select_row(matrix, target, buttons, i); 

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

//assume square matrix
void transpose_matrix(vector<vector<float>> &matrix){
    for (int i = 0; i < matrix.size(); i++){
        for (int j = i; j < matrix.size(); j++){
            swap(matrix[i][j], matrix[j][i]);
        }
    }
}

void push_button(Machine &m, vector<float> button, int n){
    //loop through joltage counters, push button n times
    for (int i; i < m.state.size(); i++){
        if (button[i] > 0){
            m.state[i] += n;
        }
    }
}

void apply_solution(Machine &m){
    //loop through buttons (they have been re-ordered)
    for (int i = 0; i < m.rows; i++){
        push_button(m, m.buttons[m.button_order[i]], m.target[i]);
    }
}

//assume matrix is square
vector<float> mat_mul(vector<vector<float>> &matrix, vector<float> &vec){
    vector<float> solution(matrix.size(), 0.0);
    for (int row = 0; row < matrix.size(); row++){
        float sum = 0;
        for (int column = 0; column < matrix.size(); column++){
            sum += matrix[row][column] * vec[column];
        }
        solution[row] = sum;
    }
    return solution;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;
    
    transpose_matrix(machines[1].matrix);
    solve_matrix(machines[1].matrix, machines[1].target, machines[1].button_order);
    //machines[1].target = {2,5,0,5,0};
    //apply_solution(machines[1]);
    print_machine(machines[1]);
    //print(mat_mul(machines[1].matrix, machines[1].target));
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}

//110197 is too high