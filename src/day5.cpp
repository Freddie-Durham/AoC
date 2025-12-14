#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cmath>
using namespace std;

bool is_spoiled(vector<vector<long long>> limits, long long item){
    for (vector<long long> lims : limits){
        if (item >= lims[0] && item <= lims[1]){
            return false;
        }
    }
    return true;
}

void analyse_limits(vector<vector<long long>> limits){
    vector<vector<long long>> new_limits;
    int new_ind = 0;
    int vec_len = limits.size();
    int i = 0;

    while (i < vec_len){
        long long upper = limits[i][1];
        long long lower = limits[i][0];
        for (int j = 0; j < vec_len; j++){
            if (j != i){
                //If two limits are unrelated, add second to new lims
                if ((limits[j][0] > upper) || (lower > limits[j][1])){
                    new_limits.push_back(limits[j]);
                }
                //If two limits intersect, absorb one into the other
                else {
                    upper = max(upper, limits[j][1]);
                    lower = min(lower, limits[j][0]);
                }
            }
        }
        vector<long long> final{lower, upper};
        new_limits.push_back(final); 

        if (new_limits.size() < vec_len){
            limits = new_limits;
            vec_len = limits.size();
            i = 0;
        }
        else {
            i += 1;
        }
        new_limits.resize(0);
    }

    long long score = 0;
    for (vector<long long> l : limits){
        score += l[1] - l[0] + 1;
        cout << "min = " << l[0] << " max = " << l[1] << "\n";
    }
    cout << "Final score = " << score << "\n";
}

void analyse(string file){
    string line;
    string sequence;
    ifstream data(file);
    long long score = 0;
    vector<vector<long long>> limits;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                if (sequence.find('-') != string::npos){
                    stringstream seq(sequence);
                    vector<long long> start_stop = {1,1};
                    string str;
                    int i = 0;
                    while (getline(seq,str,'-')){
                        start_stop[i] = stoll(str);
                        i++;
                    }
                    limits.push_back(start_stop);
                }   
                else{
                    long long ingredient = stoll(sequence);
                    if (!is_spoiled(limits,ingredient)){
                        score += 1;
                    }
                }
    }}
    data.close();
    analyse_limits(limits);
    //cout << "Score = " << score << "\n";
    }    
    else 
    cout << "Unable to open data file";
}

int main() {
    string file = "../input/day5.txt";

    analyse(file);
    return 0;
}