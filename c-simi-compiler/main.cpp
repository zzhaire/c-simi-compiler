#include"common.h"
#include"LexicalAnalyser.h"
using namespace std;

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "使用方法:" << argv[0] << " "<< "编译文件路径  产生文件路径" << endl;
		cout << "例如:    " << argv[0] << " "<< "text.cpp  " << "product.txt" << endl;
		return 0;
	}
	string file_src;
	Lexer LX;
	LX.lexicalAnalyser(argv[1]);
	LX.outputToFile(argv[2]);
	return 0;
}