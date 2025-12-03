#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string line;
    ifstream data("input/day1_test.txt");
    if (data.is_open()){
        while (getline (data,line)){
            cout << line << "\n";
        }
        data.close();
    }
    else cout << "Unable to open data file";

    return 0;
}