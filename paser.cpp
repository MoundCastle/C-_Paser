#include "paser.h"

Paser::Paser()
{
	startSymbol = "pragram";
	outputFilePath = "";
};
Paser::~Paser()
{

}
Paser::Paser(string startSymbol)
{
	this->startSymbol = startSymbol;
};

void Paser::calculateFirst(const vector<Production>& productions)
{
	// 初始化 first 集为空
	for (const auto& symbol : nonTerminals)
	{
		firstSets[symbol] = {};
	}
	for (const auto& symbol : terminals)
	{
		firstSets[symbol] = { symbol };
	}

	bool changed;
	do
	{
		changed = false;
		for (const auto& production : productions)
		{
			const auto& lhs = production.lhs; // 左侧
			const auto& rhs = production.rhs; // 右侧们
			string alpha = "";
			for (const auto& symbol : rhs) { alpha += symbol.value; }
			alphaFirstSets[alpha] = {};

			size_t before = firstSets[lhs].size();
			int RisEp = 0; // 右侧 非终结符号 含有 EPsilon 空字符  的个数

			for (const auto& symbol : rhs) // 遍历右侧 符号
			{
				// 右侧 是终结符 加入 first  规则二 、三 （ @ Epsilon也是终结符）。跳出
				if (symbol.type == TERMINAL)
				{
					firstSets[lhs].insert(symbol);
					alphaFirstSets[alpha].insert(symbol);
					break;
				}
				else
				{
					bool hasEsilon = false;
					for (const auto& it : firstSets[symbol])
					{
						if (it == Symbol(TERMINAL, "@"))
						{
							RisEp += 1;
							hasEsilon = true;
						}
						else
						{
							firstSets[lhs].insert(it);
							alphaFirstSets[alpha].insert(symbol);
						}
					}

					if (!hasEsilon)
					{
						// 右侧 不都含有Epsilon   此非终结符不含有 Epsilon 跳出
						break;
					}
				}
			}

			if (RisEp == rhs.size() && RisEp != 0) // 右侧 非终结符号 都含有 EPsilon 空字符
			{
				firstSets[lhs].insert(Symbol(TERMINAL, "@"));
				alphaFirstSets[alpha].insert(Symbol(TERMINAL, "@"));
			}

			if (firstSets[lhs].size() > before)
			{
				changed = true;
			}
		}
	} while (changed);
}

void Paser::calculateFollow(const vector<Production>& productions, const string& startSymbol)
{
	// 初始化 follow 集为空
	for (const auto& symbol : nonTerminals)
	{
		followSets[symbol] = {};
	}
	// 规则1 文法的开始符号S，置$于FOLLOW(S)中
	followSets[Symbol(NON_TERMINAL, startSymbol)].insert(Symbol(TERMINAL, "$"));

	bool changed;
	do
	{
		changed = false;
		for (const auto& production : productions)
		{
			const auto& lhs = production.lhs;
			const auto& rhs = production.rhs;

			// 遍历产生式的右部，为每个非终结符添加FOLLOW集
			for (size_t i = 0; i < rhs.size(); ++i)
			{
				if (rhs[i].type == NON_TERMINAL)
				{
					size_t before = followSets[rhs[i]].size();

					// 规则 2 若A→A1BA2是文法的一个产生式，则把FIRST(A1) \ {Epsilon}加至FOLLOW(B)中
					if (i + 1 < rhs.size()) // 不是产生式最后一个 符号
					{
						bool allEpsilon = true;
						for (size_t j = i + 1; j < rhs.size(); ++j)
						{
							allEpsilon = false;
							if (rhs[j].type == TERMINAL)
							{
								followSets[rhs[i]].insert(rhs[j]);
								break;
							}
							else
							{
								for (const auto& it : firstSets[rhs[j]])
								{
									if (it == Symbol(TERMINAL, "@"))
									{
										allEpsilon = true;
									}
									else
									{
										followSets[rhs[i]].insert(it);
									}
								}
							}

							if (!allEpsilon)
							{
								break;
							}
							else if (j == rhs.size() - 1)
							{
								// 如果当前非终结符的后面终结符都可以取 @  则将产生式左部的FOLLOW集添加到当前非终结符的FOLLOW集中
								// followSets[rhs[i]].insert(followSets[lhs].begin(), followSets[lhs].end());
								for (const auto& it : followSets[lhs])
								{
									followSets[rhs[i]].insert(it);
								}
							}
						}
					}
					else
					{
						// 如果当前非终结符是产生式右部的最后一个符号
						// 则将产生式左部的FOLLOW集添加到当前非终结符的FOLLOW集中
						// followSets[rhs[i]].insert(followSets[lhs].begin(), followSets[lhs].end());
						for (const auto& it : followSets[lhs])
						{
							followSets[rhs[i]].insert(it);
						}
					}

					// 检查FOLLOW集是否发生了变化
					if (followSets[rhs[i]].size() > before)
					{
						changed = true;
					}
				}
			}
		}
	} while (changed);
}

