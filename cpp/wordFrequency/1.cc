#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
using namespace std;

struct Record{
    Record(string & word, int frequency):
        _word(word),_frequency(frequency){}
    // 按照Record的_word排序
    bool operator<(Record &rhs){
        return _word < rhs._word;
    }
    string _word;
    int _frequency;
};

class DictVector{
public:
    DictVector(int capacity)
    {
        _dict.reserve(capacity);
    }

    void read(const string & filename){
        ifstream ifs(filename);
        if(!ifs){
            cout << "ifs open file failed!" << endl;
            return;
        }
        string line;
        while(getline(ifs,line)){
            istringstream iss(line);
            string word;
            while(iss >> word){
                // 处理单词
                handleWord(word);
                // 插入
                insertWord(word);
            }
        }
        // 将元素进行排序
        sort(_dict.begin(),_dict.end());
        ifs.close();
    }
    void store(const string & filename){
        ofstream ofs(filename);
        if(!ofs){
            cout << "ofs open file failed!" << endl;
        }
        for(auto & record:_dict){
            ofs << record._word << "\t" 
                << record._frequency << endl;
        }
        ofs.close();
    }
    void print(){
        for(auto it = _dict.begin();
            it != _dict.end(); ++it){
            cout << it->_word << "\t" 
                << it->_frequency << endl;
        }
    }
private:
    void handleWord(string & word){
        for(auto & ch:word){
            if(!isalpha(ch)){
                word = string();
                return;
            }else if(isupper(ch)){
                ch = tolower(ch);
            }
        }
    }
    void insertWord(string & word){
        if(word == string()){
            return;
        }
        auto it = _dict.begin();
        for(; it != _dict.end(); ++it){
            if(it->_word == word){
                ++it->_frequency;
                break;
            }
        }
        if(it == _dict.end()){
            _dict.push_back(Record(word,1));
        }
    }
    vector<Record> _dict;
};

int main()
{
    DictVector dict(10000);
    time_t begTime = time(NULL);
    cout << "1:begin read!" << endl;
    dict.read("The_Holy_Bible.txt");
    cout << "1:over read!" << endl;
    //dict.print();
    cout << "1:begin store!" << endl;
    dict.store("myDict1.txt");
    cout << "1:over store!" << endl;
    time_t endTime = time(NULL);
    cout << "1:time : " << endTime-begTime << "s" << endl;
    return 0;
}

