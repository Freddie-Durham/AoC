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
            buttons = newM;

            //if there are more displays than buttons, pad the matrix with buttons that do nothing
            while (newM.size() < num_displays){ 
                newM.push_back(vector<float>(num_displays, 0.0));
            }

            //if there are more buttons than displays, invent new displays
            //these displays have a target of the number of times an excess button has been pressed (which we don't know)
            //these displays are incremented whenever button "num_displays + n" is pressed
            while (newM[0].size() < num_buttons){ 
                target.push_back(0.0);
                
                for (int i = 0; i < newM.size(); i++){
                    //display how many times button "num_displays + n" has been pressed
                    if ((i >= num_displays) && (newM[i].size() == i)){
                        newM[i].push_back(1.0);
                    }
                    else{
                        newM[i].push_back(0.0);
                    }
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

long long next_highest(const vector<float>& target){
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

long long sum(const vector<float>& target){
    long long val = 0;
    for (int i = 0; i < target.size(); i++){
        val += target[i];
    }
    return val;
}

//assume matrix is square
vector<float> mat_mul(const vector<vector<float>> &matrix, const vector<float> &vec){
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

int max_presses(const vector<float> &button, const vector<float> &target){
   int max_press = 10000;
    for (int i = 0; i < target.size(); i++){
        if (button[i] > 0){
            max_press = min(max_press, int(target[i]));
        }
    }
    return max_press;
}

vector<float> handle_excess_buttons(const Machine &machine, vector<float> cur_target){
    vector<float> solution;
    bool success = false;

    while (!success){
        for (long long j = machine.num_displays; j < machine.num_buttons; j++){
            cur_target[j] = max_presses(machine.buttons[j], machine.target);
        }
        solution = solve_matrix(machine.matrix, cur_target, machine.matrix.size());
        //print(solution);

        success = true;
        for (int j = 0; j < machine.num_buttons; j++){
            if ((solution[j] < 0) || (ceilf(solution[j]) != solution[j])){
                success = false;
            }
        }
        break;
    }

    print(solution);
    bool is_correct = (cur_target == mat_mul(machine.matrix, solution));
    if (!is_correct){
        cout << "Doesn't solve matrix" << "\n";
    }
    return solution;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;
    vector<float> solution;

    for (int i = 0; i < 1/*machines.size()*/; i++){
        long long button_presses = 0;
        print_machine(machines[i]);

        if (machines[i].num_displays < machines[i].num_buttons){
            solution = handle_excess_buttons(machines[i], machines[i].target);
        }
        else{
            solution = solve_matrix(machines[i].matrix, machines[i].target, machines[i].matrix.size());
        }
        
        for (long long s : solution){
            button_presses += s;
        }
        cout << "Buttons pressed = " << button_presses << "\n";
        total += button_presses;
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}