void Paser::constructSelectTable(const vector<Production>& productions)
{
	// 遍历所有的产生式
	for (const auto& production : productions)
	{
		const auto& lhs = production.lhs; // 获取产生式左部（非终结符）
		const auto& rhs = production.rhs; // 获取产生式右部（符号序列）
		string alpha = "";
		for (const auto& symbol : rhs) { alpha += symbol.value; }

		// 获取产生式右部的FIRST集
		unordered_set<Symbol> firstRhs = alphaFirstSets[alpha];

		//​A→α 同时 ε∈/ FIRST(α)此时SELECT(A→α) = { FIRST(α)− { ε } }
		for (const auto& it : firstRhs) {
			if (it.value != "@") {
				selectTable[lhs][alpha].insert(it);
			}
		}
		// A→α 同时 ε∈FIRST(α)此时SELECT(A→α)={FIRST(α)−{ε}}∪FOLLOW(A)
		if (firstRhs.find(Symbol(TERMINAL, "@")) != firstRhs.end())
		{
			firstRhs = followSets[lhs];
			for (const auto& it : firstRhs) {
				selectTable[lhs][alpha].insert(it);
			}
		}
	}
}

void Paser::constructParseTable(const vector<Production>& productions)
{
	constructSelectTable(productions); // 构建 SELECT 集
	// 初始化预测分析表为空
	// 对每个非终结符和终结符的组合初始化预测分析表
	for (const auto& nonTerminal : nonTerminals)
	{
		for (const auto& terminal : terminals)
		{
			// 将预测分析表的每个位置初始化为空产生式
			parseTable[nonTerminal][terminal] = Production(Symbol(TERMINAL, "ERROR"), {});
		}
	}

	// 遍历所有的产生式
	for (const auto& production : productions)
	{
		const auto& lhs = production.lhs; // 获取产生式左部（非终结符）
		const auto& rhs = production.rhs; // 获取产生式右部（符号序列）

		string algh = lhs.value + "->";
		for (const auto& it : rhs)
		{
			algh += it.value + " ";
		}

		bool allEpsilon = false;

		// 获取产生式右部符号的FIRST集
		unordered_set<Symbol> firstRhs = firstSets[rhs[0]];

		// 判断当前符号有无包含 空
		allEpsilon = (firstSets[rhs[0]].count(Symbol(TERMINAL, "@")) > 0);
		for (int i = 1; i < rhs.size(); i++)
		{
			if (allEpsilon)
			{ // 之前的符号 first 集合都含有 空 Epsilon
				firstRhs.insert(firstSets[rhs[i]].begin(), firstSets[rhs[i]].end());
				allEpsilon = (firstSets[rhs[i]].count(Symbol(TERMINAL, "@")) > 0);
			}
			else
			{
				break;
			}
		}

		// 对First( α )中的每个元素a，都将A→ α添加到M[A,a]中。
		for (const auto& it : firstRhs)
		{
			if (it.value != "@") //  预测分析表没有 Epsilon
			{
				// 是否 分析表  重复填入
				if (!parseTable[lhs][it].rhs.empty())
				{
					cout << "ERROR: Re " << algh << "328: Termnal: " << it.value << endl;
				}
				else
				{
					parseTable[lhs][it] = production;
				}
			}
		}

		// 遍历完 rhs 后 判断是否都含 空 Epsilonn
		if (allEpsilon)
		{
			// ε 在First( α）中，则对于Follow(A)的每个元素a，都将A→ε 添加到M[A,a]中
			const auto& followLhs = followSets[lhs];
			for (const auto& it : followLhs)
			{
				// 是否 重复填入
				if (!parseTable[lhs][it].rhs.empty())
				{
					cout << "ERROR: Re " << algh << " 346: Termnal: " << it.value << endl;
				}
				else
				{
					parseTable[lhs][it] = production;
				}
			}
		}
	}
}

