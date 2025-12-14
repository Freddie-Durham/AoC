#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

bool is_valid(long long v){
    string str = to_string(v);
    int len = str.length();

    if (len % 2 != 0){
        return true;
    }

    if (str.substr(0,len/2) == str.substr(len/2)){
        return false;
    }
    return true;
}

bool is_still_valid(long long v){
    string str = to_string(v);
    int len = str.length();
    int max_len = len / 2;

    for (int sub_len = max_len; sub_len >= 1; sub_len--){
        if (len % sub_len == 0){
            string target = str.substr(0,sub_len);
            //cout << "Current target = " << target << "\n";
            bool valid = false; //assume invalid
            for (int pos = sub_len; pos < len; pos+=sub_len){
                //cout << "Current comparison = " << str.substr(pos,sub_len) << "\n";
                if (str.substr(pos,sub_len) != target){
                    valid = true;
                    break;
                }
            }
            if (valid==false){
                return false;
            }
    }}
    return true;
}

void analyse(string file){
    string line;
    string sequence;
    ifstream data(file);
    long long score = 0;
    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,',')){
                stringstream seq(sequence);
                vector<long long> start_stop = {1,1};
                string str;
                int i = 0;
                while (getline(seq,str,'-')){
                    start_stop[i] = stoll(str);
                    i++;
                }
                for (long long v = start_stop[0]; v <= start_stop[1]; v++){
                    if (!is_still_valid(v)){
                        score += v;
                        cout << "Invalid ID: " << v << "\n";
                    }
                }
    }}
        data.close();
    cout << "Score = " << score << "\n";
    }    
    else 
    cout << "Unable to open data file";
}

int main() {
    string file = "../input/day2.txt";

    analyse(file);
    return 0;
}