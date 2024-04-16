#include <iostream>
#include <vector>

using namespace std;

template <typename T, typename Compare = std::less<T>> 
class HeapSort
{
public:
    HeapSort(T *arr, size_t size):
        _cmp(Compare())
    {
        _vec.reserve(size);
        for(size_t i = 0; i < size; ++i){
            _vec.push_back(arr[i]);
        }
    }
    void heapAdjust(size_t len,size_t idx){
        if(len <= 1){
            return;
        }
        while(1){
            size_t max_idx = idx;
            size_t lchild_idx = (idx << 1) + 1;
            size_t rchild_idx = (idx << 1) + 2;
            if(lchild_idx < len && _cmp(_vec[lchild_idx],_vec[max_idx])) {
                max_idx = lchild_idx;
            }
            if(rchild_idx < len && _cmp(_vec[rchild_idx],_vec[max_idx])) {
                max_idx = rchild_idx;
            }
            if(max_idx != idx) {
                swap(_vec[idx], _vec[max_idx]);
                idx = max_idx;
            }
            else{
                break;
            }
        }
        //print();
    }
    void sort(){
        // 初始化堆
        size_t len = _vec.size();
        for(int i = (len-2)>>1; i >= 0; --i){
            heapAdjust(len,i);
        }
        // 堆排序
        while(len > 1){
            // 交换元素
            swap(_vec[0],_vec[--len]);
            // 重新堆化
            heapAdjust(len,0);
        }
    }
    void print(){
        for(auto &num : _vec){
            cout << num << " ";
        }
        cout << endl;
    }
private:
    void swap(T &lhs, T &rhs){
        T temp = lhs;
        lhs = rhs;
        rhs = temp;
    }
    vector<T> _vec;
    Compare _cmp;
};

int main()
{
    /* int arr[10] = {3,5,7,9,1,6,8,10,2,4}; */
    int arr[10] = {3,5,7,8,2,4,8,10,2,4};
    for(auto &num : arr){
        cout << num << " ";
    }
    cout << endl;
    HeapSort<int> hs(arr,sizeof(arr)/sizeof(int));
    /* HeapSort<int,std::greater<int>> hs(arr,sizeof(arr)/sizeof(int)); */
    cout << "==========" << endl;
    hs.print();
    cout << "==========" << endl;
    hs.sort();
    hs.print();
    return 0;
}

