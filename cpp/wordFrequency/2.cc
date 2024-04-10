#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

class DictMap{
public:
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
        ifs.close();
    }
    void store(const string & filename){
        ofstream ofs(filename);
        if(!ofs){
            cout << "ofs open file failed!" << endl;
        }
        for(auto & wordPair:_dict){
            ofs << wordPair.first << "\t" 
                << wordPair.second << endl;
        }
        ofs.close();
    }
    void print(){
        for(auto & wordPair:_dict){
            cout << wordPair.first << "\t" 
                << wordPair.second << endl;
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
        ++_dict[word];
    }
    map<string,int> _dict;
};

int main()
{
    DictMap dict;
    time_t begTime = time(NULL);
    cout << "2:begin read!" << endl;
    dict.read("The_Holy_Bible.txt");
    cout << "2:over read!" << endl;
    //dict.print();
    cout << "2:begin store!" << endl;
    dict.store("myDict2.txt");
    cout << "2:over store!" << endl;
    time_t endTime = time(NULL);
    cout << "2:time : " << endTime-begTime << "s" << endl;
    return 0;
}

