#include "LL1Converter.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

// 构造函数
LL1Converter::LL1Converter(const Grammar& grammar)
    : originalGrammar(grammar), ll1Grammar(grammar) {}

// 转换为LL1文法
bool LL1Converter::convertToLL1() {
    std::cout << "开始转换文法为LL1文法..." << std::endl;

    // 1. 消除左递归
    std::cout << "步骤 1: 消除左递归..." << std::endl;
    ll1Grammar = eliminateAllLeftRecursion(ll1Grammar);
    std::cout << "消除左递归后的文法:" << std::endl;
    ll1Grammar.print();
    std::cout << "-------------------------" << std::endl;

    // 2. 提取左公因子
    std::cout << "步骤 2: 提取左公因子..." << std::endl;
    ll1Grammar = eliminateLeftFactoring(ll1Grammar);
    std::cout << "提取左公因子后的文法:" << std::endl;
    ll1Grammar.print();
    std::cout << "-------------------------" << std::endl;

    // 3. 计算First集
    std::cout << "步骤 3: 计算First集..." << std::endl;
    computeFirstSets();
    printFirstSets();
    std::cout << "-------------------------" << std::endl;

    // 4. 计算Follow集
    std::cout << "步骤 4: 计算Follow集..." << std::endl;
    computeFollowSets();
    printFollowSets();
    std::cout << "-------------------------" << std::endl;

    // 5. 构建预测分析表
    std::cout << "步骤 5: 构建预测分析表..." << std::endl;
    buildParseTable();
    printParseTable();
    std::cout << "-------------------------" << std::endl;

    // 6. 检查是否是LL1文法
    if (isLL1Grammar()) {
        std::cout << "转换成功！生成的文法是LL1文法。" << std::endl;
        return true;
    } else {
        std::cerr << "转换失败！生成的文法不是LL1文法。" << std::endl;
        return false;
    }
}

// --- 左递归消除 ---

// 检查是否存在左递归（简化版，仅检查直接左递归）
bool LL1Converter::hasLeftRecursion(const Grammar& grammar) const {
    for (const auto& nonTerminal : grammar.getNonTerminals()) {
        for (const auto& prod : grammar.getProductionsByLeft(nonTerminal)) {
            if (!prod.right.empty() && prod.right[0] == nonTerminal) {
                return true;
            }
        }
    }
    return false;
}

// 消除直接左递归
// A -> A α1 | A α2 | ... | A αm | β1 | β2 | ... | βn
// 转换为:
// A -> β1 A' | β2 A' | ... | βn A'
// A' -> α1 A' | α2 A' | ... | αm A' | $
Grammar LL1Converter::eliminateDirectLeftRecursion(const Grammar& grammar) {
    Grammar newGrammar;
    newGrammar.setStartSymbol(grammar.getStartSymbol());

    // 遍历每个非终结符
    for (const auto& nonTerminal : grammar.getNonTerminals()) {
        std::vector<Production> nonTerminalProds =
            grammar.getProductionsByLeft(nonTerminal);

        // 分类产生式：递归的和非递归的
        std::vector<std::vector<std::string>>
            alphas;  // 递归产生式右部(不含首个非终结符)
        std::vector<std::vector<std::string>> betas;  // 非递归产生式右部

        for (const auto& prod : nonTerminalProds) {
            if (!prod.right.empty() && prod.right[0] == nonTerminal) {
                // 如果是递归产生式 A -> A α，则提取α部分
                std::vector<std::string> alpha(prod.right.begin() + 1,
                                               prod.right.end());
                alphas.push_back(alpha);
            } else {
                // 非递归产生式 A -> β
                betas.push_back(prod.right);
            }
        }

        // 如果存在直接左递归
        if (!alphas.empty()) {
            // 创建新的非终结符 A'
            std::string newNonTerminal = nonTerminal + "'";

            // 添加 A -> β A' 产生式
            if (betas.empty()) {
                // 如果没有非递归产生式，添加一个推导空串的产生式
                betas.push_back({"$"});
            }

            for (const auto& beta : betas) {
                std::vector<std::string> newRight = beta;
                if (newRight.size() == 1 && newRight[0] == "$") {
                    // 如果β是空串，则 A -> A'
                    newGrammar.addProduction(
                        Production(nonTerminal, {newNonTerminal}));
                } else {
                    // 否则 A -> β A'
                    newRight.push_back(newNonTerminal);
                    newGrammar.addProduction(Production(nonTerminal, newRight));
                }
            }

            // 添加 A' -> α A' 产生式
            for (const auto& alpha : alphas) {
                std::vector<std::string> newRight = alpha;
                newRight.push_back(newNonTerminal);
                newGrammar.addProduction(Production(newNonTerminal, newRight));
            }

            // 添加 A' -> $ 产生式(空串)
            newGrammar.addProduction(Production(newNonTerminal, {"$"}));
        } else {
            // 没有直接左递归，直接添加原产生式
            for (const auto& prod : nonTerminalProds) {
                newGrammar.addProduction(prod);
            }
        }
    }

    return newGrammar;
}

