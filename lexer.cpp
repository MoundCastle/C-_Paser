#include "lexer.h"

std::map<std::string, KW_CODE> stringToEnum = {
	{"unmain", KW_CODE::KW_MAIN}, {"else", KW_CODE::KW_ELSE}, {"if", KW_CODE::KW_IF}, {"int", KW_CODE::KW_INT},
	{"return", KW_CODE::KW_RETURN}, {"void", KW_CODE::KW_VOID}, {"while", KW_CODE::KW_WHILE},
	{"ID", KW_CODE::KW_ID}, {"NUM", KW_CODE::KW_NUM}, {"ERROR", KW_CODE::KW_ERR},
	{"+", KW_CODE::KW_PLUS}, {"-", KW_CODE::KW_MINUS}, {"*", KW_CODE::KW_MULTIPLY}, {"/", KW_CODE::KW_DIVIDE},
	{"<", KW_CODE::KW_LESS_THAN}, {"<=", KW_CODE::KW_LESS_EQUAL}, {">", KW_CODE::KW_GREATER_THAN}, {">=", KW_CODE::KW_GREATER_EQUAL},
	{"==", KW_CODE::KW_EQUAL}, {"!=", KW_CODE::KW_NOT_EQUAL}, {"=", KW_CODE::KW_ASSIGNMENT},
	{";", KW_CODE::KW_SEMICOLON}, {",", KW_CODE::KW_COMMA}, {"(", KW_CODE::KW_LPAREN}, {")", KW_CODE::KW_RPAREN},
	{"[", KW_CODE::KW_LBRACKET}, {"]", KW_CODE::KW_RBRACKET}, {"{", KW_CODE::KW_LBRACE}, {"}", KW_CODE::KW_RBRACE},
	{"/*", KW_CODE::KW_COMMENT_START}, {"*/", KW_CODE::KW_COMMENT_END},{"@",KW_CODE::KW_EPSILON}
};

std::map<KW_CODE, std::string> EnumToString = {
	{KW_CODE::KW_MAIN, "main"}, {KW_CODE::KW_ELSE, "else"}, {KW_CODE::KW_IF, "if"}, {KW_CODE::KW_INT, "int"},
	{KW_CODE::KW_RETURN, "return"}, {KW_CODE::KW_VOID, "void"}, {KW_CODE::KW_WHILE, "while"},
	{KW_CODE::KW_ID, "ID"}, {KW_CODE::KW_NUM, "NUM"}, {KW_CODE::KW_ERR, "ERROR"},
	{KW_CODE::KW_PLUS, "+"}, {KW_CODE::KW_MINUS, "-"}, {KW_CODE::KW_MULTIPLY, "*"}, {KW_CODE::KW_DIVIDE, "/"},
	{KW_CODE::KW_LESS_THAN, "<"}, {KW_CODE::KW_LESS_EQUAL, "<="}, {KW_CODE::KW_GREATER_THAN, ">"}, {KW_CODE::KW_GREATER_EQUAL, ">="},
	{KW_CODE::KW_EQUAL, "=="}, {KW_CODE::KW_NOT_EQUAL, "!="}, {KW_CODE::KW_ASSIGNMENT, "="},
	{KW_CODE::KW_SEMICOLON, ";"}, {KW_CODE::KW_COMMA, ","}, {KW_CODE::KW_LPAREN, "("}, {KW_CODE::KW_RPAREN, ")"},
	{KW_CODE::KW_LBRACKET, "["}, {KW_CODE::KW_RBRACKET, "]"}, {KW_CODE::KW_LBRACE, "{"}, {KW_CODE::KW_RBRACE, "}"},
	{KW_CODE::KW_COMMENT_START, "/*"}, {KW_CODE::KW_COMMENT_END, "*/"},{KW_CODE::KW_EPSILON,"@"}
};

// token 结构体的构造函数
token::token() : token_type(KW_CODE::KW_END_OF_ENUM), token_val("") {}
token::token(KW_CODE temp, string val) : token_type(temp), token_val(val) {}

