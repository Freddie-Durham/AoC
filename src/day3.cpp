#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cmath>
using namespace std;

void analyse(string file){
    string line;
    string sequence;
    ifstream data(file);
    long long score = 0;
    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                int len = sequence.length();
                long long jolts = 0;
                int max_ind = 0;

                for (int k = 11; k >= 0; k--){
                    int max_val = 0;
                    for (int i = max_ind; i < len-k; i++){
                    int val = sequence[i] - '0';
                    if (val > max_val){
                        max_val = val;
                        max_ind = i;
                    }}   
                    jolts += max_val * pow(10,k);
                    max_ind += 1;
                }

                cout << fixed << "Max jolts = " << jolts << "\n"; 
                score += jolts;   
    }}
    data.close();
    cout << "Score = " << score << "\n";
    }    
    else 
    cout << "Unable to open data file";
}

int main() {
    string file = "../input/day3.txt";

    analyse(file);
    return 0;
}