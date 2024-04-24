#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;

// RSS文件每个item内容
struct RSSItem{
    RSSItem(const string& title,const string& link,const string& description,const string& content)
    :_title(title),_link(link),_description(description),_content(content)
    {}
    string _title;
    string _link;
    string _description;
    string _content;
};
// 处理RSS文件
class RSS{
public:
    RSS(size_t capacity = 20){
        _items.reserve(capacity);
    }
    // 读文件&处理
    void readRSS(const string& fileName){
        // 加载xml文件
        XMLDocument doc;
        doc.LoadFile(fileName.c_str());
        // 检查报错
        if(doc.ErrorID()){
            cout << "XML loadfile failed\n";
        }
        // 处理
        // 1.获取rss-->channel-->第一个item
        XMLElement * pItemNode = doc.FirstChildElement("rss")
                                ->FirstChildElement("channel")
                                ->FirstChildElement("item");
        // 2.循环处理所有item
        while(pItemNode){
            string title = pItemNode->FirstChildElement("title")->GetText();
            string link = pItemNode->FirstChildElement("link")->GetText();
            string description = pItemNode->FirstChildElement("description")->GetText();
            string content = pItemNode->FirstChildElement("content:encoded")->GetText();
            // 正则表达式处理content
            std::regex reg("<[^>]*>");
            description = regex_replace(description,reg,"");
            content = regex_replace(content,reg,"");
            // 插入vector
            _items.emplace_back(title,link,description,content);
            // 获取下一个item结点
            pItemNode = pItemNode->NextSiblingElement("item");
        }
    }
    // 写入新文件
    void storeFile(const string& fileName){
        ofstream ofs(fileName);
        if(!ofs){
            cerr << "ofstream open file failed\n";
        }
        for(size_t idx = 0; idx != _items.size(); ++idx)
        {
            ofs << "<doc><docid>" << idx + 1
                << "</docid><title>" << _items[idx]._title
                << "</title><link>" << _items[idx]._link
                << "</link><description>" << _items[idx]._description
                << "</description><content>" << _items[idx]._content << "</content></doc>";
            ofs << '\n';
        }
        ofs.close();
    }
private:
    vector<RSSItem> _items;
};

int main()
{
    RSS rss;
    rss.readRSS("coolshell.xml");
    rss.storeFile("pagelib.dat");
    return 0;
}

