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

// token �ṹ��Ĺ��캯��
token::token() : token_type(KW_CODE::KW_END_OF_ENUM), token_val("") {}
token::token(KW_CODE temp, string val) : token_type(temp), token_val(val) {}

// Lexer��Ĺ��캯��
Lexer::Lexer() : cur_index(0), cur_lines(1), token_type(0), token_str("") {}

// �ʷ���������������
bool Lexer::lexical_analyzer(const string inputFilePath, const string outputFilePath) {
	ostringstream preprocessed; // ����� �����ݴ����� ����ŵ� ����ļ���

	showAll(preprocessed); // ��� ����� preprocessed
	string allCodes = preprocess(inputFilePath);
	int codeLength = allCodes.length();
	preprocessed << "\n�к�" << "( �ֱ��� , " << " ����ֵ  )" << endl;

	while (cur_index < codeLength) {
		token_str = "";                          // ���token_str
		token_type = scan(allCodes, codeLength); // �õ���һ������

		vector<string> temp_str;
		switch (token_type) {
		case (int)KW_ERR:
			cout << "��" << cur_lines << "��" << "\tERROR: ���ڷǷ��ַ�" << endl;
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
	cout << "ɨ�����" << endl;

	for (const auto& it : tokens) {
		cout << EnumToString[it.token_type] << "\t  " << it.token_val << endl;
	}

	return true;
}

// ��ʾ�����ֱ���
void Lexer::showAll(ostringstream& preprocessed) {
	preprocessed << "�ֱ���: " << endl;

	int count = 0; // �����������ڸ���ÿ�еĹؼ�������
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

// �ж��ַ��Ƿ�Ϊ��ĸ
bool Lexer::IsLetter(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// �ж��ַ��Ƿ�Ϊ����
bool Lexer::IsDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

// Ԥ���������ļ�
string Lexer::preprocess(const string& inputFilePath) {
	cout << "\nԴ�ļ�·��: " << inputFilePath << endl;
	ifstream file(inputFilePath);
	if (!file) {
		cerr << "Unable to open file: " << inputFilePath << std::endl;
		return "0";
	}

	ostringstream preprocessed;
	bool inComment = false;        //  �Ƿ���ע�� ��ʶ��
	bool prevCharWasSpace = false; // ǰһ���ַ��Ƿ��ǿո�

	char c;
	while (file.get(c)) {
		// �����ǰ����ע���У�����հ��ַ���ע�͵Ŀ�ʼ
		if (!inComment) {
			if (c == '/') {
				char nextChar;
				if (file.get(nextChar)) {
					if (nextChar == '*')
					{
						inComment = true; // ����ע��
					}
					else
					{
						// ���'/'���治��'*'����'/'��һ����ͨ�ַ�
						preprocessed << '/';
						file.putback(nextChar); // �Ż��ַ����Ա��´ζ�ȡ
					}
				}
				else {
					// ����ļ������ˣ���'/'����û���ַ�����ô'/'��һ����ͨ�ַ�
					preprocessed << '/';
				}
			}
			else if (!std::isspace(c))
			{
				// ������ǿհ��ַ�������ӵ�Ԥ�������ַ�����
				preprocessed << c;
				prevCharWasSpace = false; // ����״̬����Ϊ���������һ���ǿո��ַ�
			}
			else
			{
				// ����ǿո񣬲���ǰһ���ַ����ǿո����ǰһ���ַ��ǻ��з�������ӿո�
				if (!prevCharWasSpace)
				{
					preprocessed << ' ';
					prevCharWasSpace = true; // ����״̬Ϊ��ǰ�ַ��ǿո�
				}
				if (c == '\n')
				{ // ���� ���з� ͳ��������
					preprocessed << c;
				}
				// ���򣬲����κ����飬��Ϊ�����Ѿ���һ���ո���
			}
		}
		else {
			// �����ǰ��ע���У�����ע�͵Ľ���
			if (c == '*' && file.peek() == '/')
			{
				file.get();               // ��ȡ������'/'�ַ�
				inComment = false;        // ע�ͽ���
				prevCharWasSpace = false; // ����״̬����Ϊע�ͽ�����
			}
			// ע���е��ַ�����ӵ�Ԥ�������ַ�����
		}
	}

	file.close();
	cout << "Ԥ�������" << endl;
	return preprocessed.str();
}

// ɨ������ַ���
int Lexer::scan(const string& allCodes, int codeLength) {

	char curChar = allCodes[cur_index];
	if (curChar == ' ' || curChar == '\n')
	{
		if (curChar == '\n')
		{
			cur_lines += 1;
		}
		token_type = -2;
		++cur_index; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
	}
	else
	{
		//  1.�ж��ַ��Ƿ�Ϊ����
		if (IsDigit(curChar))
		{
			while (IsDigit(curChar))
			{
				token_str += curChar;
				curChar = allCodes[++cur_index];
			}
			if (IsLetter(curChar)) // ���ֺ�ֱ�Ӹ� ��ĸ ���
			{
				token_type = KW_ERR;
			}
			else
				token_type = KW_NUM; // NUM�ֱ���  NUM = digit digit*
		}
		// 2.�ַ�Ϊ�ַ���������Ϊ��ĸ��ͷ�ν���������ֻ���ĸ    �ĵ�ֻҪ�� ID = letter letter*
		else if (IsLetter(curChar))
		{
			while (IsLetter(curChar) || IsDigit(curChar)) // ��ĸ������� Ҳ�Ǵ�ģ�������ʱ����
			{
				token_str += curChar; // ����token_str�ַ���
				curChar = allCodes[++cur_index];
			}

			auto it = stringToEnum.find(token_str);
			token_type = (it != stringToEnum.end()) ? it->second : KW_ID;
		}
		// 3. �ж�Ϊ����
		else
		{
			switch (curChar)
			{
			case '+':
				token_type = KW_PLUS;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "+";
				break;

			case '-':
				token_type = KW_MINUS;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "-";
				break;

			case '*':
				token_type = KW_MULTIPLY;
				curChar = allCodes[++cur_index];
				token_str = "*"; // Ԥ����֮�� /* �� ��Ӧ�� */ Ӧ�ö���ȥ����
				// if (curChar == '/')
				// {
				//     token_type = KW_ERR;

				//     curChar = allCodes[++cur_index];
				//     token_str = "*/";
				// }
				break;

			case '/':
				token_type = KW_DIVIDE;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "/";
				break;

			case '<':
				token_type = KW_LESS_THAN;

				curChar = allCodes[++cur_index];
				token_str = "<";
				if (curChar == '=')
				{
					token_type = KW_LESS_EQUAL;
					cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
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
					cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
					token_str = ">=";
				}
				break;

			case '!':
				token_type = KW_ERR; // �ĵ� û�涨�������  ��Ϊ ERROR
				curChar = allCodes[++cur_index];
				if (allCodes[cur_index] == '=')
				{
					token_type = KW_NOT_EQUAL;
					cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
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
					cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
					token_str = "==";
				}
				break;

			case ';':
				token_type = KW_SEMICOLON;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = ";";
				break;

			case ',':
				token_type = KW_COMMA;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = ",";
				break;

			case '(':
				token_type = KW_LPAREN;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "(";
				break;

			case ')':
				token_type = KW_RPAREN;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = ")";
				break;

			case '[':
				token_type = KW_LBRACKET;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "[";
				break;

			case ']':
				token_type = KW_RBRACKET;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "]";
				break;

			case '{':
				token_type = KW_LBRACE;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "{";
				break;

			case '}':
				token_type = KW_RBRACE;
				cur_index += 1; // ����ȡ��һ���ַ��ˣ� ֱ���˳�
				token_str = "}";
				break;

			default:
				token_type = KW_ERR; // δ����
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
	cout << "\n����ʽԴ�ļ�·��: " << inputFilePath << endl;
	ifstream file(inputFilePath);
	if (!file) {
		cout << "Unable to open file: " << inputFilePath << std::endl;
		return "0";
	}

	string cur_line;
	while (getline(file, cur_line)) { // ���ж�ȡ�ļ� 
		rhs.clear();
		int length = cur_line.size();
		if (length < 4) {
			cout << "���У�������" << endl;
			continue;
		}
		if (cur_line[0] == '#') {
			break;  // �������ʽ������������
		}

		string tmp_str = "";
		int cur_idx = 0;

		// E �� T E'  lhs
		while (cur_line[cur_idx] == ' ') {
			cur_idx++;
		}
		while (cur_line[cur_idx] != ' ') {
			tmp_str += cur_line[cur_idx++];
		}
		lhs.value = tmp_str;

		// ��ȥ��ͷ ���������е� rhs
		cur_idx += 3; // ��ȥ �ո� ��ͷ �ո�
		while (cur_idx < length) {
			tmp_str = "";
			while (cur_idx < length && cur_line[cur_idx] != ' ') {
				tmp_str += cur_line[cur_idx++];
			}
			cur_idx += 1;  // ��ָ��ո�

			// rhs 
			if (tmp_str.size() > 0) {  // �������� ������ �ո�
				auto it = stringToEnum.find(tmp_str);
				if (it != stringToEnum.end()) {  //�ڹؼ��ʱ����ҵ� �� �ս��
					tmphs.type = TERMINAL;
				}
				else { //���ս��
					tmphs.type = NON_TERMINAL;
				}
				tmphs.value = tmp_str;
				rhs.push_back(tmphs);
			}
		}
		//productions.push_back(Production(Symbol(NON_TERMINAL, "S"), {Symbol(NON_TERMINAL, "A"), Symbol(NON_TERMINAL, "B")}));
		productions.push_back(Production(lhs, rhs));
	}

	cout << "proԴ�ļ��������" << endl;


	return true;
}
