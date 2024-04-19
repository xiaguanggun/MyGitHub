#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> words(wordList.begin(),wordList.end());
        if(!words.count(endWord)){
            return 0;
        }
        // 初始化
        queue<string> q;
        q.push(beginWord);
        unordered_map<string,int> pathLength;
        pathLength[beginWord] = 1;
        // BFS
        while(!q.empty()){
            string word = q.front();
            q.pop();
            int len = pathLength[word];
            string newWord = word;
            for(auto& ch : newWord){
                // 暂存自己
                char temp = ch;
                for(int i = 0; i < 26; ++i){
                    ch = 'a' + i;
                    // 跳过自己
                    if(ch == temp){
                        continue;
                    }
                    // 判断
                    // 1 已到终点,return len+1
                    if(newWord == endWord){
                        return len + 1;
                    }
                    // 2 未到终点
                    // 2.1 找不到下一个单词,continue
                    if(!words.count(newWord)){
                        continue;
                    }
                    // 2.2 找到下一个单词
                    // 2.2.1 已经搜索过,continue
                    if(pathLength.count(newWord)){
                        continue;
                    }
                    // 2.2.2 未搜索过,插入
                    q.push(newWord);
                    pathLength[newWord] = len + 1;
                }
                // 回溯
                ch = temp;
            }
        }
        return 0;
    }
};

int main()
{
    Solution s1;
    string beginWord = "hit";
    string endWord = "cog";
    vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    int len = s1.ladderLength(beginWord,endWord,wordList);
    cout << len << endl;
    return 0;
}

