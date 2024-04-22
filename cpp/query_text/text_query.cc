#include "text_query.h"
#include <fstream>
#include <sstream>
using namespace std;


// 1.class TextQuery
// 构造
TextQuery::TextQuery(int capacity):
    _pfile(new vector<string>)
{
    _pfile->reserve(capacity);
}
// 成员函数
void TextQuery::readFile(const string &filename){
    // 打开文件
    ifstream ifs(filename);
    if(!ifs){
        cout << "文件打开失败!" << endl;
        return;
    }
    string lineStr;
    string word;
    size_t line = 0;
    while(getline(ifs,lineStr)){
        // 先存_line
        _pfile->push_back(lineStr);
        // 统计所有line的单词并存储在_wordNum和_count
        istringstream iss(lineStr);
        while(iss >> word){
            // 处理单词
            handleWord(word);
            // 插入
            insertWord(word, line);
        }
        ++line;
    }
    ifs.close();
}
QueryResult TextQuery::query(const string & word){
    auto ret = _wordNum.find(word);
    if(ret == _wordNum.end()){
        // 未找到
        static shared_ptr<set<size_t>> nodata(new set<size_t>);
        return QueryResult(word,0,nodata,_pfile);
    }
    // 找到
    return QueryResult(word,_count[word],ret->second,_pfile);
}

// 辅助函数
void TextQuery::handleWord(string & word){
    for(auto & ch:word){
        if(!isalpha(ch)){
            word = string();
            return;
        }else if(isupper(ch)){
            ch = tolower(ch);
        }
    }
}
void TextQuery::insertWord(const string & word,size_t line){
    if(word == string()){
        return;
    }
    // _count
    ++_count[word];
    // _wordNum
    auto & plines = _wordNum[word];
    if(!plines){
        plines.reset(new set<size_t>);
    }
    plines->insert(line);
}


// 2.class QueryResult
QueryResult::QueryResult(const string & sought,size_t countNum,
                         shared_ptr<set<size_t>> plines,
                         shared_ptr<vector<string>> pfile):
    _sought(sought),_countNum(countNum),_plines(plines),_pfile(pfile)
{}
ostream & operator<<(ostream & os,const QueryResult & rhs){
    if(rhs._countNum == 0){
        os << "文件中不存在!" << endl;
        return os;
    }
    os << "--------------------" << endl;
    os << rhs._sought << "出现了" << rhs._countNum << "次!" << endl;
    for(auto num:*rhs._plines){
        os << "(line " << num+1 << ") " << (*rhs._pfile)[num] << endl;
    }
    os << "--------------------" << endl;
    return os;
}