bool Paser::parse(const vector<Production>& productions, const string& startSymbol, queue<Symbol>& tokens)
{
	stack<Symbol> parseStack;
	int step = 1;
	parseStack.push(Symbol(TERMINAL, "$"));
	parseStack.push(Symbol(NON_TERMINAL, startSymbol));
	cout << "开始语法分析：  步骤   符号栈   剩余输入串    所用产生式" << endl;
	preprocessed << "开始语法分析：  步骤   符号栈   剩余输入串    所用产生式" << endl;
	while (!parseStack.empty())
	{
		Symbol top = parseStack.top();
		parseStack.pop();
		if (tokens.empty())
		{
			cout << "tokens 空了" << endl;
			preprocessed << "tokens 空了" << endl;
			return false;
		}

		Symbol head = tokens.front();

		if (top.type == TERMINAL)
		{
			if (top.value == head.value)
			{
				tokens.pop();
				string stackStr = stackToStr(parseStack);
				string queueStr = queueToStr(tokens);
				cout << (step++) << "\t" << stackStr << top.value << "\t" << queueStr << "\t 匹配项:" << top.value << endl;
				preprocessed << (step) << "\t" << stackStr << top.value << "\t" << queueStr << "\t 匹配项:" << top.value << endl;
				if (top.value == "$" && parseStack.empty())
				{
					if (tokens.empty())
					{
						cout << "OVER GOOD !!" << endl;
						preprocessed << "OVER GOOD !!" << endl;
						break;
					}
					cout << "符号栈空了，词素队列没空" << endl;
					preprocessed << "符号栈空了，词素队列没空" << endl;
					break;
				}
			}
			else
			{
				if (top.value == "$" && parseStack.empty())
				{
					cout << " 输入未完成全部匹配。匹配到第 " << head.lines << "行" << endl;
					preprocessed << " 输入未完成全部匹配。匹配到第 " << head.lines << "行" << endl;
				}
				cout << " Error: Expected " << top.value << " but found " << head.value << "在 " << head.lines << "行" << endl;
				preprocessed << " Error: Expected " << top.value << " but found " << head.value << "在 " << head.lines << "行" << endl;
				return false;
			}
		}
		else
		{
			bool allEpsilon = false;

			// 查预测分析表  parseTable[A][a]  ->  production
			if (parseTable[top].find(head) != parseTable[top].end())
			{
				const auto& production = parseTable[top][head];

				string proStr = ProductionToStr(production);
				string stackStr = stackToStr(parseStack);
				string queueStr = queueToStr(tokens);
				cout << (step++) << "\t" << stackStr << top.value << "\t" << queueStr << "\t 产生式:" << proStr << endl;
				preprocessed << (step) << "\t" << stackStr << top.value << "\t" << queueStr << "\t 产生式:" << proStr << endl;

				if (production.rhs.empty())
				{
					cout << "Terrible Input! " << " Error: Expected " << top.value << " but found " << head.value << " 在 " << head.lines << "行" << endl;
					preprocessed << "Terrible Input! " << " Error: Expected " << top.value << " but found " << head.value << " 在 " << head.lines << "行" << endl;
					return false;
				}

				for (auto it = production.rhs.rbegin(); it != production.rhs.rend(); ++it)
				{
					// allEpsilon = (it->value == "@");
					if (it->value != "@")
					{
						parseStack.push(*it);
					}
				}
			}
			else
			{
				cout << "Error: No production for " << top.value << " with lookahead " << head.value << "在 " << head.lines << "行" << endl;
				preprocessed << "Error: No production for " << top.value << " with lookahead " << head.value << "在 " << head.lines << "行" << endl;
				return false;
			}
		}
	}

	return tokens.empty();
}

bool Paser::isLL()
{
	for (const auto& it : selectTable)
	{
		for (const auto& term : terminals) {
			int count = 0;
			string tmpstr = "";
			for (const auto& fit : it.second) {
				if (fit.second.find(term) != fit.second.end()) {
					count += 1;
					tmpstr += fit.first;
					tmpstr += " | ";
				}
			}
			if (count > 1) {
				cout << "\nNOT LL(1)  : " << it.first.value<< "=>" << tmpstr << endl;
				cout << endl;
				return false;
			}
		}
	}
	cout << "\nIS LL(1) \n\n " << endl;
	return true;
}

