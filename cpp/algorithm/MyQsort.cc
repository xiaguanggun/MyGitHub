#include <iostream>
#include <vector>
using namespace std;

template<typename T,typename Compare=std::less<T>>
class MyQsort
{
public:
    MyQsort(T *arr, size_t size):
        _cmp(Compare())
    {
        _vec.reserve(size);
        for(size_t i = 0; i < size; ++i){
            _vec.push_back(arr[i]);
        }
        quick(0,size-1);
    }
    void print(){
        for(auto &num : _vec){
            cout << num << " ";
        }
        cout << endl;
    }
private:
    void quick(int left, int right){
        // 递归出口
        if(left >= right){
            return;
        }
        // 递归体
        int pivot = partition(left,right);
        quick(left,pivot-1);
        quick(pivot+1,right);
    }
    int partition(int left, int right){
        T temp = _vec[left];
        while(left < right){
            while(left < right && _cmp(temp,_vec[right])){--right;}
            //while(left < right && temp < _vec[right]){--right;}
            if(left >= right){break;}
            _vec[left] = _vec[right];
            while(left < right && !_cmp(temp,_vec[left])){++left;}
            //while(left < right && _vec[left] <= temp){++left;}
            if(left >= right){break;}
            _vec[right] = _vec[left];
        }
        _vec[left] = temp;
        return left;
    }
    Compare _cmp;
    vector<T> _vec;
};

int main()
{
    /* int arr[10]{1,3,5,7,9,2,4,6,8,10}; */
    /* int arr[10]{3,5,1,7,9,2,4,10,8,6}; */
    int arr[10]{3,5,3,7,9,2,4,1,4,6};
    for(auto &num:arr){
        cout << num << " ";
    }
    cout << endl << "==========" << endl;
    /* MyQsort<int> mq(arr,sizeof(arr)/sizeof(int)); */
    MyQsort<int,std::greater<int>> mq(arr,sizeof(arr)/sizeof(int));
    mq.print();
    return 0;
}

