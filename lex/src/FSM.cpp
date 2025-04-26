#include "../include/FSM.h"

#include "../include/TokenCodes.h"
#include "../include/util.h"

// FSM_Node构造函数实现
FSM_Node::FSM_Node(int id, int state, bool isEndState) {
    this->id = id;
    this->state = state;
    this->isEndState = isEndState;
}

FSM::FSM(std::set<std::set<char>> charList) {
    this->charList = charList;
    this->num = 0;
    FSM_Node* firstNode = new FSM_Node(this->num, TokenCode::UNDIFNIE, false);
    this->nodes.push_back(firstNode);
    this->index.insert({firstNode->id, firstNode});
    this->first = firstNode;
    this->num++;
}

void FSM::addNode(FSM_Node* node) {
    this->nodes.push_back(node);
    this->index.insert({node->id, node});
    this->num++;
}

void FSM::addTrans(int from, int to, std::set<char> chSet) {
    FSM_Node* fromNode = this->index[from];
    FSM_Node* toNode = this->index[to];
    if (fromNode->trans.count(chSet)) {
        fromNode->trans[chSet].push_back(toNode);
    } else {
        std::vector<FSM_Node*> v;
        v.push_back(toNode);
        fromNode->trans.insert({chSet, v});
    }
}

FSM_Node* FSM::getFisrt() { return this->first; }

FSM_Node* FSM::getNode(int id) {
    if (this->index.count(id) > 0) {
        return this->index[id];
    } else {
        return nullptr;
    }
}

std::set<std::set<char>> FSM::getCharList() { return this->charList; }

int FSM::getNum() { return this->num; }

// 该函数暂时仅用于debug
void FSM::printFSM() {
    std::cout << this->num << std::endl;
    std::cout
        << "================================================================"
        << std::endl;
    for (int i = 0; i < this->num; i++) {
        FSM_Node* node = this->index[i];
        std::cout << "num:" << node->id << std::endl;
        if (node->isEndState) {
            std::cout << "END STATE!" << std::endl;
        }
        if (node->trans.count(getLetterList('a'))) {
            std::vector<FSM_Node*> v = node->trans[getLetterList('a')];
            for (FSM_Node* node : v) {
                std::cout << "字母 -> " << node->id << std::endl;
            }
        }
        if (node->trans.count(getLetterList('0'))) {
            std::vector<FSM_Node*> v = node->trans[getLetterList('0')];
            for (FSM_Node* node : v) {
                std::cout << "数字0 -> " << node->id << std::endl;
            }
        }
        if (node->trans.count(getLetterList('1'))) {
            std::vector<FSM_Node*> v = node->trans[getLetterList('1')];
            for (FSM_Node* node : v) {
                std::cout << "数字1-9 -> " << node->id << std::endl;
            }
        }
        std::cout
            << "=============================================================="
               "=="
            << std::endl;
    }
}

FSM createNFA() {
    std::set<std::set<char>> charList;
    std::set<char> letterList = {'_'};
    for (char c = 'a'; c <= 'z'; c++) {
        letterList.insert(c);
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        letterList.insert(c);
    }
    charList.insert(letterList);
    std::set<char> numList;
    for (char c = '1'; c <= '9'; c++) {
        numList.insert(c);
    }
    charList.insert(numList);
    std::set<char> zeroList = {'0'};
    charList.insert(zeroList);
    std::set<char> dotList = {'.'};  // 添加小数点字符集
    charList.insert(dotList);
    FSM NFA = FSM(charList);

    NFA.addNode(new FSM_Node(1, TokenCode::INT, true));        // 只有一个数字
    NFA.addNode(new FSM_Node(2, TokenCode::UNDIFNIE, false));  // 过渡节点
    NFA.addNode(new FSM_Node(3, TokenCode::INT, true));        // 数字
    NFA.addNode(new FSM_Node(4, TokenCode::IDN, true));        // 只有一个字母
    NFA.addNode(new FSM_Node(5, TokenCode::UNDIFNIE, false));  // 过渡节点
    NFA.addNode(new FSM_Node(6, TokenCode::IDN, true));        // 标识符
    NFA.addNode(new FSM_Node(7, TokenCode::UNDIFNIE, false));  // 小数点后的过渡状态
    NFA.addNode(new FSM_Node(8, TokenCode::FLOAT, true));      // 浮点数终态

    NFA.addTrans(0, 1, zeroList);
    NFA.addTrans(0, 1, numList);
    NFA.addTrans(0, 2, numList);
    NFA.addTrans(2, 2, zeroList);
    NFA.addTrans(2, 2, numList);
    NFA.addTrans(2, 3, zeroList);
    NFA.addTrans(2, 3, numList);

    // 添加浮点数识别的转换规则
    NFA.addTrans(1, 7, dotList);    // 单个数字后接小数点
    NFA.addTrans(3, 7, dotList);    // 多个数字后接小数点
    NFA.addTrans(7, 8, zeroList);   // 小数点后至少一个数字
    NFA.addTrans(7, 8, numList);
    NFA.addTrans(8, 8, zeroList);   // 小数点后可以有多个数字
    NFA.addTrans(8, 8, numList);

    NFA.addTrans(0, 4, letterList);
    NFA.addTrans(0, 5, letterList);
    NFA.addTrans(5, 5, letterList);
    NFA.addTrans(5, 5, zeroList);
    NFA.addTrans(5, 5, numList);
    NFA.addTrans(5, 6, zeroList);
    NFA.addTrans(5, 6, numList);
    NFA.addTrans(5, 6, letterList);
    return NFA;
}

