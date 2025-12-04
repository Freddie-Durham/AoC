#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
    string line;
    string sequence;
    ifstream data("../input/day2_test.txt");
    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,',')){
                stringstream seq(sequence);
                vector<int> start_stop = {1,1};
                string str;
                int i = 0;
                while (getline(seq,str,'-')){
                    start_stop[i] = stoi(str);
                    i++;
                }
                for (int v : start_stop){
                    cout << v << "\n";
                }
    }}
        data.close();
    }    
    else 
    cout << "Unable to open data file";
    return 0;
}