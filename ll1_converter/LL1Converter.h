#ifndef LL1_CONVERTER_H
#define LL1_CONVERTER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Grammar.h"

// LL1转换器类
class LL1Converter {
   private:
    Grammar originalGrammar;                                  // 原始文法
    Grammar ll1Grammar;                                       // 转换后的LL1文法
    std::map<std::string, std::set<std::string>> firstSets;   // First集
    std::map<std::string, std::set<std::string>> followSets;  // Follow集
    std::map<std::pair<std::string, std::string>, Production>
        parseTable;  // 预测分析表

    // 是否存在左递归
    bool hasLeftRecursion(const Grammar& grammar) const;

    // 消除直接左递归
    Grammar eliminateDirectLeftRecursion(const Grammar& grammar);

    // 消除所有左递归
    Grammar eliminateAllLeftRecursion(const Grammar& grammar);

    // 提取左公因子
    Grammar eliminateLeftFactoring(const Grammar& grammar);

    // 计算First集
    void computeFirstSets();

    // 计算单个符号的First集
    std::set<std::string> computeFirstSet(const std::string& symbol);

    // 计算产生式右部的First集
    std::set<std::string> computeFirstSetOfRightSide(
        const std::vector<std::string>& rightSide);

    // 计算Follow集
    void computeFollowSets();

    // 构建预测分析表
    void buildParseTable();

    // 检查是否是LL1文法
    bool isLL1Grammar() const;

   public:
    // 构造函数
    LL1Converter(const Grammar& grammar);

    // 转换为LL1文法
    bool convertToLL1();

    // 获取原始文法
    const Grammar& getOriginalGrammar() const;

    // 获取LL1文法
    const Grammar& getLL1Grammar() const;

    // 获取First集
    const std::map<std::string, std::set<std::string>>& getFirstSets() const;

    // 获取Follow集
    const std::map<std::string, std::set<std::string>>& getFollowSets() const;

    // 获取预测分析表
    const std::map<std::pair<std::string, std::string>, Production>&
    getParseTable() const;

    // 保存First集到文件
    bool saveFirstSets(const std::string& filename) const;

    // 保存Follow集到文件
    bool saveFollowSets(const std::string& filename) const;

    // 保存预测分析表到文件
    bool saveParseTable(const std::string& filename) const;

    // 打印First集
    void printFirstSets() const;

    // 打印Follow集
    void printFollowSets() const;

    // 打印预测分析表
    void printParseTable() const;
};

#endif  // LL1_CONVERTER_H