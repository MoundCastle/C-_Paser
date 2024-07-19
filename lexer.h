#pragma once
#ifndef _LEXER_H_
#define _LEXER_H_

#include <map>
#include <cctype>
#include "public.h"

// ö�ٹؼ��֡���ʶ������������ָ��������������ֱ���
enum  KW_CODE {
    // �ؼ���
    KW_MAIN = 0, KW_ELSE, KW_IF, KW_INT, KW_RETURN, KW_VOID, KW_WHILE,
    // ��ʶ��
    KW_ID,
    // ������
    KW_NUM,
    // �����ʶ
    KW_ERR,
    // ר�÷��� - ���������
    KW_PLUS, KW_MINUS, KW_MULTIPLY, KW_DIVIDE,
    // ר�÷��� - ��ϵ�����
    KW_LESS_THAN, KW_LESS_EQUAL, KW_GREATER_THAN, KW_GREATER_EQUAL, KW_EQUAL, KW_NOT_EQUAL,
    // ��ֵ�����
    KW_ASSIGNMENT,
    // ��������
    KW_SEMICOLON, KW_COMMA, KW_LPAREN, KW_RPAREN, KW_LBRACKET, KW_RBRACKET, KW_LBRACE, KW_RBRACE,
    // ע�ͷ���
    KW_COMMENT_START, KW_COMMENT_END,
    // �� ���� Epsilon
    KW_EPSILON,
    // �������
    KW_END_OF_ENUM
};

// token �ṹ��
struct token {
    KW_CODE token_type;
    string token_val;

    token();
    token(KW_CODE temp, string val);
};

// �ʷ���������
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
