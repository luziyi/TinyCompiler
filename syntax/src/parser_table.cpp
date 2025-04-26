#include "../include/parser_table.h"

#include <cstdio>
#include <iostream>

#include "../include/first_follow.h"
#include "../include/grammar.h"

// 预测分析表
std::pair<int, int> table[N][N];
std::map<std::string, int> vn;
std::map<std::string, int> vt;
int cnt_vn;
int cnt_vt;
const std::pair<int, int> _00 = {0, -1};

// 表输出文件路径
const char table_file[] = "./result/table.txt";

bool getTable() {
    /*
     * Get the predictive analysis table use ALgorithm which teached in the
     * classs
     */
    cnt_vn = 0;
    for (std::string token : Vn) {
        vn[token] = ++cnt_vn;
    }
    cnt_vt = 0;
    Vt.insert("#");
    for (std::string token : Vt) {
        vt[token] = ++cnt_vt;
    }
    for (int i = 0; i <= cnt_vn; i++) {
        for (int j = 0; j <= cnt_vt; j++) {
            table[i][j] = _00;
        }
    }
    bool wrong = false;
    for (std::string token : Vn) {
        int Id = Map[token];
        bool hasE = false;
        for (size_t i = 0; i < v[Id].size(); i++) {
            std::set<std::string> st;
            std::vector<std::string> vv = v[Id][i];
            bool theE = true;
            for (std::string cur : vv) {
                bool hasNotE = true;
                std::set<std::string> curset = first[cur];
                for (std::string item : curset) {
                    if (item == "$") {
                        hasNotE = false;
                        continue;
                    }
                    st.insert(item);
                }
                if (hasNotE) {
                    theE = false;
                    break;
                }
            }
            for (std::string item : st) {
                int id = vt[item];
                if (table[Id][id] != _00) {
                    wrong = true;
                    return wrong;
                }
                table[Id][id] = {Id, i};
            }
            if (theE) hasE = true;
        }
        if (hasE) {
            std::set<std::string> st = follow[token];
            for (std::string item : st) {
                int id = vt[item];
                if (table[Id][id] != _00) {
                    wrong = true;
                    return wrong;
                }
                table[Id][id] = {Id, N - 1};
            }
        }
    }
    return wrong;
}

void print_table() {
    FILE *fp = freopen(table_file, "w", stdout);
    std::cout << "   ";
    for (std::string cur : Vt) {
        std::cout << cur << " ";
    }
    std::cout << std::endl;
    for (std::string token : Vn) {
        int Id = Map[token];
        std::cout << token << " ";
        for (std::string cur : Vt) {
            int id = vt[cur];
            std::cout << table[Id][id].second << " ";
        }
        std::cout << std::endl;
    }
    fflush(fp);
    fclose(fp);
    freopen("CON", "w", stdout);
}