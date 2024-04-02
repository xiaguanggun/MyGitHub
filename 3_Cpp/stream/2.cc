#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

int main()
{
    vector<vector<int>> v1{{1,2},{3,4},{5,6}};
    for(auto & it : v1){
        for(auto & num : it){
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}