bool Paser::getTermnalANDNontermnal(vector<Production>& productions)
{
	for (const auto& production : productions)
	{
		nonTerminals.insert(production.lhs);
		string alpha = "";
		for (const auto& symbol : production.rhs)
		{
			if (symbol.isTerminal())
			{
				terminals.insert(symbol);
			}
			else
			{
				nonTerminals.insert(symbol);
			}
			alpha += symbol.value;
		}
		selectTable[production.lhs][alpha] = {};
	}
	// $ 符号 是follow 加的，预测分析表也使用
	terminals.insert(Symbol(TERMINAL, "$"));

	return true;
}

bool Paser::getTokens(queue<Symbol>& tokens)
{

	tokens.push(Symbol(TERMINAL, "a", 2));
	tokens.push(Symbol(TERMINAL, "b", 4));
	tokens.push(Symbol(TERMINAL, "a", 4));
	tokens.push(Symbol(TERMINAL, "a", 5));
	tokens.push(Symbol(TERMINAL, "b", 6));
	//tokens.push(Symbol(TERMINAL, "ID", 1));
	//tokens.push(Symbol(TERMINAL, "+", 2));
	//tokens.push(Symbol(TERMINAL, "ID", 4));
	//tokens.push(Symbol(TERMINAL, "*", 4));
	//tokens.push(Symbol(TERMINAL, "ID", 5));

	tokens.push(Symbol(TERMINAL, "$", 6));

	return true;
}

void Paser::printParseTable()
{
	if (terminals.find(Symbol(TERMINAL, "@")) != terminals.end())
	{
		terminals.erase(Symbol(TERMINAL, "@"));
	}

	preprocessed << "\nParse Table:" << endl;

	for (const auto& nonTerminal : nonTerminals)
	{
		for (const auto& terminal : terminals)
		{
			const auto& production = parseTable[nonTerminal][terminal];
			if (!production.rhs.empty())
			{
				preprocessed << setw(20) << nonTerminal.value << " => ";
				preprocessed << setw(6) << terminal.value << " :";
				preprocessed << setw(30) << production.lhs.value << " -> ";
				for (const auto& symbol : production.rhs)
				{
					preprocessed << symbol.value << " ";
				}
				preprocessed << endl;
			}
		}
	}
}
void Paser::printProductions(vector<Production>& productions)
{
	preprocessed << "\nProductions:" << endl;
	for (const auto& it : productions)
	{
		preprocessed << it.lhs.value << " => ";
		for (const auto& itt : it.rhs)
		{
			preprocessed << itt.value << " ";
		}
		preprocessed << endl;
	}

	preprocessed << "\nProductions:WITH terminal or not : 1 非终结符； 0 终结符" << endl;
	for (const auto& it : productions)
	{
		preprocessed << it.lhs.type << it.lhs.value << " => ";
		for (const auto& itt : it.rhs)
		{
			preprocessed << itt.type << itt.value << " ";
		}
		preprocessed << endl;
	}
}
void Paser::printFirst()
{
	preprocessed << "\nFirst():" << endl;
	for (const auto& it : firstSets)
	{
		preprocessed << it.first.value << ": ";
		for (const auto& itt : it.second)
		{
			preprocessed << itt.value << " ";
		}
		preprocessed << endl;
	}
}
void Paser::printFollow()
{
	preprocessed << "\nFollow():" << endl;
	for (const auto& it : followSets)
	{
		preprocessed << it.first.value << ": ";
		for (const auto& itt : it.second)
		{
			preprocessed << itt.value << " ";
		}
		preprocessed << endl;
	}
}
void Paser::setOutputFilePath(string outputFP)
{
	this->outputFilePath = outputFP;
}
bool Paser::storeToFile(vector<Production>& productions) {
	printProductions(productions);
	printFirst();
	printFollow();
	printParseTable();
	if (outputFilePath.size() > 2)
	{
		ofstream outfile(outputFilePath);
		if (!outfile) {
			cerr << "Unable to open file for writing: " << outputFilePath << endl;
			return false;
		}
		outfile << preprocessed.str();
		outfile.close();
		return true;
	}
	return false;
}