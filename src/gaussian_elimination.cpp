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
        int num_displays;
        int num_buttons;

        Machine(string str){
            vector<vector<float>> newM;
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    num_displays = sec.size() - 2;
                }
                else if (sec[0] == '('){
                    vector<float> initial(num_displays, 0.0);
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
            num_buttons = newM.size(); //number of buttons

            //if there are more displays than buttons, pad the matrix with buttons that do nothing
            while (newM.size() < num_displays){ 
                newM.push_back(vector<float>(num_displays, 0.0));
            }

            //if there are more buttons than displays, invent new displays
            //these displays have a target of total button presses (which we don't know)
            //these displays are incremented by every button
            while (newM[0].size() < num_buttons){ 
                target.push_back(7.0);
                for (vector<float> &vec : newM){
                    vec.push_back(1.0);
                }
            }
            matrix = transpose_matrix(newM, newM.size(), newM[0].size());
        }
};

void print_machine(Machine m){
    cout << "Buttons = " << m.num_buttons << ", displays = " << m.num_displays << ". Target: " << "\n";
    print(m.target);
    print(m.matrix);
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
        swap_rows(matrix, target, i, largest_index);
        return true;
    }
}

vector<float> solve_matrix(vector<vector<float>> matrix, vector<float> target, const int &rows){
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
    }
    return target;
}

long long next_highest(vector<float>& target){
    long long max_val = 0;
    long long max_ind;
    for (int i = 0; i < target.size(); i++){
        if (target[i] > max_val){
            max_val = target[i];
            max_ind = i;
        }
    }
    return max_val;
}

long long sum(vector<float>& target){
    long long val = 0;
    for (int i = 0; i < target.size(); i++){
        val += target[i];
    }
    return val;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;
    vector<float> solution;

    for (int i = 0; i < machines.size(); i++){
        long long button_presses = 0;
        if (machines[i].num_displays < machines[i].num_buttons){
            long long min_presses = next_highest(machines[i].target);
            long long max_presses = sum(machines[i].target);
            bool success = false;

            while (!success && min_presses <= max_presses){
                for (long long j = machines[i].num_displays; j < machines[i].num_buttons; j++){
                    machines[i].target[j] = float(min_presses);
                }
                solution = solve_matrix(machines[i].matrix, machines[i].target, machines[i].matrix.size());
                //print(solution);

                success = true;
                for (int j = 0; j < machines[i].num_buttons; j++){
                    if ((solution[j] < 0) || (ceilf(solution[j]) != solution[j])){
                        success = false;
                    }
                }
                min_presses ++;
            }
            if (min_presses > max_presses){
                cout << "Failed to find solution for " << i << ": "
                << machines[i].num_buttons - machines[i].num_displays<< "\n";
            }
            else{
                print(solution);
                button_presses = min_presses;
            }
        }
        else{
            solution = solve_matrix(machines[i].matrix, machines[i].target, machines[i].matrix.size());
            for (long long s : solution){
                button_presses += s;
            }
        }
        cout << "Buttons pressed = " << button_presses << "\n";
        total += button_presses;
    }
}

int main() {
    string file = "../input/day10.txt";
    analyse(file);
    return 0;
}