#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

const long long START = -1;
const long long SPLIT = -2;
const long long EMPTY = 0;

void print(vector<int> vec){
    for (int v : vec){
        cout << max(0,v);
    }
    cout << "\n";
}

void print(vector<vector<int>> vec){
    for (vector<int> v : vec){
        print(v);
    }
}

//return tachyon room
vector<vector<long long>> get_array(string file){
    string line;
    string sequence;
    ifstream data(file);
    vector<vector<long long>> room;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                vector<long long> row; 
                for (int i = 0; i < sequence.length(); i++){
                    if (sequence[i] == 'S'){
                        row.push_back(START);
                    }
                    else if (sequence[i] == '^'){
                        row.push_back(SPLIT);
                    }
                    else{
                        row.push_back(EMPTY);
                    }
                } 
                room.push_back(row);
    }}
    data.close();
    }    
    else{
    cout << "Unable to open data file";
    }
    return room;
}

void analyse(string file){
    vector<vector<long long>> room = get_array(file);

    int width = room[0].size();
    int height = room.size();

    for (int j = 0; j < height - 1; j++){
        for (int i = 0; i < width; i++){
            if (room[j][i] == START){
                room[j+1][i] = 1;
            }
            else if (room[j][i] > 0){
                if (room[j+1][i] == SPLIT){
                    room[j+1][i-1] += room[j][i];
                    room[j+1][i+1] += room[j][i];
                }
                else if (room[j+1][i] >= EMPTY){
                    room[j+1][i] += room[j][i];
                }
            }
        }
    }

    //print(room);

    long long score = 0;
    for (int i = 0; i < width; i++){
        score += room[height-1][i];
    }
    cout << "Score = " << score;
}

int main() {
    string file = "../input/day7.txt";

    analyse(file);
    return 0;
}