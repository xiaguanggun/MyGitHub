#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class TextQuery{
public:
    TextQuery(int capacity){
        _line.reserve(capacity);
    }
    void readFile(const string &filename){
        // 打开文件
        ifstream ifs(filename);
        if(!ifs){
            cout << "文件打开失败!" << endl;
            return;
        }
        string line;
        int lines = 0;
        while(getline(ifs,line)){
            // 先存_line
            _line.push_back(line);
            // 统计所有line的单词并存储在_wordNum和_count
            istringstream iss(line);
            string words;
            while(iss >> words){
                // 处理单词
                handleWord(words);
                // 插入
                insertWord(words, lines);
            }
            ++lines;
        }
        ifs.close();
    }
    void query(const string &word){
        if(_count.count(word) == 0){
            cout << "文件中不存在此单词!" << endl;
            return;
        }
        cout << "--------------------" << endl;
        cout << word << "出现了" << _count[word] << "次!" << endl;
        for(auto &w:_wordNum[word]){
            cout << w+1 << " " << _line[w] << endl;
        }
        cout << "--------------------" << endl;
    }
private:
    void handleWord(string & words){
        for(auto & ch:words){
            if(!isalpha(ch)){
                words = string();
                return;
            }else if(isupper(ch)){
                ch = tolower(ch);
            }
        }
    }
    void insertWord(const string & words,int lines){
        if(words == string()){
            return;
        }
        // _count
        ++_count[words];
        // _wordNum
        _wordNum[words].insert(lines);
    }
    vector<string> _line;
    map<string,set<int>> _wordNum;
    map<string,int> _count;
};

int main()
{
    // 文件名
    cout << "请输入文件名:" << endl;
    string filename;
    cin >> filename;
    // 遍历文件
    TextQuery text(107);
    text.readFile(filename);
    // 查询
    string word;
    while(1){
        cout << "请输入待查询单词:" << endl;
        cin >> word;
        if(cin.eof()){
            break;
        }
        text.query(word);
    }
    return 0;
}

