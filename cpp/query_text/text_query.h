#ifndef __TextQuery_H__
#define __TextQuery_H__

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using std::shared_ptr;
using std::ostream;
using std::string;
using std::vector;
using std::set;
using std::map;

class QueryResult;

// 1.class TextQuery
class TextQuery{
public:
    // 构造
    TextQuery(int capacity);
    // 成员函数
    // 1.处理文件
    void readFile(const string & filename);
    // 2.查询单词
    QueryResult query(const string & word);
private:
    // 辅助函数
    void handleWord(string & word);
    void insertWord(const string & word,size_t line);
private:
    shared_ptr<vector<string>> _pfile;
    map<string,shared_ptr<set<size_t>>> _wordNum;
    map<string,size_t> _count;
};

// 2.class QueryResult
class QueryResult{
public:
    QueryResult(const string & sought,size_t countNum,
                shared_ptr<set<size_t>> plines,
                shared_ptr<vector<string>> pfile);
    size_t getCount() const{
        return _countNum;
    }
    auto begin() const{
        return _plines->begin();
    }
    auto end() const{
        return _plines->end();
    }
    auto get_file() const{
        return _pfile;
    }
    friend ostream & operator<<(ostream & os,const QueryResult & rhs);
private:
    string _sought;
    size_t _countNum;
    shared_ptr<set<size_t>> _plines;
    shared_ptr<vector<string>> _pfile;
};

#endif
