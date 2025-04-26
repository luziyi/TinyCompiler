#include "../include/first_follow.h"

#include <cstdio>
#include <iostream>

#include "../include/grammar.h"

// first集和follow集
std::map<std::string, std::set<std::string>> first = {
    {"$", std::set<std::string>{"$"}}};
std::map<std::string, std::set<std::string>> follow = {
    {"program", std::set<std::string>{"#"}}};

// 输出文件路径
const char first_file[] = "./result/first.txt";
const char follow_file[] = "./result/follow.txt";

void getFirst(std::string token) {
    /*
     * Get the first set of each characters recuresively
     */
    if (first.count(token) > 0) return;
    if (Vt.count(token)) {
        std::set<std::string> st;
        st.insert(token);
        first[token] = st;
        return;
    }
    int Id = Map[token];
    std::set<std::string> st;
    for (size_t i = 0; i < v[Id].size(); i++) {
        std::vector<std::string> vv = v[Id][i];
        bool AllHasE = 1;
        for (std::string str : vv) {
            getFirst(str);
            std::set<std::string> curset = first[str];
            bool hasE = 0;
            for (std::string ss : curset) {
                if (ss == "$") {
                    hasE = 1;
                } else {
                    st.insert(ss);
                }
            }
            if (!hasE) {
                AllHasE = 0;
                break;
            }
        }
        if (AllHasE) st.insert("$");
    }
    first[token] = st;
    for (size_t i = 0; i < v[Id].size(); i++) {
        std::vector<std::string> vv = v[Id][i];
        for (std::string str : vv) {
            getFirst(str);
        }
    }
}

bool getFollow() {
    /*
     * Get the follow sets of non terminal characters
     * return whether the follow set updated
     * when the follow set did not change after one scanning , stop to call this
     * function
     */
    bool updated = false;
    for (std::string token : Vn) {
        if (follow.count(token) == 0) {
            std::set<std::string> st;
            follow[token] = st;
        }
        std::set<std::string> st = follow[token];
        size_t size_before = st.size();
        for (int i = 1; i <= v_cnt; i++) {
            for (size_t j = 0; j < v[i].size(); j++) {
                std::vector<std::string> vv = v[i][j];
                bool found = false;
                bool hasNotE = false;
                for (std::string str : vv) {
                    if (found) {
                        std::set<std::string> curset = first[str];
                        bool hasE = false;
                        for (std::string cur : curset) {
                            if (cur == "$")
                                hasE = true;
                            else
                                st.insert(cur);
                        }
                        if (!hasE) {
                            hasNotE = true;
                            break;
                        }
                    }
                    if (str == token) found = true;
                }
                if (!hasNotE && found) {
                    std::string head = Map2[i];
                    if (!follow.count(head)) continue;
                    std::set<std::string> curset = follow[head];
                    for (std::string cur : curset) {
                        st.insert(cur);
                    }
                }
            }
        }
        if (st.size() != size_before) {
            follow[token] = st;
            updated = true;
        }
    }
    return updated;
}

void print_first() {
    FILE *fp = freopen(first_file, "w", stdout);
    for (std::string token : Vn) {
        std::set<std::string> st = first[token];
        std::cout << token << std::endl << " -- ";
        for (std::string cur : st) {
            std::cout << "'" << cur << "' ";
        }
        std::cout << std::endl;
    }
    fflush(fp);
    fclose(fp);
    freopen("CON", "w", stdout);
}

void print_follow() {
    FILE *fp = freopen(follow_file, "w", stdout);
    for (std::string token : Vn) {
        std::set<std::string> st = follow[token];
        std::cout << token << std::endl << " -- ";
        for (std::string cur : st) {
            std::cout << "'" << cur << "'" << " ";
        }
        std::cout << std::endl;
    }
    fflush(fp);
    fclose(fp);
    freopen("CON", "w", stdout);
}