#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cmath>
using namespace std;

vector<string> split(string str, char delim){
    vector<string> strings;
    string cur_string = "";

    for (int i = 0; i < str.length(); i++){
        if (str[i] == delim){
            if (cur_string != ""){
                strings.push_back(cur_string);
                cur_string = "";
            }   
        }
        else{
            cur_string.append(str,i,1);
        }
    }
    if (cur_string != ""){
        strings.push_back(cur_string);
    }   
    return strings;
}

void analyse(string file){
    string line;
    string sequence;
    ifstream data(file);
    long long score = 0;
    vector<vector<long long>> numbers;
    vector<bool> is_plus;

    if (data.is_open()){
        int linenum = 0;
        while(getline(data,line)){
            linenum += 1;
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                if (linenum < 5){
                    vector<long long> num_list;
                    for (string number : split(sequence, ' ')){
                        num_list.push_back(stoll(number));
                    }
                    numbers.push_back(num_list);
                }
                else{
                    for (string op : split(sequence, ' ')){
                        if (op == "+"){
                            is_plus.push_back(true);
                        }
                        else{
                            is_plus.push_back(false);
                        }
                    }
                }
            }
        }
    
    for (int i = 0; i < is_plus.size(); i++){
        if (is_plus[i]){
            score +=  numbers[0][i] + numbers[1][i] + numbers[2][i]
            + numbers[3][i];
        }
        else{
            score +=  numbers[0][i] * numbers[1][i] * numbers[2][i]
            * numbers[3][i];
        }
    }
    
    data.close();
    cout << "Score = " << score << "\n";
    }    
    else 
    cout << "Unable to open data file";
}

int main() {
    string file = "../input/day6.txt";

    analyse(file);
    return 0;
}

//17323787913 too low