#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// 表示一个产生式
struct Production {
    std::string left;                // 产生式左部
    std::vector<std::string> right;  // 产生式右部

    // 默认构造函数
    Production() : left(""), right({}) {}

    // 构造函数
    Production(const std::string& l, const std::vector<std::string>& r)
        : left(l), right(r) {}

    // 打印产生式
    void print() const;
};

// 表示文法
class Grammar {
   private:
    std::vector<Production> productions;  // 所有产生式
    std::set<std::string> terminals;      // 终结符集合
    std::set<std::string> nonTerminals;   // 非终结符集合
    std::string startSymbol;              // 开始符号

   public:
    // 构造函数
    Grammar();

    // 从文件加载文法
    bool loadFromFile(const std::string& filename);

    // 保存文法到文件
    bool saveToFile(const std::string& filename) const;

    // 添加产生式
    void addProduction(const Production& prod);

    // 获取所有产生式
    const std::vector<Production>& getProductions() const;

    // 获取终结符集合
    const std::set<std::string>& getTerminals() const;

    // 获取非终结符集合
    const std::set<std::string>& getNonTerminals() const;

    // 获取开始符号
    const std::string& getStartSymbol() const;

    // 设置开始符号
    void setStartSymbol(const std::string& symbol);

    // 判断符号是否为终结符
    bool isTerminal(const std::string& symbol) const;

    // 判断符号是否为非终结符
    bool isNonTerminal(const std::string& symbol) const;

    // 打印文法
    void print() const;

    // 根据左部查找所有产生式
    std::vector<Production> getProductionsByLeft(const std::string& left) const;
};

#endif  // GRAMMAR_H