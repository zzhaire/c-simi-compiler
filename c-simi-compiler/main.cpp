#include"common.h"
#include"LexicalAnalyser.h"
using namespace std;
Lexer LX;// 词法分析
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "使用方法:" << argv[0] << " "<< "编译文件路径  产生文件路径" << endl;
		cout << "例如:    " << argv[0] << " "<< "text.cpp  " << "product.txt" << endl;
		return 0;
	}
	LX.lexicalAnalyser(argv[1]);
	if (LX.outputToFile(argv[2]))
		cout << "生成成功,请在" << argv[2] << "中查看" << endl;
	return 0;
}