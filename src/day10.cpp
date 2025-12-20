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

const int FAIL = 1000;

class Machine{
    public:
        uint32_t state;
        uint32_t target;
        vector<uint32_t> switches;
        int len;
        int flips;

        Machine(string str){
            state = 0;
            target = 0;
            flips = 0;
            len = 0;
            
            vector<uint32_t> switch_vec;
            for (string sec : split(str, ' ')){
                if (sec[0] == '['){
                    len = sec.size() - 2;
                    for (int i = len; i > 0; i--){
                        target |= ((sec[i] == '#') << (len - i));
                    }
                }
                else if (sec[0] == '('){
                    uint32_t initial = 0;
                    vector<string> substr = split(sec.substr(1, sec.size() - 2), ',');
                    uint32_t first_light = 1 << (len - 1);
                    for (string num : substr){
                        initial |= (first_light >> (stoi(num)));
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
    const int l = 10;
    bitset<l> t(m.target);
    bitset<l> s(m.state);
    cout << "Current = " << s << ", target = " << t << ". Switches: " << "\n";

    for (uint32_t sw : m.switches){
        bitset<l> b(sw);
        cout << b << " ";
    }
    cout << "\n";
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

int test_switches(Machine& machine, int index, int best){
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

//return number of flips if successful
int test_flip(Machine machine, int i, int len){
    for (int j = 0; j < len; j++){
        if ((i & (1 << j)) != 0){ //test if jth bit is set
            machine.flip_switch(j);
        }
    }
    if (machine.state == machine.target){
        return machine.flips;
    }
    else{
        return FAIL;
    }
}

int linear_switches(Machine machine){
    int len = machine.switches.size();
    int count = (1 << len);
    int best = FAIL;

    for (int i = 0; i < count; i++){
        int score = test_flip(machine, i, len);
        best = min(best, score);
    }
    return best;
}

void analyse(string file){
    vector<Machine> machines = get_machines(file);
    long long total = 0;
    for (Machine machine : machines){
        print_machine(machine);
        int score = linear_switches(machine);
        //int score = test_switches(machine, 0, FAIL);
        //cout << "Best score = " << score << "\n";
        total += score;
    }
    cout << "Score = " << total << "\n";
}

int main() {
    string file = "../input/day10.txt";
    analyse(file);
    return 0;
}

//110197 is too high