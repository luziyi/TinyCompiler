#include "Grammar.h"

#include <algorithm>
#include <fstream>
#include <sstream>

// Production类方法实现
void Production::print() const {
    std::cout << left << " -> ";
    for (const auto& symbol : right) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
}

// Grammar类构造函数
Grammar::Grammar() : startSymbol("") {}

// 从文件加载文法
bool Grammar::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '/' || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string left, arrow, token;

        // 解析产生式左部
        if (!(iss >> left)) {
            continue;
        }

        // 解析箭头符号
        if (!(iss >> arrow) || arrow != "->") {
            std::cerr << "格式错误，缺少箭头: " << line << std::endl;
            continue;
        }

        // 解析产生式右部
        std::vector<std::string> right;
        while (iss >> token) {
            right.push_back(token);
        }

        // 添加产生式
        addProduction(Production(left, right));

        // 如果是第一个产生式，则设置开始符号
        if (startSymbol.empty()) {
            startSymbol = left;
        }
    }

    file.close();
    return true;
}

// 保存文法到文件
bool Grammar::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    for (const auto& prod : productions) {
        file << prod.left << " -> ";
        for (const auto& symbol : prod.right) {
            file << symbol << " ";
        }
        file << std::endl;
    }

    file.close();
    return true;
}

// 添加产生式
void Grammar::addProduction(const Production& prod) {
    productions.push_back(prod);

    // 添加左部符号到非终结符集合
    nonTerminals.insert(prod.left);

    // 处理右部符号，所有右部符号都先加入终结符集合
    for (const auto& symbol : prod.right) {
        if (symbol != "$") {  // 空串不算终结符
            terminals.insert(symbol);
        }
    }

    // 从终结符集合中移除所有非终结符
    for (const auto& nonTerminal : nonTerminals) {
        terminals.erase(nonTerminal);
    }
}

// 获取所有产生式
const std::vector<Production>& Grammar::getProductions() const {
    return productions;
}

// 获取终结符集合
const std::set<std::string>& Grammar::getTerminals() const { return terminals; }

// 获取非终结符集合
const std::set<std::string>& Grammar::getNonTerminals() const {
    return nonTerminals;
}

// 获取开始符号
const std::string& Grammar::getStartSymbol() const { return startSymbol; }

// 设置开始符号
void Grammar::setStartSymbol(const std::string& symbol) {
    startSymbol = symbol;
}

// 判断符号是否为终结符
bool Grammar::isTerminal(const std::string& symbol) const {
    return terminals.find(symbol) != terminals.end();
}

// 判断符号是否为非终结符
bool Grammar::isNonTerminal(const std::string& symbol) const {
    return nonTerminals.find(symbol) != nonTerminals.end();
}

// 打印文法
void Grammar::print() const {
    std::cout << "文法:" << std::endl;
    for (const auto& prod : productions) {
        prod.print();
    }

    std::cout << "\n终结符: ";
    for (const auto& terminal : terminals) {
        std::cout << terminal << " ";
    }

    std::cout << "\n非终结符: ";
    for (const auto& nonTerminal : nonTerminals) {
        std::cout << nonTerminal << " ";
    }

    std::cout << "\n开始符号: " << startSymbol << std::endl;
}

// 根据左部查找所有产生式
std::vector<Production> Grammar::getProductionsByLeft(
    const std::string& left) const {
    std::vector<Production> result;
    for (const auto& prod : productions) {
        if (prod.left == left) {
            result.push_back(prod);
        }
    }
    return result;
}