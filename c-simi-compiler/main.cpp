#include "common.h"
#include "LexicalAnalyser.h"
#include "Parser.h"
using namespace std;
Lexer LX;// 词法分析
Parser PS;//语法分析
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "使用方法:" << argv[0] << " " << "编译文件路径" << endl;
		cout << "例如:    " << argv[0] << " " << "text.cpp" << endl;
		return 0;
	}
	if (!LX.lexicalAnalyser(argv[1]))
	{
		cout << "词法分析出错" << endl;
		return 0;
	}
	else {
		cout << "词法分析通过" << endl;
		if (LX.outputToFile())
			cout << "在./products/LexicalProduct.txt中查看" << endl;
	}

	PS.parseAnalyser();
	
	return 0;
}