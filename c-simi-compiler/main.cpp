#include "common.h"
#include "LexicalAnalyser.h"
#include "Parser.h"
using namespace std;
Lexer LX;// �ʷ�����
Parser PS;//�﷨����
int main(int argc, char** argv)
{
	// argv ==2 ��Դ�ļ��Ͳ���
	if (argc == 1)
	{
		cout << "ʹ�÷���:	" << argv[0] << " " << "�����ļ�·��" << endl;
		cout << "����:		" << argv[0] << " " << "text.cpp" << endl;
		return 0;
	}
	if (!LX.lexicalAnalyser(argv[1]))
	{
		cout << "�ʷ���������" << endl;
		return 0;
	}
	else {
		cout << "�ʷ�����ͨ��" << endl;
		if (LX.outputToFile())
			cout << "��./products/LexicalProduct.txt�в鿴" << endl;
	}

	if (!PS.parseAnalyser())
	{
		cout << "�﷨��������" << endl;
	}
	else {
		cout << "�﷨����ͨ��" << endl;
		cout << "����./products/�ļ����²鿴" << endl;
	}
		
}