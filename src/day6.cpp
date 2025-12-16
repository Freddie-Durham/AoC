#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cmath>
using namespace std;

bool contains(string str, string substr){
    return str.find(substr) != string::npos;
}

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
    vector<string> numbers;
    vector<bool> is_plus;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                if (!contains(sequence,"*")){
                    numbers.push_back(sequence);
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

    int op_pos = 0;
    long long cur_calc = 0;
    for (int i = 0; i < numbers[0].length(); i++){
        string num_str = "";
       for (int j = 0; j < numbers.size(); j++){
            if (numbers[j][i] != ' '){
                num_str.push_back(numbers[j][i]);
            }   
       }

       if (num_str == ""){
            op_pos += 1; 
            score += cur_calc;
            cur_calc = 0;
       }
       else{
            long long num = stoll(num_str);
            if (is_plus[op_pos]){
                cur_calc += num;
            }
            else{
                if (cur_calc == 0){
                    cur_calc = 1;
                } 
                cur_calc *= num;
            }            
       }
    }
    score += cur_calc;

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