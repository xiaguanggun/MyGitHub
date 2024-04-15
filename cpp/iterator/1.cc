#include <iostream>
#include <vector>
#include <deque>
#include <list>

using namespace std;

template<class T>
void display(const T & seqCon){
    for(auto & num : seqCon){
        cout << num << " ";
    }
    cout << endl;
}

int main()
{
    vector<int> v1;
    vector<int> v2(10,5);
    auto it = v2.begin();
    vector<int> v3(it,it+5);
    vector<int> v4{1,3,5,2,4,6};
    vector<int> v5 = v3;
    vector<int> v6({1,3,4,7});
    display(v1);
    display(v2);
    display(v3);
    display(v4);
    display(v5);
    display(v6);
    cout << "==========" << endl;
    deque<int> d1;
    deque<int> d2(10,5);
    auto it2 = d2.begin();
    deque<int> d3(it2,it2+5);
    deque<int> d4{1,3,5,2,4,6};
    deque<int> d5 = d3;
    deque<int> d6({1,3,4,7});
    display(d1);
    display(d2);
    display(d3);
    display(d4);
    display(d5);
    display(d6);
    cout << "==========" << endl;
    list<int> l1;
    list<int> l2(10,5);
    auto it3 = l2.begin();
    auto it4 = l2.begin();
    ++it4;
    ++it4;
    ++it4;
    ++it4;
    ++it4;
    list<int> l3(it3,it4);
    list<int> l4{1,3,5,2,4,6};
    list<int> l5 = l3;
    list<int> l6({1,3,4,7});
    display(l1);
    display(l2);
    display(l3);
    display(l4);
    display(l5);
    display(l6);
    return 0;
}

