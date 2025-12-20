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
using namespace std;

const int FAIL = 1000;

class Machine{
    public:
        uint32_t state;
        uint32_t target;
        vector<uint32_t> switches;
        int flips;

        Machine(string str){
            state = 0;
            target = 0;
            flips = 0;
            vector<uint32_t> switch_vec;
            for (string sec : split(str, ' ')){
                int len = sec.size();
                if (sec[0] == '['){
                    for (int i = len - 2; i > 0; i--){
                        target |= ((sec[i] == '#') << (len - 2 - i));
                    }
                }
                else if (sec[0] == '('){
                    uint32_t initial = 0;
                    for (string num : split(sec.substr(1, len - 2), ',')){
                        initial |= (1 << stoi(num));
                    }
                    switch_vec.push_back(initial);
                }
            }
            switches = switch_vec;
        }

        void flip_switch(int i){
            state ^= switches[i];
            flips += 1;
        }
        void unflip_switch(int i){
            state ^= switches[i];
            flips -= 1;
        }
};

void print_machine(Machine m){
    cout << "Current = " << m.state << ", target = " << m.target << "\n";
    print(m.switches);
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

int test_switches(Machine machine, int index, int best){
    if (machine.target == machine.state){
        return machine.flips;
    }
    if (index >= machine.switches.size()){
        return FAIL;
    }
    //flip
    machine.flip_switch(index);
    int score = test_switches(machine, index + 1, best);
    best = min(best, score);
    machine.unflip_switch(index);
    //no flip
    score = test_switches(machine, index + 1, best);
    best = min(best, score);
    return best;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);

    for (Machine machine : machines){
        print_machine(machine);
        //int score = test_switches(machine, 0, FAIL);
        //cout << "Best score = " << score << "\n";
    }
}

int main() {
    string file = "../input/day10_test.txt";
    analyse(file);
    return 0;
}