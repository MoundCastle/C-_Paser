#include "public.h"

unordered_set<Symbol> terminals;
unordered_set<Symbol> nonTerminals;

string stackToStr(stack<Symbol> pS)
{
    stack<Symbol> pStack(pS);
    string stackOSS = "";
    while (!pStack.empty())
    {
        stackOSS = pStack.top().value + stackOSS; // 假设字符串之间不需要分隔符
        pStack.pop();
    }
    return stackOSS;
}
string queueToStr(queue<Symbol>& tok)
{
    queue<Symbol> tokens(tok);
    // 对于queue，正向迭代并连接字符串
    string queueOSS = "";
    while (!tokens.empty())
    {
        queueOSS += tokens.front().value; // 假设字符串之间不需要分隔符
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
