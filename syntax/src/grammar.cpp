#include "../include/grammar.h"

#include <cstdio>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// 语法文件路径
const char grammer_path[] = "data/grammer.txt";

// 非终结符和终结符集合
std::set<std::string> Vn, Vt;

// 语法映射
std::map<std::string, int> Map;
std::map<int, std::string> Map2;

// 语法规则向量
std::vector<std::vector<std::string>> v[N];
int v_cnt = 0;

void getVnVt() {
    /*
     * read the grammer.txt and get Vn  wihich the set of non terminal
     * characters  and Vt which the set of terminal characters
     */
    FILE *fp = freopen(grammer_path, "r", stdin);
    char c;
    std::vector<std::string> *right;
    std::string cur = "";
    bool left = 1;
    int Id;
    std::string str = "";
    int line = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            Vt.insert(cur);
            right->push_back(cur);
            v[Id].push_back(*right);
            left = 1;
            cur = "";
            continue;
        }
        if (c != ' ' && c != '\t') {
            cur += c;
            continue;
        } else {
            if (cur == "") continue;
            if (cur == "->") {
                left = 0;
                cur = "";
            } else if (left) {
                Id = Map[cur];
                if (Id == 0) {
                    Id = ++v_cnt;
                    Map[cur] = Id;
                    Map2[Id] = cur;
                }
                Vn.insert(cur);
                right = new std::vector<std::string>;
                cur = "";
            } else {
                right->push_back(cur);
                Vt.insert(cur);
                cur = "";
            }
        }
    }

    for (std::string token : Vn) Vt.erase(token);
    fclose(fp);
}