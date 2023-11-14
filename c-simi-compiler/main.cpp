#include "common.h"
#include "LexicalAnalyser.h"
#include "Parser.h"
using namespace std;
Lexer LX;// �ʷ�����
Parser PS;//�﷨����
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "ʹ�÷���:" << argv[0] << " "<< "�����ļ�·��  ����﷨����  �ʷ��������·��    �﷨��ʽ�ļ�  ��Ŀ������ļ�  ACTION���ļ�  FIRST���ļ�    �﷨���������ļ�" << endl;
		cout << "����:    " << argv[0] << " "<< "text.cpp      product.txt   lexical_output.txt    grammar.txt   items.txt       action.txt    first_set.txt  procedure.txt" << endl;
		return 0;
	}
	LX.lexicalAnalyser(argv[1]);
	LX.outputToFile(argv[2]);

	PS.parseAnalyser(argv[4], argv[3], argv[5], argv[6], argv[7], argv[8]);
	

	if (LX.outputToFile(argv[2]))
		cout << "���ɳɹ�,����" << argv[2] << "�в鿴" << endl;
	return 0;
}