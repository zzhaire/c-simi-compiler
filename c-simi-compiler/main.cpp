#include"common.h"
#include"LexicalAnalyser.h"
using namespace std;

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "ʹ�÷���:" << argv[0] << " "<< "�����ļ�·��  �����ļ�·��" << endl;
		cout << "����:    " << argv[0] << " "<< "text.cpp  " << "product.txt" << endl;
		return 0;
	}
	string file_src;
	Lexer LX;
	LX.lexicalAnalyser(argv[1]);
	LX.outputToFile(argv[2]);
	return 0;
}