#pragma once
#ifndef _LEXER_H_
#define _LEXER_H_

#include <map>
#include <cctype>
#include "public.h"

// 枚举关键字、标识符、运算符、分隔符和字面量的种别码
enum  KW_CODE {
    // 关键字
    KW_MAIN = 0, KW_ELSE, KW_IF, KW_INT, KW_RETURN, KW_VOID, KW_WHILE,
    // 标识符
    KW_ID,
    // 字面量
    KW_NUM,
    // 错误标识
    KW_ERR,
    // 专用符号 - 算术运算符
    KW_PLUS, KW_MINUS, KW_MULTIPLY, KW_DIVIDE,
    // 专用符号 - 关系运算符
    KW_LESS_THAN, KW_LESS_EQUAL, KW_GREATER_THAN, KW_GREATER_EQUAL, KW_EQUAL, KW_NOT_EQUAL,
    // 赋值运算符
    KW_ASSIGNMENT,
    // 其他符号
    KW_SEMICOLON, KW_COMMA, KW_LPAREN, KW_RPAREN, KW_LBRACKET, KW_RBRACKET, KW_LBRACE, KW_RBRACE,
    // 注释符号
    KW_COMMENT_START, KW_COMMENT_END,
    // 空 符号 Epsilon
    KW_EPSILON,
    // 结束标记
    KW_END_OF_ENUM
};

// token 结构体
struct token {
    KW_CODE token_type;
    string token_val;

    token();
    token(KW_CODE temp, string val);
};

// 词法分析器类
class Lexer {
public:
    Lexer();
    bool lexical_analyzer(const string inputFilePath, const string outputFilePath);
    void showAll(ostringstream& preprocessed);
    bool IsLetter(char ch);
    bool IsDigit(char ch);
    int scan(const string& allCodes, int codeLength);
    bool getTokensQueue(queue<Symbol>& tokensQ);
    bool getProductions(vector<Production>& productions, const string& inputFilePath);
    string preprocess(const string& inputFilename);

private:
    vector<token> tokens;
    queue<Symbol> tokensQueue;
    vector<vector<string>> vc_token;
    int cur_index;
    int cur_lines;
    int token_type;
    string token_str;
};

#endif // LEXER_H