// Lexer类的构造函数
Lexer::Lexer() : cur_index(0), cur_lines(1), token_type(0), token_str("") {}

// 词法分析器的主函数
bool Lexer::lexical_analyzer(const string inputFilePath, const string outputFilePath) {
	ostringstream preprocessed; // 输出流 用以暂存数据 最后存放到 输出文件中

	showAll(preprocessed); // 结果 存放在 preprocessed
	string allCodes = preprocess(inputFilePath);
	int codeLength = allCodes.length();
	preprocessed << "\n行号" << "( 种别码 , " << " 属性值  )" << endl;

	while (cur_index < codeLength) {
		token_str = "";                          // 清空token_str
		token_type = scan(allCodes, codeLength); // 得到下一个词素

		vector<string> temp_str;
		switch (token_type) {
		case (int)KW_ERR:
			cout << "第" << cur_lines << "行" << "\tERROR: 存在非法字符" << endl;
			return false;
		case -2:
			break;
		default:
			tokens.emplace_back((KW_CODE)token_type, token_str);
			temp_str.push_back(to_string(cur_lines));
			temp_str.push_back(to_string(token_type));
			temp_str.push_back(token_str);
			vc_token.emplace_back(temp_str);
			preprocessed << cur_lines << "\t(  " << token_type << "\t,  " << token_str << "\t)" << endl;
		}
	}

	ofstream outfile(outputFilePath);
	if (!outfile) {
		cerr << "Unable to open file for writing: " << outputFilePath << endl;
		return false;
	}
	outfile << preprocessed.str();
	outfile.close();
	cout << "扫描结束" << endl;

	for (const auto& it : tokens) {
		cout << EnumToString[it.token_type] << "\t  " << it.token_val << endl;
	}

	return true;
}

// 显示所有种别码
void Lexer::showAll(ostringstream& preprocessed) {
	preprocessed << "种别码: " << endl;

	int count = 0; // 计数器，用于跟踪每行的关键词数量
	int columnsPerRow = 3;
	for (int i = 0; i < KW_END_OF_ENUM; i++) {
		KW_CODE temp = (KW_CODE)i;
		preprocessed << setw(6) << EnumToString[temp] << " \t " << setw(2) << i << "\t\t";
		count++;
		if (count == columnsPerRow || i == KW_END_OF_ENUM - 1) {
			preprocessed << endl;
			count = 0;
		}
	}
}

