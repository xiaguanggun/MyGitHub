#include "query.h"
#include <algorithm>

using namespace std;

// 0.0 Query
/* ostream & operator<<(ostream & os, const Query & q){ */
/*     return os << q.req(); */
/* } */
Query::Query(const string & sought):_qb(new WordQuery(sought)){}
QueryResult Query::eval(TextQuery & tq) const{
    return _qb->eval(tq);
}
std::string Query::req() const{
    return _qb->req();
}

// 1.1 class WordQuery
WordQuery::WordQuery(const string & sought):_sought(sought){}
QueryResult WordQuery::eval(TextQuery & tq) const {
    return tq.query(_sought);
}
string WordQuery::req() const{
    return _sought;
}
// 1.2 class NotQuery
Query operator~(const Query & q){
    return shared_ptr<Query_base>(new NotQuery(q));
}
NotQuery::NotQuery(const Query & q):_q(q){}
string NotQuery::req() const{
    return "~" + _q.req();
}
QueryResult NotQuery::eval(TextQuery & tq) const{
    // 获取WordQuery的查询结果
    QueryResult ret(_q.eval(tq));
    // 构造行号set
    shared_ptr<set<size_t>> ret_lineSet(new set<size_t>());
    size_t lineSize = ret.get_file()->size();
    auto it = ret.begin();
    for(size_t i = 0; i < lineSize && it != ret.end(); ++i){
        if(i < *it){
            ret_lineSet->insert(i);
        }else{
            ++it;
        }
    }
    // 返回QueryResult
    return QueryResult(req(),lineSize-ret.getCount(),ret_lineSet,ret.get_file());
}


// 1.3 class BinaryQuery
BinaryQuery::BinaryQuery(const Query & lhs, const Query & rhs, const string & op):
    _lhs(lhs),_rhs(rhs),_opSym(op)
{}
string BinaryQuery::req() const{
    return "(" + _lhs.req() + " " + _opSym + " " +  _rhs.req() + ")";
}

// 2.1 class AndQuery
Query operator&(const Query & lhs,const Query & rhs){
    return shared_ptr<Query_base>(new AndQuery(lhs,rhs));
}
AndQuery::AndQuery(const Query & lhs,const Query & rhs):
    BinaryQuery(lhs, rhs, "&")
{}
QueryResult AndQuery::eval(TextQuery & tq) const{
    // 获取左右WordQuery查询结果
    QueryResult left(_lhs.eval(tq));
    QueryResult right(_rhs.eval(tq));
    // 构造行号set
    shared_ptr<set<size_t>> ret_lineSet(new set<size_t>());
    // 这里使用inserter函数,底层使用insert_iterator,底层使用insert,无需考虑扩容的事情
    set_intersection(left.begin(),left.end(),
                     right.begin(),right.end(),
                     inserter(*ret_lineSet,ret_lineSet->begin()));
    // 返回QueryResult
    return QueryResult(req(),ret_lineSet->size(),ret_lineSet,left.get_file());
}

// 2.2 class OrQuery
Query operator|(const Query & lhs,const Query & rhs){
    return shared_ptr<Query_base>(new OrQuery(lhs,rhs));
}
OrQuery::OrQuery(const Query & lhs,const Query & rhs):
    BinaryQuery(lhs, rhs, "|")
{}
QueryResult OrQuery::eval(TextQuery & tq) const{
    // 获取左右WordQuery查询结果
    QueryResult left(_lhs.eval(tq));
    QueryResult right(_rhs.eval(tq));
    // 构造行号set
    shared_ptr<set<size_t>> ret_lineSet = make_shared<set<size_t>>(left.begin(),left.end());
    ret_lineSet->insert(right.begin(),right.end());
    // 返回QueryResult
    return QueryResult(req(),left.getCount()+right.getCount(),ret_lineSet,left.get_file());
}

