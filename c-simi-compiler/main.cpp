#include"common.h"
#include"LexicalAnalyser.h"
using namespace std;
Lexer LX;// �ʷ�����
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "ʹ�÷���:" << argv[0] << " "<< "�����ļ�·��  �����ļ�·��" << endl;
		cout << "����:    " << argv[0] << " "<< "text.cpp  " << "product.txt" << endl;
		return 0;
	}
	LX.lexicalAnalyser(argv[1]);
	if (LX.outputToFile(argv[2]))
		cout << "���ɳɹ�,����" << argv[2] << "�в鿴" << endl;
	return 0;
}