#include "public.h"

unordered_set<Symbol> terminals;
unordered_set<Symbol> nonTerminals;

string stackToStr(stack<Symbol> pS)
{
    stack<Symbol> pStack(pS);
    string stackOSS = "";
    while (!pStack.empty())
    {
        stackOSS = pStack.top().value + stackOSS; // �����ַ���֮�䲻��Ҫ�ָ���
        pStack.pop();
    }
    return stackOSS;
}
string queueToStr(queue<Symbol>& tok)
{
    queue<Symbol> tokens(tok);
    // ����queue����������������ַ���
    string queueOSS = "";
    while (!tokens.empty())
    {
        queueOSS += tokens.front().value; // �����ַ���֮�䲻��Ҫ�ָ���
        tokens.pop();
    }

    return queueOSS;
}
string ProductionToStr(const Production& pro)
{
    string proStr = pro.lhs.value + "->";
    for (Symbol it : pro.rhs)
    {
        proStr += it.value;
    }

    return proStr;
}
