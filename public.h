#pragma once
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

enum SymbolType
{
    TERMINAL,
    NON_TERMINAL
};

struct Symbol
{
    SymbolType type; // 是否为终结符
    string value;    // 非终结符的 符号； 终结符的： 符号
    string str = "NULL";      // 存放 NUM ID 类型的值
    int lines = 0;

    Symbol() :type(TERMINAL), value("#") {}
    Symbol(SymbolType t, string v) : type(t), value(v) {}
    Symbol(SymbolType t, string v, int l) : type(t), value(v), lines(l) {}
    Symbol(SymbolType t, string v,string s,int l) : type(t), value(v),str(s),lines(l) {}

    bool isTerminal() const { return type == TERMINAL; }

    bool operator==(const Symbol& other) const
    {
        return type == other.type && value == other.value;
    }

    bool operator!=(const Symbol& other) const
    {
        return !(*this == other);
    }
};

namespace std
{
    // 特化std::hash模板，使其能够处理Symbol类型。
    template <>
    struct hash<Symbol>
    {
        size_t operator()(const Symbol& s) const
        {
            return hash<string>()(s.value) ^ hash<int>()(s.type);
        }
    };
}

struct Production
{
    Symbol lhs;         //  产生式左侧
    vector<Symbol> rhs; // 产生式右侧

    // 默认构造函数
    Production() : lhs(NON_TERMINAL, "") {}
    Production(Symbol l, vector<Symbol> r) : lhs(l), rhs(r) {}
};

extern unordered_set<Symbol> terminals;
extern unordered_set<Symbol> nonTerminals;

string stackToStr(stack<Symbol> pS);
string queueToStr(queue<Symbol>& tok);
string ProductionToStr(const Production& pro);

#endif