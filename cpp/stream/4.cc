#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) 
        : _size(0)
          , _capacity(capacity)
    {}

    int get(int key) {
        if(_umap.count(key)){
            update(_umap[key]);
            return _umap[key]->second;
        }else{
            return -1;
        }
    }

    void put(int key, int value) {
        if(_umap.count(key)){
            /* cout << "更新value值" << endl; */
            // 更新value值
            _umap[key]->second = value;
            update(_umap[key]);
        }else if(_size < _capacity){
            /* cout << "插入value值" << endl; */
            // 插入链表(头插法)
            _list.push_front(pair(key,value));
            _umap[key] = _list.begin();
            ++_size;
        }else{
            /* cout << "删除" << endl; */
            // 删除最久未活跃对象
            int last_key = _list.back().first;
            _list.pop_back();
            _umap.erase(last_key);
            // 插入新元素
            _list.push_front(pair(key,value));
            _umap[key] = _list.begin();
        }
    }
private:
    // 更新链表
    void update(list<pair<int,int>>::iterator it){
        /* cout << "更新链表" << endl; */
        if(it == _list.begin()){
            return;
        }
        _list.splice(_list.begin(),_list,it);
    }
    // 成员变量
    int _size;
    int _capacity;
    unordered_map<int,list<pair<int,int>>::iterator> _umap;
    list<pair<int,int>> _list;
};

int main()
{
    LRUCache lRUCache(2);
    lRUCache.put(1, 0);
    lRUCache.put(2, 2);
    cout << lRUCache.get(1) << endl;
    lRUCache.put(3, 3);
    cout << lRUCache.get(2) << endl;
    lRUCache.put(4, 4);
    cout << lRUCache.get(1) << endl;
    cout << lRUCache.get(3) << endl;
    cout << lRUCache.get(4) << endl;
    return 0;
}