FSM NFAtoDFA(FSM NFA) {
    FSM DFA = FSM(NFA.getCharList());

    std::stack<std::set<int>> nodes;  // 遍历产生新节点的栈
    std::map<std::set<int>, int>
        statesMap;  // 存储当前的eplision-closure(move(I,a))对应的DFA中的标号，避免重复
    std::set<int> nowStates;  // 当前NFA状态集的标号集合
    nowStates.insert(NFA.getFisrt()->id);
    statesMap.insert({nowStates, DFA.getFisrt()->id});
    nodes.push(nowStates);
    int newNum = 0;
    while (!nodes.empty()) {
        nowStates = nodes.top();
        nodes.pop();
        for (int id : nowStates) {
            FSM_Node* node = NFA.getNode(id);
            for (std::set<char> chList : NFA.getCharList()) {
                if (node->trans.count(chList)) {  // 通过当前符号集有边能够转移
                    std::set<int> temp;           // 当前状态集转移到的I的标号集
                    int tokenCode = TokenCode::UNDIFNIE;
                    bool isEnd = false;
                    for (unsigned int i = 0; i < node->trans[chList].size();
                         i++) {
                        FSM_Node* toNode =
                            node->trans[chList][i];  // 当前转换到的
                        if (toNode->state != TokenCode::UNDIFNIE) {
                            tokenCode = toNode->state;
                        }
                        if (toNode->isEndState) {
                            isEnd = true;
                        }
                        int toId = toNode->id;
                        temp.insert(toId);
                    }
                    if (statesMap.count(temp) ==
                        0) {  // 到达了一个新的状态节点！
                        FSM_Node* newNode =
                            new FSM_Node(++newNum, tokenCode, isEnd);
                        statesMap.insert({temp, newNode->id});
                        DFA.addNode(newNode);
                        DFA.addTrans(statesMap[nowStates], newNode->id, chList);
                        nodes.push(temp);  // 新发现的状态集需要加入栈中进行遍历
                    } else {               // 之前就存在的状态集，添加边即可
                        int toId = statesMap[temp];
                        DFA.addTrans(statesMap[nowStates], toId, chList);
                    }
                }
            }
        }
    }
    return DFA;
}

