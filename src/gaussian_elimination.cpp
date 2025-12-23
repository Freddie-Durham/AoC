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
        int columns;
        int rows;

        Machine(string str){
            vector<vector<float>> newM;
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    columns = sec.size() - 2; //number of displays
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
            rows = newM.size(); //number of buttons

            //if there are more displays than buttons, pad the matrix with buttons that do nothing
            while (newM.size() < columns){ 
                newM.push_back(vector<float>(columns, 0.0));
            }

            //if there are more buttons than displays, invent new displays that have a target of zero
            while (newM[0].size() < rows){ 
                target.push_back(0.0);
                for (vector<float> &vec : newM){
                    vec.push_back(0.0);
                }
            }

            vector<int> pos(rows, 0.0);
            for (int i = 0; i < rows; i++){
                pos[i] = i;
            }
            matrix = transpose_matrix(newM, newM.size(), newM[0].size());
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
    }
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;

    for (int i = 0; i < 1; i++){
        solve_matrix(machines[i].matrix, machines[i].target, machines[i].matrix.size());
        print_machine(machines[i]);
        // 1 3 3 0 1 2

        long long solution = 0;
        for (long long t : machines[i].target){
            solution += t;
        }
        cout << "Buttons pressed = " << solution << "\n";
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}

//110197 is too high