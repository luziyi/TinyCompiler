#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__
#include <iostream>
#include <string>
#include <unordered_map>

// 符号表输出文件路径
extern const char symbolTableTxtPath[];

struct Record {
    /* data */
    std::string name;  // 符号名
    void* value;       // 扩展属性指针
    Record(std::string name, void* value) : name(name), value(value) {}
};

class SymbolTable {
   private:
    /* data */
    std::unordered_map<std::string, Record*> Hash;

   public:
    SymbolTable() = default;
    ~SymbolTable() = default;
    void addSymbol(std::string name);
    void removeSymbol(std::string name);
    void* getSymbolValue(std::string name);
    bool lookupSymbol(std::string name);
    void printTable();
};

#endif