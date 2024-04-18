#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

int main()
{
    string map_file = "map.txt";
    string file_file = "file.txt";
    string dest_file = "dest.txt";

    // 处理map.txt
    ifstream ifs(map_file);
    if(!ifs){
        cout << "打开" << map_file << "失败!" << endl;
        return -1;
    }
    unordered_map<string,string> umap;
    string line;
    string key_word;
    while((getline(ifs,line),!ifs.eof())){
        istringstream iss(line);
        iss >> key_word;
        iss.ignore(std::numeric_limits<std::streamsize>::max(),' ');
        getline(iss,umap[key_word]);
    }
    ifs.close();
    for(auto &str_pair:umap){
        cout << str_pair.first << endl << str_pair.second << endl;
    }

    // 处理file.txt
    ifs.open(file_file);
    if(!ifs){
        cout << "打开" << file_file << "失败!" << endl;
        return -1;
    }
    ofstream ofs(dest_file);
    while((getline(ifs,line),!ifs.eof())){
        istringstream iss(line);
        ostringstream oss;
        while(!iss.eof()){
            iss >> key_word;
            if(umap.count(key_word)){
                oss << umap[key_word] << " ";
            }else{
                oss << key_word << " ";
            }
        }
        ofs << oss.str();
        ofs << endl;
    }
    ifs.close();
    ofs.close();

    return 0;
}