// 消除所有左递归（包括间接左递归）
// 算法：
// 1. 将非终结符排序 A1, A2, ..., An
// 2. for i = 1 to n:
// 3.   for j = 1 to i-1:
// 4.     对于每个形如 Ai -> Aj γ 的产生式:
// 5.       将 Aj 的所有产生式 Aj -> δ1 | δ2 | ... | δk 代入
// 6.       得到 Ai -> δ1 γ | δ2 γ | ... | δk γ
// 7.   消除 Ai 的直接左递归
Grammar LL1Converter::eliminateAllLeftRecursion(const Grammar& grammar) {
    // 创建一个新的Grammar对象用于存储转换后的结果
    Grammar resultGrammar;
    resultGrammar.setStartSymbol(grammar.getStartSymbol());

    // 1. 将非终结符按字典序排序
    std::vector<std::string> nonTerminals(grammar.getNonTerminals().begin(),
                                          grammar.getNonTerminals().end());
    std::sort(nonTerminals.begin(), nonTerminals.end());

    // 深拷贝原始文法的所有产生式到resultGrammar
    for (const auto& prod : grammar.getProductions()) {
        resultGrammar.addProduction(prod);
    }

    // 2. 逐个处理每个非终结符
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        std::string Ai = nonTerminals[i];

        // 3. 对于每个j < i的非终结符Aj
        for (size_t j = 0; j < i; ++j) {
            std::string Aj = nonTerminals[j];

            // 创建新的产生式集合，用于替换Ai的产生式
            std::vector<Production> newAiProductions;

            // 获取Ai的所有产生式
            std::vector<Production> aiProductions =
                resultGrammar.getProductionsByLeft(Ai);

            // 4. 对于Ai的每个产生式，检查是否形如 Ai -> Aj γ
            for (const auto& aiProd : aiProductions) {
                if (!aiProd.right.empty() && aiProd.right[0] == Aj) {
                    // 找到形如 Ai -> Aj γ 的产生式
                    // 提取γ部分（去掉Aj后的部分）
                    std::vector<std::string> gamma(aiProd.right.begin() + 1,
                                                   aiProd.right.end());

                    // 获取Aj的所有产生式
                    std::vector<Production> ajProductions =
                        resultGrammar.getProductionsByLeft(Aj);

                    // 5. 用Aj的每个产生式右部δ替换，生成 Ai -> δ γ
                    for (const auto& ajProd : ajProductions) {
                        std::vector<std::string> delta = ajProd.right;

                        // 创建新的右部 δ γ
                        std::vector<std::string> newRight;

                        // 处理δ为空的情况
                        if (delta.size() == 1 && delta[0] == "$") {
                            newRight = gamma;  // 如果δ是空串，新的右部就是γ
                        } else {
                            // 否则，新的右部是δ + γ
                            newRight = delta;
                            newRight.insert(newRight.end(), gamma.begin(),
                                            gamma.end());
                        }

                        // 添加新的产生式 Ai -> δ γ
                        newAiProductions.push_back(Production(Ai, newRight));
                    }
                } else {
                    // 不是 Ai -> Aj γ 形式的产生式，保持不变
                    newAiProductions.push_back(aiProd);
                }
            }

            // 更新Grammar对象中Ai的产生式
            // 先移除所有Ai的旧产生式
            std::vector<Production> updatedProductions;
            for (const auto& prod : resultGrammar.getProductions()) {
                if (prod.left != Ai) {
                    updatedProductions.push_back(prod);
                }
            }

            // 将新的Ai产生式添加到Grammar
            Grammar tempGrammar;
            tempGrammar.setStartSymbol(resultGrammar.getStartSymbol());

            for (const auto& prod : updatedProductions) {
                tempGrammar.addProduction(prod);
            }

            // 使用集合去重，防止添加重复的产生式
            std::map<std::vector<std::string>, bool> uniqueProductions;
            for (const auto& prod : newAiProductions) {
                if (!uniqueProductions[prod.right]) {
                    tempGrammar.addProduction(prod);
                    uniqueProductions[prod.right] = true;
                }
            }

            resultGrammar = tempGrammar;
        }

        // 7. 消除Ai的直接左递归
        // 首先获取当前文法中Ai的所有产生式
        std::vector<Production> aiProductions =
            resultGrammar.getProductionsByLeft(Ai);

        // 分类产生式：递归的和非递归的
        std::vector<std::vector<std::string>>
            alphas;  // 递归部分(不含首个非终结符)
        std::vector<std::vector<std::string>> betas;  // 非递归部分

        for (const auto& prod : aiProductions) {
            if (!prod.right.empty() && prod.right[0] == Ai) {
                // 如果是递归产生式 Ai -> Ai α，则提取α部分
                std::vector<std::string> alpha(prod.right.begin() + 1,
                                               prod.right.end());
                alphas.push_back(alpha);
            } else {
                // 非递归产生式 Ai -> β
                betas.push_back(prod.right);
            }
        }

        // 如果存在直接左递归
        if (!alphas.empty()) {
            // 创建临时Grammar对象存储转换结果
            Grammar tempGrammar;
            tempGrammar.setStartSymbol(resultGrammar.getStartSymbol());

            // 保留所有非Ai的产生式
            for (const auto& prod : resultGrammar.getProductions()) {
                if (prod.left != Ai) {
                    tempGrammar.addProduction(prod);
                }
            }

            // 创建新的非终结符 Ai'
            std::string newNonTerminal = Ai + "'";

            // 如果没有非递归产生式，添加 Ai -> Ai'
            if (betas.empty()) {
                tempGrammar.addProduction(Production(Ai, {newNonTerminal}));
            } else {
                // 使用集合去重，防止添加重复的β
                std::map<std::vector<std::string>, bool> uniqueBetas;
                for (const auto& beta : betas) {
                    if (!uniqueBetas[beta]) {
                        std::vector<std::string> newRight = beta;
                        if (!(newRight.size() == 1 && newRight[0] == "$")) {
                            // 如果β不是空串，添加Ai'
                            newRight.push_back(newNonTerminal);
                        } else {
                            // 如果β是空串，则 Ai -> Ai'
                            newRight = {newNonTerminal};
                        }
                        tempGrammar.addProduction(Production(Ai, newRight));
                        uniqueBetas[beta] = true;
                    }
                }
            }

            // 使用集合去重，防止添加重复的α
            std::map<std::vector<std::string>, bool> uniqueAlphas;
            for (const auto& alpha : alphas) {
                if (!uniqueAlphas[alpha]) {
                    std::vector<std::string> newRight = alpha;
                    newRight.push_back(newNonTerminal);
                    tempGrammar.addProduction(
                        Production(newNonTerminal, newRight));
                    uniqueAlphas[alpha] = true;
                }
            }

            // 添加 Ai' -> ε 产生式
            tempGrammar.addProduction(Production(newNonTerminal, {"$"}));

            // 更新resultGrammar
            resultGrammar = tempGrammar;
        }
    }

    return resultGrammar;
}

