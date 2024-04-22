#ifndef __Query_H__
#define __Query_H__

#include "text_query.h"

class Query_base;

// 0.0 class Query
class Query{
    friend Query operator~(const Query & q);
    friend Query operator|(const Query & lhs,const Query & rhs);
    friend Query operator&(const Query & lhs,const Query & rhs);
    /* friend ostream & operator<<(ostream & os, const Query & q); */
public:
    Query(const string & sought);
    // 返回查询语句
    std::string req() const;
    // 返回查询结果
    QueryResult eval(TextQuery & tq) const;
private:
    // 根据Query_base创建Query
    Query(shared_ptr<Query_base> qb):_qb(qb){}
    // 私有成员
    shared_ptr<Query_base> _qb;
};


// 0.1 class Query_base
class Query_base{
    friend class Query;
protected:
    virtual ~Query_base() = default;
private:
    // 返回查询语句
    virtual std::string req() const = 0;
    // 返回查询结果
    virtual QueryResult eval(TextQuery & tq) const = 0;
};


// 1.1 class WordQuery
class WordQuery:
    public Query_base
{
    friend class Query;
private:
    WordQuery(const string & sought);
    string req() const override;
    QueryResult eval(TextQuery & tq) const override;
private:
    string _sought;
};
// 1.2 class NotQuery
class NotQuery:
    public Query_base
{
    friend Query operator~(const Query & query);
private:
    NotQuery(const Query & q);
    string req() const override;
    QueryResult eval(TextQuery & tq) const override;
private:
    Query _q;
};


// 1.3 class BinaryQuery
class BinaryQuery:
    public Query_base
{
    friend class Query;
protected:
    BinaryQuery(const Query & lhs, const Query & rhs, const string & op);
    string req() const override;
protected:
    Query _lhs;
    Query _rhs;
    string _opSym;
};


// 2.1 class AndQuery
class AndQuery:
    public BinaryQuery
{
    friend Query operator&(const Query & lhs,const Query & rhs);
private:
    AndQuery(const Query & lhs,const Query & rhs);
    QueryResult eval(TextQuery & tq) const override;
};
// 2.2 class OrQuery
class OrQuery:
    public BinaryQuery
{
    friend Query operator|(const Query & lhs,const Query & rhs);
private:
    OrQuery(const Query & lhs,const Query & rhs);
    QueryResult eval(TextQuery & tq) const override;
};

#endif
