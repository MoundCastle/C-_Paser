#ifndef _PASER_H_
#define _PASER_H_

#include "public.h"

class Paser
{
public:
    Paser();
    Paser(string startSymbol);
    ~Paser();
    void calculateFirst(const vector<Production>& productions);

    void calculateFollow(const vector<Production>& productions, const string& startSymbol);

    void constructParseTable(const vector<Production>& productions);

    void constructSelectTable(const vector<Production>& productions);

    bool parse(const vector<Production>& productions, const string& startSymbol, queue<Symbol>& tokens);
    bool isLL();

    bool getTermnalANDNontermnal(vector<Production>& productions);

    bool getTokens(queue<Symbol>& tokens);

    void setOutputFilePath(string outputFP);
    void printParseTable();
    void printProductions(vector<Production>& productions);
    void printFirst();
    void printFollow();
    bool storeToFile(vector<Production>& productions);

private:
    string startSymbol;
    string outputFilePath;
    ostringstream preprocessed; // 输出流 用以暂存数据 最后存放到 输出文件中
    unordered_map<Symbol, unordered_set<Symbol>> firstSets;
    unordered_map<Symbol, unordered_set<Symbol>> followSets;
    unordered_map<string, unordered_set<Symbol>> alphaFirstSets;
    unordered_map<Symbol, unordered_map<Symbol, Production>> parseTable;
    unordered_map<Symbol, unordered_map<string, unordered_set<Symbol>>> selectTable;
};

#endif