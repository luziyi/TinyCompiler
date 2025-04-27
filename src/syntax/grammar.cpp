#include "syntax/grammar.h"

Production splitProduction(const string &input, const string &delimiter) {
    Production result;

    size_t delimiterPos = input.find(delimiter);
    if (delimiterPos != string::npos) {
        result.left = input.substr(0, delimiterPos - 1);  // 提取左部
        string rightPart = input.substr(delimiterPos + delimiter.length() + 1);
        istringstream iss(rightPart);
        string token;

        while (iss >> token) {
            if (token != "$") {
                result.right.push_back(token);
            }
        }
    } else {
        result.left = input;
    }

    return result;
}

vector<Production> parseGrammar(const string &filename) {
    vector<Production> productions;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        return productions;
    }

    string line;
    while (getline(file, line)) {
        line.erase(line.find_last_not_of(" \r\n\t") + 1);  // 去除尾部空白字符

        if (line.empty()) continue;

        string arrow = "->";
        productions.push_back(splitProduction(line, arrow));
    }

    return productions;
}

void getSymbols(const vector<Production> &productions,
                unordered_set<string> &nonTerminal,
                unordered_set<string> &Terminal) {
    for (const auto &production : productions) {
        nonTerminal.insert(production.left);
    }
    for (const auto &production : productions) {
        for (const auto &symbol : production.right) {
            if (!nonTerminal.count(symbol)) {
                Terminal.insert(symbol);
            }
        }
    }
    Terminal.insert("#");
}