// 判断字符是否为字母
bool Lexer::IsLetter(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// 判断字符是否为数字
bool Lexer::IsDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

// 预处理输入文件
string Lexer::preprocess(const string& inputFilePath) {
	cout << "\n源文件路径: " << inputFilePath << endl;
	ifstream file(inputFilePath);
	if (!file) {
		cerr << "Unable to open file: " << inputFilePath << std::endl;
		return "0";
	}

	ostringstream preprocessed;
	bool inComment = false;        //  是否在注释 标识符
	bool prevCharWasSpace = false; // 前一个字符是否是空格

	char c;
	while (file.get(c)) {
		// 如果当前不在注释中，处理空白字符和注释的开始
		if (!inComment) {
			if (c == '/') {
				char nextChar;
				if (file.get(nextChar)) {
					if (nextChar == '*')
					{
						inComment = true; // 进入注释
					}
					else
					{
						// 如果'/'后面不是'*'，则'/'是一个普通字符
						preprocessed << '/';
						file.putback(nextChar); // 放回字符流以便下次读取
					}
				}
				else {
					// 如果文件结束了，但'/'后面没有字符，那么'/'是一个普通字符
					preprocessed << '/';
				}
			}
			else if (!std::isspace(c))
			{
				// 如果不是空白字符，则添加到预处理后的字符串中
				preprocessed << c;
				prevCharWasSpace = false; // 重置状态，因为我们添加了一个非空格字符
			}
			else
			{
				// 如果是空格，并且前一个字符不是空格或者前一个字符是换行符，则添加空格
				if (!prevCharWasSpace)
				{
					preprocessed << ' ';
					prevCharWasSpace = true; // 设置状态为当前字符是空格
				}
				if (c == '\n')
				{ // 保留 换行符 统计行数用
					preprocessed << c;
				}
				// 否则，不做任何事情，因为我们已经有一个空格了
			}
		}
		else {
			// 如果当前在注释中，查找注释的结束
			if (c == '*' && file.peek() == '/')
			{
				file.get();               // 读取并丢弃'/'字符
				inComment = false;        // 注释结束
				prevCharWasSpace = false; // 重置状态，因为注释结束了
			}
			// 注释中的字符不添加到预处理后的字符串中
		}
	}

	file.close();
	cout << "预处理结束" << endl;
	return preprocessed.str();
}

// 扫描代码字符串
int Lexer::scan(const string& allCodes, int codeLength) {

	char curChar = allCodes[cur_index];
	if (curChar == ' ' || curChar == '\n')
	{
		if (curChar == '\n')
		{
			cur_lines += 1;
		}
		token_type = -2;
		++cur_index; // 不用取下一个字符了， 直接退出
	}
	else
	{
		//  1.判断字符是否为数字
		if (IsDigit(curChar))
		{
			while (IsDigit(curChar))
			{
				token_str += curChar;
				curChar = allCodes[++cur_index];
			}
			if (IsLetter(curChar)) // 数字后直接跟 字母 错的
			{
				token_type = KW_ERR;
			}
			else
				token_type = KW_NUM; // NUM种别码  NUM = digit digit*
		}
		// 2.字符为字符串，表现为字母开头衔接任意个数字或字母    文档只要求 ID = letter letter*
		else if (IsLetter(curChar))
		{
			while (IsLetter(curChar) || IsDigit(curChar)) // 字母后跟数字 也是错的，这里暂时容许
			{
				token_str += curChar; // 加入token_str字符串
				curChar = allCodes[++cur_index];
			}

			auto it = stringToEnum.find(token_str);
			token_type = (it != stringToEnum.end()) ? it->second : KW_ID;
		}
		// 3. 判断为符号
		else
		{
			switch (curChar)
			{
			case '+':
				token_type = KW_PLUS;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "+";
				break;

			case '-':
				token_type = KW_MINUS;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "-";
				break;

			case '*':
				token_type = KW_MULTIPLY;
				curChar = allCodes[++cur_index];
				token_str = "*"; // 预处理之后 /* 和 对应的 */ 应该都被去除了
				// if (curChar == '/')
				// {
				//     token_type = KW_ERR;

				//     curChar = allCodes[++cur_index];
				//     token_str = "*/";
				// }
				break;

			case '/':
				token_type = KW_DIVIDE;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "/";
				break;

			case '<':
				token_type = KW_LESS_THAN;

				curChar = allCodes[++cur_index];
				token_str = "<";
				if (curChar == '=')
				{
					token_type = KW_LESS_EQUAL;
					cur_index += 1; // 不用取下一个字符了， 直接退出
					token_str = "<=";
				}
				break;

			case '>':
				token_type = KW_GREATER_THAN;
				curChar = allCodes[++cur_index];
				token_str = ">";
				if (allCodes[cur_index] == '=')
				{
					token_type = KW_GREATER_EQUAL;
					cur_index += 1; // 不用取下一个字符了， 直接退出
					token_str = ">=";
				}
				break;

			case '!':
				token_type = KW_ERR; // 文档 没规定这个符号  设为 ERROR
				curChar = allCodes[++cur_index];
				if (allCodes[cur_index] == '=')
				{
					token_type = KW_NOT_EQUAL;
					cur_index += 1; // 不用取下一个字符了， 直接退出
					token_str = "!=";
				}
				break;

			case '=':
				token_type = KW_ASSIGNMENT;
				curChar = allCodes[++cur_index];
				token_str = "=";
				if (allCodes[cur_index] == '=')
				{
					token_type = KW_EQUAL;
					cur_index += 1; // 不用取下一个字符了， 直接退出
					token_str = "==";
				}
				break;

			case ';':
				token_type = KW_SEMICOLON;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = ";";
				break;

			case ',':
				token_type = KW_COMMA;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = ",";
				break;

			case '(':
				token_type = KW_LPAREN;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "(";
				break;

			case ')':
				token_type = KW_RPAREN;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = ")";
				break;

			case '[':
				token_type = KW_LBRACKET;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "[";
				break;

			case ']':
				token_type = KW_RBRACKET;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "]";
				break;

			case '{':
				token_type = KW_LBRACE;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "{";
				break;

			case '}':
				token_type = KW_RBRACE;
				cur_index += 1; // 不用取下一个字符了， 直接退出
				token_str = "}";
				break;

			default:
				token_type = KW_ERR; // 未出现
				break;
			}
		}
	}

	return token_type;
}

bool Lexer::getTokensQueue(queue<Symbol>& tokensQ) {
	cout << "\nSetTokens" << endl;
	for (auto it : vc_token) {
		KW_CODE temp_code = (KW_CODE)stoi(it[1]);
		string tk_type_str = EnumToString[temp_code];
		string tk_val_str = it[2];
		if (tk_type_str != tk_val_str) {
			Symbol tmp(TERMINAL, tk_type_str, tk_val_str, stoi(it[0]));
			tokensQ.push(tmp);
		}
		else {
			Symbol tmp(TERMINAL, tk_type_str, tk_type_str, stoi(it[0]));
			tokensQ.push(tmp);
		}

	}
	tokensQ.push(Symbol(TERMINAL, "$", tokensQ.back().lines + 1));
	cout << "\nSetTokens Over." << endl;

	return true;
}

bool Lexer::getProductions(vector<Production>& productions, const string& inputFilePath)
{
	Symbol lhs(NON_TERMINAL, "NULL");
	Symbol tmphs;
	vector<Symbol> rhs;
	cout << "\n产生式源文件路径: " << inputFilePath << endl;
	ifstream file(inputFilePath);
	if (!file) {
		cout << "Unable to open file: " << inputFilePath << std::endl;
		return "0";
	}

	string cur_line;
	while (getline(file, cur_line)) { // 逐行读取文件 
		rhs.clear();
		int length = cur_line.size();
		if (length < 4) {
			cout << "空行，无意义" << endl;
			continue;
		}
		if (cur_line[0] == '#') {
			break;  // 读完产生式，遇到结束符
		}

		string tmp_str = "";
		int cur_idx = 0;

		// E → T E'  lhs
		while (cur_line[cur_idx] == ' ') {
			cur_idx++;
		}
		while (cur_line[cur_idx] != ' ') {
			tmp_str += cur_line[cur_idx++];
		}
		lhs.value = tmp_str;

		// 丢去箭头 ，读出所有的 rhs
		cur_idx += 3; // 丢去 空格 箭头 空格
		while (cur_idx < length) {
			tmp_str = "";
			while (cur_idx < length && cur_line[cur_idx] != ' ') {
				tmp_str += cur_line[cur_idx++];
			}
			cur_idx += 1;  // 不指向空格

			// rhs 
			if (tmp_str.size() > 0) {  // 不是上面 流出的 空格
				auto it = stringToEnum.find(tmp_str);
				if (it != stringToEnum.end()) {  //在关键词表中找到 是 终结符
					tmphs.type = TERMINAL;
				}
				else { //非终结符
					tmphs.type = NON_TERMINAL;
				}
				tmphs.value = tmp_str;
				rhs.push_back(tmphs);
			}
		}
		//productions.push_back(Production(Symbol(NON_TERMINAL, "S"), {Symbol(NON_TERMINAL, "A"), Symbol(NON_TERMINAL, "B")}));
		productions.push_back(Production(lhs, rhs));
	}

	cout << "pro源文件读入完毕" << endl;


	return true;
}