// --- 提取左公因子 ---
// 提取左公因子
// 比如把 A -> αβ1 | αβ2 | ... | γ 转换为
// A -> αA' | γ
// A' -> β1 | β2 | ...
Grammar LL1Converter::eliminateLeftFactoring(const Grammar& grammar) {
    Grammar resultGrammar;
    resultGrammar.setStartSymbol(grammar.getStartSymbol());

    // 复制原始文法的所有产生式
    for (const auto& prod : grammar.getProductions()) {
        resultGrammar.addProduction(prod);
    }

    // 记录所有非终结符，包括在处理过程中新生成的
    std::vector<std::string> allNonTerminals(grammar.getNonTerminals().begin(),
                                             grammar.getNonTerminals().end());

    // 保存需要继续处理的非终结符
    std::queue<std::string> nonTerminalsToProcess;
    // 初始化队列，先处理原始文法中的所有非终结符
    for (const auto& nt : allNonTerminals) {
        nonTerminalsToProcess.push(nt);
    }

    // 用于确保新生成的非终结符名称唯一
    int suffix = 1;

    // 处理队列中的每个非终结符，直到队列为空
    while (!nonTerminalsToProcess.empty()) {
        std::string currentNT = nonTerminalsToProcess.front();
        nonTerminalsToProcess.pop();

        // 获取当前非终结符的所有产生式
        std::vector<Production> currentProductions =
            resultGrammar.getProductionsByLeft(currentNT);

        // 如果只有一个产生式，无需提取左公因子
        if (currentProductions.size() <= 1) {
            continue;
        }

        // 按产生式右部的第一个符号分组
        std::map<std::string, std::vector<std::vector<std::string>>>
            prefixGroups;
        for (const auto& prod : currentProductions) {
            if (prod.right.empty()) {
                // 空产生式，用特殊标记
                prefixGroups["$"].push_back({});
            } else {
                std::string firstSymbol = prod.right[0];
                prefixGroups[firstSymbol].push_back(prod.right);
            }
        }

        // 检查是否有需要提取的公共前缀
        bool needFactoring = false;
        for (const auto& [prefix, rightSides] : prefixGroups) {
            if (rightSides.size() > 1) {
                needFactoring = true;
                break;
            }
        }

        if (!needFactoring) {
            continue;  // 没有公共前缀，处理下一个非终结符
        }

        // 创建一个新的Grammar对象存储提取公因子后的结果
        Grammar tempGrammar;
        tempGrammar.setStartSymbol(resultGrammar.getStartSymbol());

        // 复制除当前非终结符外的所有产生式
        for (const auto& prod : resultGrammar.getProductions()) {
            if (prod.left != currentNT) {
                tempGrammar.addProduction(prod);
            }
        }

        // 处理每个前缀组
        for (const auto& [prefix, rightSides] : prefixGroups) {
            if (rightSides.size() <= 1) {
                // 只有一个产生式，直接添加
                for (const auto& right : rightSides) {
                    tempGrammar.addProduction(Production(currentNT, right));
                }
                continue;
            }

            // 需要提取左公因子
            // 找到最长公共前缀
            std::vector<std::string> commonPrefix;
            size_t minLength = SIZE_MAX;

            // 找到最短的右部长度
            for (const auto& right : rightSides) {
                minLength = std::min(minLength, right.size());
            }

            // 逐符号比较，找出最长公共前缀
            for (size_t i = 0; i < minLength; ++i) {
                std::string currentSymbol = rightSides[0][i];
                bool allMatch = true;

                for (size_t j = 1; j < rightSides.size(); ++j) {
                    if (rightSides[j][i] != currentSymbol) {
                        allMatch = false;
                        break;
                    }
                }

                if (allMatch) {
                    commonPrefix.push_back(currentSymbol);
                } else {
                    break;
                }
            }

            // 创建新的非终结符
            std::string newNT = currentNT + "_" + std::to_string(suffix++);

            // 记录新生成的非终结符，以便后续处理
            allNonTerminals.push_back(newNT);
            nonTerminalsToProcess.push(newNT);

            // 添加 A -> α A'
            std::vector<std::string> newRight = commonPrefix;
            newRight.push_back(newNT);
            tempGrammar.addProduction(Production(currentNT, newRight));

            // 添加 A' -> β1 | β2 | ...
            // 使用集合去重，避免添加重复的产生式
            std::map<std::vector<std::string>, bool> uniqueSuffixes;

            for (const auto& right : rightSides) {
                if (right.size() == commonPrefix.size()) {
                    // 提取后为空，添加空产生式
                    if (!uniqueSuffixes[{"$"}]) {
                        tempGrammar.addProduction(Production(newNT, {"$"}));
                        uniqueSuffixes[{"$"}] = true;
                    }
                } else {
                    // 添加提取后的后缀
                    std::vector<std::string> suffix(
                        right.begin() + commonPrefix.size(), right.end());
                    if (!uniqueSuffixes[suffix]) {
                        tempGrammar.addProduction(Production(newNT, suffix));
                        uniqueSuffixes[suffix] = true;
                    }
                }
            }
        }

        // 更新resultGrammar
        resultGrammar = tempGrammar;
    }

    return resultGrammar;
}

