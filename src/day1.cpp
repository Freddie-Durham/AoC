#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string line;
    int pos = 50;
    int total = 0;
    ifstream data("../input/day1.txt");
    if (data.is_open()){
        while (getline (data,line)){
            int shift = stoi(line.substr(1,size(line)-1));
            int step = 1;
            if (line[0]=='L'){
                step = -1;
            }
            for (int i=0; i < shift; i++){
                pos += step;
                if (pos%100 == 0){
                    total += 1;
                };
            }  
            //cout << pos << " Total = " << total << "\n";
        }
        data.close();
    }else 
    cout << "Unable to open data file";

    cout << "Total = " << total;
    return 0;
}

//1226 is too high