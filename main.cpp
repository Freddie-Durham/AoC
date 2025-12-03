#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string line;
    int pos = 0;
    int total = 0;
    ifstream data("input/day1.txt");
    if (data.is_open()){
        while (getline (data,line)){
            int shift = stoi(line.substr(1,size(line)-1));
            if (line[0]=='L'){
                pos -= shift;
            }else
                pos += shift;
            pos %= 50;
            if (pos == 0){
                total += 1;
            };
        }
        data.close();
    }else 
    cout << "Unable to open data file";

    cout << "Total = " << total;
    return 0;
}

//1226 is too high