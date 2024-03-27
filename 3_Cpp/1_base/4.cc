#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    int x;
    cin >> x;
    if(x++ > 5)
    {
        cout << x << endl;
    }
    else
    {
        cout << x-- << endl;
    }

    return 0;
}
