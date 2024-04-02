#include <iostream>
#include <limits>
using std::cin;
using std::cout;
using std::endl;

int main()
{
    int num;
    cin >> num;
    if(!cin.good()){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    else{
        cout << num << endl;
    }
    cin >> num;
    cout << num << endl;
    return 0;
}

