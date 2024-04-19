#include <iostream>
#include <stack>
#include <unordered_map>

using namespace std;

class Solution {
public:
    Solution(){
        _umap['('] = ')';
        _umap['['] = ']';
        _umap['{'] = '}';
    }
    bool isValid(string s) {
        stack<char> stk;
        for(auto &ch:s){
            switch(ch){
            case '(':case '[':case '{':
                stk.push(ch);break;
            default:
                if(!stk.empty() && ch == _umap[stk.top()]){
                    stk.pop();
                    continue;
                }else{
                    return false;
                }
            }
        }
        return stk.empty();
    }
private:
    unordered_map<char,char> _umap;
};

int main()
{
    string s = "]";
    Solution solution;
    cout << solution.isValid(s) << endl;
    return 0;
}

