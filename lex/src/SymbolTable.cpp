#include "../include/SymbolTable.h"

// 定义符号表输出文件路径
const char symbolTableTxtPath[] = "./result/symbolTable.txt";

void SymbolTable::addSymbol(std::string name) {
    if (!this->lookupSymbol(name)) {
        this->Hash.insert(std::make_pair(name, new Record(name, nullptr)));
    } else {
        return;
    }
}

void SymbolTable::removeSymbol(std::string name) {
    if (this->lookupSymbol(name)) {
        this->Hash.erase(name);
    } else {
        return;
    }
}

void* SymbolTable::getSymbolValue(std::string name) {
    Record* record = this->Hash.find(name)->second;
    return record->value;
}

bool SymbolTable::lookupSymbol(std::string name) {
    if (this->Hash.find(name) != this->Hash.end()) {
        return true;
    } else {
        return false;
    }
}

void SymbolTable::printTable() {
    FILE* fp = freopen(symbolTableTxtPath, "w", stdout);
    std::cout << "name" << "\t" << "value" << std::endl;
    std::unordered_map<std::string, Record*>::iterator it;
    for (it = this->Hash.begin(); it != this->Hash.end(); it++) {
        std::string name = it->first;
        void* value = it->second;
        std::cout << name << "\t" << value << std::endl;
    }
    fflush(fp);
    fclose(fp);
    freopen("CON", "w", stdout);
}
