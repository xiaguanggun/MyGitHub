#include <iostream>
#include "query.h"
using namespace std;

int main()
{
    // 文件名
    cout << "请输入文件名:" << endl;
    string filename;
    cin >> filename;
    // 遍历文件
    TextQuery tq(107);
    tq.readFile(filename);
    // 查询
    string word;
#if 0
    while(1){
        cout << "请输入待查询单词:" << endl;
        cin >> word;
        if(cin.eof()){
            break;
        }
        cout << tq.query(word);
    }
#endif

#if 0
    while(1){
        cout << "请输入查询条件(支持~|&操作):" << endl;
        cin >> word;
        if(cin.eof()){
            break;
        }
    }
#endif
    Query q1(Query("the") & Query("a"));
    Query q2(Query("an") | Query("a"));
    Query q3(~Query("a"));
    Query q4(Query("the") & Query("a") | Query("an"));
    Query q5(Query("an"));
    cout << q1.eval(tq);
    cout << q2.eval(tq);
    cout << q3.eval(tq);
    cout << q4.eval(tq);
    cout << q5.eval(tq);

    return 0;
}