// --- First集计算 ---

// 计算所有非终结符的First集
void LL1Converter::computeFirstSets() {
    firstSets.clear();
    bool changed = true;

    // 初始化First集
    for (const auto& nonTerminal : ll1Grammar.getNonTerminals()) {
        firstSets[nonTerminal] = {};
    }
    // 初始化终结符的First集
    for (const auto& terminal : ll1Grammar.getTerminals()) {
        firstSets[terminal] = {terminal};
    }
    firstSets["$"] = {"$"};  // 空串的First集是自身

    while (changed) {
        changed = false;
        for (const auto& nonTerminal : ll1Grammar.getNonTerminals()) {
            std::set<std::string> currentFirstSet = firstSets[nonTerminal];
            std::vector<Production> productions =
                ll1Grammar.getProductionsByLeft(nonTerminal);

            for (const auto& prod : productions) {
                std::set<std::string> rightFirstSet =
                    computeFirstSetOfRightSide(prod.right);
                for (const auto& symbol : rightFirstSet) {
                    if (firstSets[nonTerminal].find(symbol) ==
                        firstSets[nonTerminal].end()) {
                        firstSets[nonTerminal].insert(symbol);
                        changed = true;
                    }
                }
            }
        }
    }
}

// 计算单个符号的First集（可能是终结符或非终结符）
// 注意：这个函数在 computeFirstSets 的循环中被间接调用
std::set<std::string> LL1Converter::computeFirstSet(const std::string& symbol) {
    if (firstSets.count(symbol)) {
        return firstSets[symbol];
    }
    // 如果是终结符或 $
    if (ll1Grammar.isTerminal(symbol) || symbol == "$") {
        return {symbol};
    }
    // 如果是非终结符，则返回当前计算的集合（可能不完整，依赖外层循环）
    if (ll1Grammar.isNonTerminal(symbol)) {
        return firstSets[symbol];  // 返回当前已计算的部分
    }

    return {};  // 未知符号
}

