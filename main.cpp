#include "paser.h"
#include "lexer.h"
 
int main()
{
	queue<Symbol> tokensQ;
	vector<Production> productions;
	Lexer myLexer;
	Paser myPaser;

	string startSymble = "program";  // �﷨������ʼ����

	string inFilePath = "D:\\VScode\\visual\\mywork\\input2.txt";  // ��������λ��
	//string inFilePath = "D:\\VScode\\visual\\mywork\\input.txt";  // ��������λ��
	string outFilePath = "D:\\VScode\\visual\\mywork\\output.txt";// �ʷ��������λ��
	string inFilePathPaser = "D:\\VScode\\visual\\mywork\\productions.txt";// ����ʽ���λ��
	string outFilePathPaser = "D:\\VScode\\visual\\mywork\\outputPaser.txt";// �ʷ��������λ��

	if (!myLexer.lexical_analyzer(inFilePath, outFilePath)){ // ���ôʷ�����
		cout << "\n�ʷ��������̳��ִ���\n";
		return -1; // ����ʧ�� �˳����� -1
	}

	if (!myLexer.getTokensQueue(tokensQ)) { cout << "����token����ʧ�ܣ�" << endl; }

	myLexer.getProductions(productions, inFilePathPaser); // ��������ʽ

	myPaser.setOutputFilePath(outFilePathPaser);
	myPaser.getTermnalANDNontermnal(productions);      // ����ս���ű� ���ս���ű�
	myPaser.calculateFirst(productions);               // ���� first ��
	myPaser.calculateFollow(productions, startSymble); // ����follow ��
	myPaser.constructParseTable(productions);          // Ԥ�������

	if (myPaser.isLL()) { cout << "��LL" << endl; }                                          //�ж��Ƿ���� LL(1) �ķ�
	if (myPaser.parse(productions, startSymble, tokensQ)) { cout << "�����ɹ���" << endl; }  // �﷨����
	else { cout << "����ʧ��...." << endl; }
	if (!myPaser.storeToFile(productions)) { cout << "�洢��Ŀ���ļ�ʧ��" << endl; }         // ������������ļ�

	return 0;
}