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
        vector<vector<float>> buttons;
        vector<int> state;
        int counter_num;
        int button_num;

        Machine(string str){
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    counter_num = sec.size() - 2;
                }
                else if (sec[0] == '('){
                    vector<float> initial(counter_num, 0.0);
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        initial[(stoi(num))] = 1.0;
                    }
                    buttons.push_back(initial);
                }
                else if (sec[0] == '{'){
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    for (string num : substr){
                        target.push_back(stof(num));
                    }
                }
            }
            button_num = buttons.size();
            vector<int> initial(counter_num, 0.0);
            state = initial;
        }
};

void print_machine(Machine m){
    cout << "Target:" << "\n";
    print(m.target);
    cout << "Buttons:" << "\n";
    print(m.buttons);
    cout << "Current state:" << "\n";
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

void push_button(Machine &m, vector<float> button, int n){
    //loop through joltage counters, push button n times
    for (int i; i < m.state.size(); i++){
        if (button[i] > 0){
            m.state[i] += n;
        }
    }
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;

    for (int i = 0; i < machines.size(); i++){
        print_machine(machines[i]);
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}

//110197 is too high