FSM minimizeDFA(FSM DFA) {
    std::set<std::set<int>> partition;    // 状态的划分
    std::set<int> endState, notEndState;  // 初始两个状态
    for (int i = 0; i < DFA.getNum(); i++) {
        if (DFA.getNode(i)->isEndState) {
            endState.insert(i);
        } else {
            notEndState.insert(i);
        }
    }
    partition.insert(endState);
    partition.insert(notEndState);
    bool isChanged = true;
    while (isChanged) {  // 循环划分
        isChanged = false;
        for (std::set<int> state : partition) {  // 遍历每一个集合
            if (state.size() == 1) {
                continue;
            }
            for (std::set<char> inputSet :
                 DFA.getCharList()) {    // 对每种输入都进行尝试
                std::map<int, int> m;    // 记录每一个节点达到的最终节点对应关系
                std::set<int> emptySet;  // 当前输入不能转换的节点集合
                for (int id : state) {   // 当前输入需要对每个状态都进行尝试
                    FSM_Node* node = DFA.getNode(id);
                    if (node->trans.count(inputSet)) {
                        while (node->trans.count(inputSet)) {
                            node = node->trans[inputSet][0];
                            if (node->isEndState) {
                                break;  // 到达终态就可以退出了，否则会死循环捏
                            }
                        }  // 此时node是当前id能一直转换到的节点
                        int toId = node->id;
                        m.insert({id, toId});
                    } else {  // 当前输入无法转换，单独落入一个集合
                        emptySet.insert(id);
                    }
                }

                std::map<int, std::set<int>>
                    tempMap;  // 当前id与其终态点划分的对应关系
                for (int id : state) {
                    if (emptySet.count(id)) {
                        continue;
                    }
                    int toId = m[id];
                    for (std::set<int> nowStateSet :
                         partition) {  // 去找当前这个id落在了哪个划分里
                        if (nowStateSet.count(toId)) {
                            tempMap.insert({id, nowStateSet});
                            break;
                        }
                    }
                }  // 到现在tempMap已经存储了该state里的id分类

                std::map<std::set<int>, std::set<int>>
                    partToId;  // 上面这个tempMap的反向操作
                for (int id : state) {
                    if (emptySet.count(id)) {
                        continue;
                    }
                    if (partToId.count(tempMap[id]) == 0) {
                        std::set<int> s;
                        s.insert(id);
                        partToId.insert({tempMap[id], s});
                    } else {
                        partToId[tempMap[id]].insert(id);
                    }
                }  // 到现在partToId已经把state里的id分好类了

                if ((emptySet.size() && partToId.size() >= 1) ||
                    (emptySet.size() == 0 && partToId.size() > 1)) {
                    isChanged = true;
                    partition.erase(state);
                    std::map<std::set<int>, std::set<int>>::iterator iter;
                    for (iter = partToId.begin(); iter != partToId.end();
                         iter++) {
                        partition.insert(iter->second);
                    }
                    partition.insert(emptySet);
                    break;
                }
            }
            if (isChanged) {
                break;
            }
        }
    }  // 到现在完成了划分，下面要根据这些划分构造新的miniDFA
    FSM miniDFA = FSM(DFA.getCharList());
    for (std::set<int> s : partition) {  // 先设置好所有的点
        if (s.count(miniDFA.getFisrt()->id)) {
            continue;
        }
        miniDFA.addNode(new FSM_Node(*s.begin(), DFA.getNode(*s.begin())->state,
                                     DFA.getNode(*s.begin())->isEndState));
    }
    for (std::set<int> s : partition) {  // 为每个点添加边
        FSM_Node* node;                  // node是当前划分的代表
        if (s.count(miniDFA.getFisrt()->id)) {
            node = miniDFA.getFisrt();
        } else {
            node = miniDFA.getNode(*s.begin());
        }
        for (int id : s) {  // 遍历当前划分的每个点
            FSM_Node* rawNode = DFA.getNode(id);
            std::map<std::set<char>, std::vector<FSM_Node*>>::iterator iter;
            // 遍历这个点的每条边
            for (iter = rawNode->trans.begin(); iter != rawNode->trans.end();
                 iter++) {
                if (node->trans.count(
                        iter->first)) {  // 因为是DFA，如果有转换了就有边了，不用再往下执行了
                    continue;
                }
                int rawToId = iter->second[0]->id;  // 原DFA中这个节点的id
                int newToId = rawToId;
                for (std::set<int> tempS : partition) {
                    if (tempS.count(rawToId)) {
                        newToId = *tempS.begin();  // 找到划分的代表
                        break;
                    }
                }
                miniDFA.addTrans(node->id, newToId, iter->first);
            }
        }
    }
    return miniDFA;
}

int identity(FSM DFA, std::string token) {
    FSM_Node* node = DFA.getFisrt();
    bool success = true;
    for (unsigned int i = 0; i < token.length(); i++) {
        char c = token[i];
        if (node->trans.count(getLetterList(c))) {
            node = node->trans[getLetterList(c)][0];
        } else {
            success = false;
        }
    }
    if (success && node->isEndState) {
        return node->state;
    } else {
        return TokenCode::UNDIFNIE;
    }
}