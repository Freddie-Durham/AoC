#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

template<typename T> struct is_vector : std::false_type {};
template<typename T> struct is_vector<std::vector<T>> : std::true_type {};

template <typename T>
void print(const T& val) {
    if constexpr (is_vector<T>::value) {
        for (const auto& item : val) {
            print(item);
        }
        std::cout << "\n";
    } 
    else {
        std::cout << val << " ";
    }
}

vector<string> split(const string& str, char delim){
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

vector<vector<long long>> get_array(string file, char delim){
    string line;
    string sequence;
    ifstream data(file);
    vector<vector<long long>> array;

    if (data.is_open()){
        while(getline(data,line)){
            stringstream stream(line);
            while (getline(stream,sequence,'\n')){
                vector<long long> row;
                for (string num : split(sequence, delim)){
                    row.push_back(stoll(num));
                }
                array.push_back(row);
    }}
    data.close();
    }    
    else{
    cout << "Unable to open data file";
    }
    return array;
}