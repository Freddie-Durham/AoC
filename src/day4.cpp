#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<cmath>
using namespace std;

//return padded room
vector<vector<bool>> get_array(string file){
    string line;
    string sequence;
    ifstream data(file);
    vector<vector<bool>> room;
    bool first_row = true;
    int row_length = 0;
    vector<bool> empty_row(row_length);

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                if (first_row){
                    row_length = sequence.length()+2;
                    empty_row.resize(row_length);
                    room.push_back(empty_row);
                    first_row = false;
                }
                
                vector<bool> row(row_length); 
                for (int i = 0; i < row_length-2; i++){
                    row[i+1] = (sequence[i] == '@');
                } 
                room.push_back(row);
    }}

    room.push_back(empty_row);
    data.close();
    }    
    else 
    cout << "Unable to open data file";
    return room;
}

bool accessible(int pos_x, int pos_y, vector<vector<bool>> arr){
    int count = 0;
    for (int i=pos_x-1; i<=pos_x+1; i++){
        for (int j=pos_y-1; j<=pos_y+1; j++){
            if (arr[i][j]){
                count += 1;
            }
            if (count > 4){
                return false;
            }
        }
    }
    return true;
}

void analyse(string file){
    long long score = 0;
    vector<vector<bool>> room = get_array(file);

    int width = room[0].size();
    int height = room.size();
    long long prev_score = 1;

    while (prev_score != score){
        prev_score = score;
        for (int i = 1; i < width - 1; i++){
        for (int j = 1; j < height - 1; j++){
            if (room[i][j]){
                if (accessible(i,j,room)){
                    score += 1;
                    room[i][j] = false;
    }}}}
    cout << "Current score = " << score << "\n";
    }
   
    cout << "Score = " << score;
}

int main() {
    string file = "../input/day4.txt";

    analyse(file);
    return 0;
}