// 计算产生式右部（符号序列）的First集
// First(X1 X2 ... Xn)
// 1. 将 First(X1) - {$} 加入结果
// 2. if $ in First(X1):
// 3.   将 First(X2) - {$} 加入结果
// 4.   if $ in First(X2):
// 5.     ...
// n.   if $ in First(Xn-1):
// n+1.   将 First(Xn) 加入结果
// n+2. if $ in First(X1) and ... and $ in First(Xn):
// n+3.   将 $ 加入结果
std::set<std::string> LL1Converter::computeFirstSetOfRightSide(
    const std::vector<std::string>& rightSide) {
    std::set<std::string> result;
    bool allHaveEpsilon = true;

    if (rightSide.empty() || (rightSide.size() == 1 && rightSide[0] == "$")) {
        result.insert("$");  // 空产生式或只包含 $
        return result;
    }

    for (const auto& symbol : rightSide) {
        std::set<std::string> symbolFirstSet = computeFirstSet(symbol);
        bool hasEpsilon = false;

        for (const auto& firstSymbol : symbolFirstSet) {
            if (firstSymbol == "$") {
                hasEpsilon = true;
            } else {
                result.insert(firstSymbol);
            }
        }

        if (!hasEpsilon) {
            allHaveEpsilon = false;
            break;  // 当前符号的First集不包含$，停止计算
        }
    }

    if (allHaveEpsilon) {
        result.insert("$");  // 所有符号的First集都包含 $
    }

    return result;
}

// --- Follow集计算 ---

