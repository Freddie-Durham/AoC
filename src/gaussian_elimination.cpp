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

const float FAIL = 10000;

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

            //if there are more buttons than displays, delete buttons from matrix
            //these excess buttons must be exhaustively searched to find optimal solution
            while (newM.size() > num_displays){ 
                newM.pop_back();
            }
            matrix = transpose_matrix(newM, newM.size(), newM[0].size());
        }
};

void print_machine(Machine m){
    cout << "Buttons = " << m.num_buttons << ", displays = " << m.num_displays << ". Target: ";
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

bool test_solution(const Machine &machine, const vector<float> &target, const vector<float> &solution){
    bool success = true;
    for (int j = 0; j < machine.num_displays; j++){
        if ((solution[j] < 0) || (ceilf(solution[j]) != solution[j])){
            success = false;
        }
    }

    success &= (target == mat_mul(machine.matrix, solution));
    return success;
}

void add_button(const vector<float> &button, vector<float> &target, int sign){
    for (int i = 0; i < target.size(); i++){
        target[i] += sign * button[i];
    }
}

bool all_positive(const vector<float> &target){
    bool all = true;
    for (const float &t : target){
        if (t < 0.0){
            all = false;
            break;
        }
    }
    return all;
}

float score_solution(const vector<float> &solution){
    float button_presses = 0;
    for (const float s : solution){
        button_presses += s;
    }
    return button_presses;
}

float recursive_search(const Machine &machine, 
    const vector<vector<float>> &buttons, vector<float> &target, float best){
    if (!all_positive(target)){
        return best;
    }
    else {
        vector<float> solution = solve_matrix(machine.matrix, target, machine.matrix.size());
        if (test_solution(machine, target, solution)){
            float score = score_solution(solution);
            best = min(best, score);
        }
    }

    float score = 0;
    for (const vector<float> &button : buttons){
        add_button(button, target, +1);
        score = recursive_search(machine, buttons,target, best);
        add_button(button, target, -1);
        best = min(best, score);
    }
    return best;
}

float handle_excess_buttons(const Machine &machine){
    vector<vector<float>> spare_buttons;
    for (int j = machine.num_displays; j < machine.num_buttons; j++){
        spare_buttons.push_back(machine.buttons[j]);
    }
    vector<float> new_target = machine.target;
    float best = recursive_search(machine, spare_buttons, new_target, best);

    cout << "Best score = " << best << "\n";
    return best;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    float total = 0;
    vector<float> solution;

    for (int i = 0; i < machines.size(); i++){
        if (machines[i].num_displays < machines[i].num_buttons){
            //total += handle_excess_buttons(machines[i]);
        }
        else{
            solution = solve_matrix(machines[i].matrix, machines[i].target, machines[i].matrix.size());
            if (!test_solution(machines[i], machines[i].target, solution)){
                cout << "Failed to find solution for i = " << i << "\n";
            }
            float button_presses = score_solution(solution);
            cout << "Buttons pressed = " << button_presses << "\n";
            total += button_presses;
        }
    }
}

int main(){
    string file = "../input/day10.txt";
    analyse(file);
    return 0;
}