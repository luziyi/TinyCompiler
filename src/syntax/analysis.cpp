#include "syntax/analysis.h"

vector<string> read_from_lexical() {
    fstream file("../data/lexical.txt");
    vector<string> input;
    if (!file.is_open()) {
        cout << "Fail to open file" << endl;
    } else {
        string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            string name, type;

            if (iss >> name >> type) {
                string keyword;
                // 数据解析成功，可以进一步处理
                size_t start = type.find(
                    "<");  // 查找 "<KW," 的位置，并加上 4，得到关键字的起始位置
                size_t end =
                    type.find(",", start);  // 在起始位置后查找 ">"
                                            // 的位置，得到关键字的结束位置

                if (start != std::string::npos && end != std::string::npos)
                    keyword =
                        type.substr(start + 1, end - start - 1);  // 提取关键字
                if (keyword == "INT") {
                    input.push_back("IntConst");
                } else if (keyword == "IDN") {
                    input.push_back("Ident");
                } else if (keyword == "FLOAT")
                    input.push_back("floatConst");
                else
                    input.push_back(name);
            }
        }
    }
    input.push_back("#");
    return input;
}

void analysis(vector<string> rest_string, vector<Production> &productions,
              vector<State> states, stack<int> &states_stack,
              stack<string> &symbol_stack, actionTable actiontable,
              gotoTable gototable) {
    int seq = 1;
    states_stack.push(0);  // 0号状态入栈
    symbol_stack.push("#");
    int index = 0;                            // 待识别的字符下标
    string next_symbol = rest_string[index];  // 下一个要读的字符
    State current_state = states[0];          // 当前状态
    int current_state_num = 0;                // 当前状态的序号

    // 创建输出文件流，重定向输出
    ofstream output_file("../data/syntax.txt");
    if (!output_file.is_open()) {
        cerr << "Error: Unable to open output file ../data/syntax.txt" << endl;
        return;
    }

    while (actiontable[current_state_num][next_symbol] != "acc" &&
           actiontable[current_state_num][next_symbol] != "error") {
        string action =
            actiontable[current_state_num][next_symbol];  // action表内容
        int goto_state =
            gototable[current_state_num][next_symbol];  // GOTO表内容

        output_file << seq << "    " << symbol_stack.top() << " # "
                    << next_symbol << "   ";

        // 处理移进动作
        if (action[0] == 's') {
            int state_num = stoi(action.substr(1));
            current_state_num = goto_state = state_num;
            symbol_stack.push(next_symbol);
            states_stack.push(goto_state);

            current_state = states[goto_state];
            next_symbol = rest_string[++index];
            output_file << "move" << endl;

            seq++;
        }
        // 处理规约动作
        else if (action[0] == 'r') {
            int production_num =
                stoi(action.substr(1));  // 规约使用的产生式序号
            Production production =
                productions[production_num];          // 规约使用的产生式
            int right_len = production.right.size();  // 产生式右部长度
            string left_symbol = production.left;     // 产生式左部符号

            // 弹出栈中的符号和状态
            while (right_len--) {
                symbol_stack.pop();
                states_stack.pop();
            }

            int top_state_num = states[states_stack.top()].stateNumber;
            goto_state = gototable[top_state_num][left_symbol];  // GOTO转移

            if (goto_state == -1) {
                output_file << "error" << endl;
                break;
            }

            states_stack.push(goto_state);
            symbol_stack.push(left_symbol);
            current_state_num = goto_state;
            current_state = states[current_state_num];
            output_file << "reduction" << endl;

            seq++;
        }
    }

    // 处理最终状态
    string final_action = actiontable[current_state_num][next_symbol];
    if (final_action == "acc") {
        seq++;
        output_file << seq << "    " << symbol_stack.top() << " # "
                    << next_symbol << "   accept" << endl;
    } else if (final_action == "error") {
        seq++;
        output_file << seq << "    " << symbol_stack.top() << " # "
                    << next_symbol << "   error" << endl;
    }

    output_file.close();
    cout << "语法分析结果已写入: ../data/syntax.txt" << endl;
}