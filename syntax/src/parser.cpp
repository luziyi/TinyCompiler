#include "../include/parser.h"

#include <cstdio>
#include <iostream>

#include "../include/grammar.h"
#include "../include/parser_table.h"

// 词法分析文件路径
const char lexFile_path[] = "./result/lexical.txt";
// 语法分析结果文件路径
const char syntax_analysis_path[] = "./result/syntax_analysis.txt";

// 分析计数器
int cnt_analysis = 0;
// 分析栈
std::string stack[N];
int top = 0;
// 输入字符流
std::vector<std::string> chars;

void getChars(std::string str) {
    /*
     * Get the chars that shoud be pushed into the stack
     */
    std::string c1 = "", c2 = "", c3 = "";
    int id = 0;
    while (str[id] != ' ' && str[id] != '\t') c1 += str[id++];
    while (str[id] == ' ' || str[id] == '\t') id++;
    id++;
    while (str[id] != ',') c2 += str[id++];
    id++;
    while (str[id] != '>') c3 += str[id++];
    if (c2 != "Ident" && c2 != "INT" && c2 != "FLOAT") {
        chars.push_back(c1);
    } else
        chars.push_back(c2);
}

bool doAnalysis() {
    /*
     * do sysntax analysis use predictive analysis table
     */
    FILE *fp = freopen(syntax_analysis_path, "w", stdout);
    auto it = chars.begin();
    while (1) {
        std::string token = stack[top - 1];
        if (Vt.count(token)) {
            if (token == *it) {
                if (token == "#") {
                    // cout << ++cnt_analysis << " " << token << "#" << *it <<
                    // "\t" << "accept" << endl;
                    std::cout << ++cnt_analysis << " " << "EOF#EOF" << "\t"
                              << "accept" << std::endl;
                    fflush(fp);
                    fclose(fp);
                    freopen("CON", "w", stdout);
                    return true;
                } else {
                    std::cout << ++cnt_analysis << " " << token << "#" << *it
                              << "\t"
                              << "move" << std::endl;
                    top--, it++;
                }
            } else {
                std::cout << ++cnt_analysis << " " << token << "#" << *it
                          << "\t"
                          << "error" << std::endl;
                fflush(fp);
                fclose(fp);
                freopen("CON", "w", stdout);
                return false;
            }
        } else {
            int Id = Map[token];
            int id = vt[*it];
            if (table[Id][id].second == -1) {
                std::cout << ++cnt_analysis << " " << token << "#" << *it
                          << "\t"
                          << "error" << std::endl;
                fflush(fp);
                fclose(fp);
                freopen("CON", "w", stdout);
                std::cout << Id << " " << id << std::endl;
                std::cout << "error info : " << token << " " << *it
                          << std::endl;
                return false;
            } else {
                std::cout << ++cnt_analysis << " " << token << "#" << *it
                          << "\t"
                          << "reduction" << std::endl;
                top--;
                int gId = table[Id][id].second;
                if (gId == 99) continue;
                std::vector<std::string> vv = v[Id][gId];
                for (int i = vv.size() - 1; i >= 0; i--) {
                    stack[top++] = vv[i];
                }
            }
        }
    }
}

bool analysis() {
    /*
     * read data from file and getChars and doAnalysis
     * return whether the analysis was successful
     */
    stack[top++] = "#";
    stack[top++] = "program";
    FILE *fp = freopen(lexFile_path, "r", stdin);
    char c;
    std::string str = "";
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            getChars(str);
            str = "";
            continue;
        }
        str += c;
    }
    fclose(fp);
    chars.push_back("#");
    return doAnalysis();
}