// 计算所有非终结符的Follow集
void LL1Converter::computeFollowSets() {
    followSets.clear();
    bool changed = true;

    // 初始化Follow集
    for (const auto& nonTerminal : ll1Grammar.getNonTerminals()) {
        followSets[nonTerminal] = {};
    }
    // 1. 将 $ 加入开始符号的Follow集
    followSets[ll1Grammar.getStartSymbol()].insert("$");

    while (changed) {
        changed = false;
        for (const auto& prod : ll1Grammar.getProductions()) {
            const std::string& A = prod.left;  // A -> α B β
            const std::vector<std::string>& right = prod.right;

            for (size_t i = 0; i < right.size(); ++i) {
                const std::string& B = right[i];  // 当前符号 B

                if (ll1Grammar.isNonTerminal(B)) {
                    // 规则 2: A -> α B β
                    // Follow(B) += First(β) - {$}
                    std::vector<std::string> beta(right.begin() + i + 1,
                                                  right.end());
                    std::set<std::string> firstOfBeta =
                        computeFirstSetOfRightSide(beta);

                    size_t oldSize = followSets[B].size();
                    for (const auto& symbol : firstOfBeta) {
                        if (symbol != "$") {
                            followSets[B].insert(symbol);
                        }
                    }
                    if (followSets[B].size() > oldSize) {
                        changed = true;
                    }

                    // 规则 3: A -> α B or A -> α B β where First(β) contains $
                    // Follow(B) += Follow(A)
                    if (firstOfBeta.count("$") || beta.empty()) {
                        size_t oldSizeFollowB = followSets[B].size();
                        const std::set<std::string>& followA = followSets[A];
                        for (const auto& sym : followA) {
                            followSets[B].insert(sym);
                        }
                        if (followSets[B].size() > oldSizeFollowB) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

// --- 预测分析表构建 ---

// 构建预测分析表
// 对于每个产生式 A -> α:
// 1. 对于 First(α) 中的每个终结符 a:
//    Table[A, a] = A -> α
// 2. 如果 $ 在 First(α) 中:
//    对于 Follow(A) 中的每个终结符 b (包括 $):
//    Table[A, b] = A -> α
void LL1Converter::buildParseTable() {
    parseTable.clear();

    for (const auto& prod : ll1Grammar.getProductions()) {
        const std::string& A = prod.left;
        const std::vector<std::string>& alpha = prod.right;

        std::set<std::string> firstOfAlpha = computeFirstSetOfRightSide(alpha);

        // 规则 1
        for (const auto& terminal : firstOfAlpha) {
            if (terminal != "$") {
                if (parseTable.count({A, terminal})) {
                    // 冲突！同一个单元格填入了多个产生式
                    std::cerr << "错误: 预测分析表冲突! 在 M[" << A << ", "
                              << terminal << "] 处已存在产生式: ";
                    parseTable[{A, terminal}].print();
                    std::cerr << "尝试填入产生式: ";
                    prod.print();
                    // 可以选择抛出异常或标记错误
                }
                parseTable[{A, terminal}] = prod;
            }
        }

        // 规则 2
        if (firstOfAlpha.count("$")) {
            const std::set<std::string>& followOfA = followSets[A];
            for (const auto& terminal : followOfA) {
                // terminal 可能是终结符或 $
                if (parseTable.count({A, terminal})) {
                    // 冲突！
                    std::cerr << "错误: 预测分析表冲突! 在 M[" << A << ", "
                              << terminal << "] 处已存在产生式: ";
                    parseTable[{A, terminal}].print();
                    std::cerr << "尝试填入产生式: ";
                    prod.print();
                }
                parseTable[{A, terminal}] = prod;
            }
        }
    }
}

// 检查是否是LL1文法
// 条件：对于每个非终结符 A，其任意两个不同的产生式 A -> α 和 A -> β：
// 1. First(α) ∩ First(β) = ∅
// 2. 如果 $ ∈ First(α)，则 First(β) ∩ Follow(A) = ∅
// 3. 如果 $ ∈ First(β)，则 First(α) ∩ Follow(A) = ∅
// (注意：条件2和3可以合并，如果两个产生式都能推导出$，则必然不满足LL1)
// 简化的检查方法：检查预测分析表中是否有冲突
bool LL1Converter::isLL1Grammar() const {
    // 遍历预测分析表，检查是否有冲突（一个单元格有多个条目）
    // buildParseTable 函数中已经包含了冲突检测逻辑
    // 这里可以再次检查，或者依赖 buildParseTable 中的报错
    std::map<std::pair<std::string, std::string>, int> entryCount;
    for (const auto& entry : parseTable) {
        entryCount[entry.first]++;
    }
    for (const auto& count : entryCount) {
        if (count.second > 1) {
            std::cerr << "LL1检查失败：预测分析表在 M[" << count.first.first
                      << ", " << count.first.second << "] 处存在冲突。"
                      << std::endl;
            return false;
        }
    }
    return true;  // 如果没有冲突，则认为是LL1文法
}

// --- Getter方法 ---

const Grammar& LL1Converter::getOriginalGrammar() const {
    return originalGrammar;
}

const Grammar& LL1Converter::getLL1Grammar() const { return ll1Grammar; }

const std::map<std::string, std::set<std::string>>& LL1Converter::getFirstSets()
    const {
    return firstSets;
}

const std::map<std::string, std::set<std::string>>&
LL1Converter::getFollowSets() const {
    return followSets;
}

const std::map<std::pair<std::string, std::string>, Production>&
LL1Converter::getParseTable() const {
    return parseTable;
}

// --- 保存和打印方法 ---

bool LL1Converter::saveFirstSets(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    for (const auto& pair : firstSets) {
        if (ll1Grammar.isNonTerminal(pair.first)) {  // 只保存非终结符的First集
            file << "First(" << pair.first << ") = { ";
            for (const auto& symbol : pair.second) {
                file << symbol << " ";
            }
            file << "}" << std::endl;
        }
    }
    file.close();
    return true;
}

bool LL1Converter::saveFollowSets(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    for (const auto& pair : followSets) {
        file << "Follow(" << pair.first << ") = { ";
        for (const auto& symbol : pair.second) {
            file << symbol << " ";
        }
        file << "}" << std::endl;
    }
    file.close();
    return true;
}

bool LL1Converter::saveParseTable(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    // 获取所有非终结符和终结符（包括$）用于表头
    std::set<std::string> terminals = ll1Grammar.getTerminals();
    terminals.insert("$");
    std::vector<std::string> nonTerminals(ll1Grammar.getNonTerminals().begin(),
                                          ll1Grammar.getNonTerminals().end());
    std::sort(nonTerminals.begin(), nonTerminals.end());
    std::vector<std::string> terminalList(terminals.begin(), terminals.end());
    std::sort(terminalList.begin(), terminalList.end());

    // 打印表头
    file << "预测分析表:\n";
    file << "NonTerminal\t";
    for (const auto& term : terminalList) {
        file << term << "\t";
    }
    file << "\n";

    // 打印表内容
    for (const auto& nt : nonTerminals) {
        file << nt << "\t";
        for (const auto& term : terminalList) {
            if (parseTable.count({nt, term})) {
                const Production& prod = parseTable.at({nt, term});
                file << prod.left << " -> ";
                for (const auto& sym : prod.right) {
                    file << sym << " ";
                }
                file << "\t";
            } else {
                file << " \t";  // 空白表示错误条目
            }
        }
        file << "\n";
    }

    file.close();
    return true;
}

void LL1Converter::printFirstSets() const {
    std::cout << "First集:" << std::endl;
    for (const auto& pair : firstSets) {
        if (ll1Grammar.isNonTerminal(pair.first)) {  // 只打印非终结符的First集
            std::cout << "  First(" << pair.first << ") = { ";
            for (const auto& symbol : pair.second) {
                std::cout << symbol << " ";
            }
            std::cout << "}" << std::endl;
        }
    }
}

void LL1Converter::printFollowSets() const {
    std::cout << "Follow集:" << std::endl;
    for (const auto& pair : followSets) {
        std::cout << "  Follow(" << pair.first << ") = { ";
        for (const auto& symbol : pair.second) {
            std::cout << symbol << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void LL1Converter::printParseTable() const {
    std::cout << "预测分析表:" << std::endl;

    // 获取所有非终结符和终结符（包括$）用于表头
    std::set<std::string> terminals = ll1Grammar.getTerminals();
    terminals.insert("$");
    std::vector<std::string> nonTerminals(ll1Grammar.getNonTerminals().begin(),
                                          ll1Grammar.getNonTerminals().end());
    std::sort(nonTerminals.begin(), nonTerminals.end());
    std::vector<std::string> terminalList(terminals.begin(), terminals.end());
    std::sort(terminalList.begin(), terminalList.end());

    // 打印表头
    printf("%-15s", "NonTerminal");
    for (const auto& term : terminalList) {
        printf("%-15s", term.c_str());
    }
    printf("\n");

    // 打印表内容
    for (const auto& nt : nonTerminals) {
        printf("%-15s", nt.c_str());
        for (const auto& term : terminalList) {
            if (parseTable.count({nt, term})) {
                const Production& prod = parseTable.at({nt, term});
                std::stringstream ss;
                ss << prod.left << " -> ";
                for (const auto& sym : prod.right) {
                    ss << sym << " ";
                }
                printf("%-15s", ss.str().c_str());
            } else {
                printf("%-15s", " ");  // 空白表示错误条目
            }
        }
        printf("\n");
    }
}