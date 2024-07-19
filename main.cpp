#include "paser.h"
#include "lexer.h"
 
int main()
{
	queue<Symbol> tokensQ;
	vector<Production> productions;
	Lexer myLexer;
	Paser myPaser;

	string startSymble = "program";  // 语法分析开始符号

	string inFilePath = "D:\\VScode\\visual\\mywork\\input2.txt";  // 分析程序位置
	//string inFilePath = "D:\\VScode\\visual\\mywork\\input.txt";  // 分析程序位置
	string outFilePath = "D:\\VScode\\visual\\mywork\\output.txt";// 词法分析输出位置
	string inFilePathPaser = "D:\\VScode\\visual\\mywork\\productions.txt";// 产生式存放位置
	string outFilePathPaser = "D:\\VScode\\visual\\mywork\\outputPaser.txt";// 词法分析输出位置

	if (!myLexer.lexical_analyzer(inFilePath, outFilePath)){ // 调用词法分析
		cout << "\n词法分析过程出现错误\n";
		return -1; // 调用失败 退出返回 -1
	}

	if (!myLexer.getTokensQueue(tokensQ)) { cout << "设置token队列失败！" << endl; }

	myLexer.getProductions(productions, inFilePathPaser); // 读出产生式

	myPaser.setOutputFilePath(outFilePathPaser);
	myPaser.getTermnalANDNontermnal(productions);      // 获得终结符号表 非终结符号表
	myPaser.calculateFirst(productions);               // 计算 first 集
	myPaser.calculateFollow(productions, startSymble); // 计算follow 集
	myPaser.constructParseTable(productions);          // 预测分析表

	if (myPaser.isLL()) { cout << "是LL" << endl; }                                          //判断是否符合 LL(1) 文法
	if (myPaser.parse(productions, startSymble, tokensQ)) { cout << "分析成功！" << endl; }  // 语法分析
	else { cout << "分析失败...." << endl; }
	if (!myPaser.storeToFile(productions)) { cout << "存储到目标文件失败" << endl; }         // 分析结果存入文件

	return 0;
}