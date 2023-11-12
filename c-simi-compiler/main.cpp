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
		cout << "使用方法:" << argv[0] << " "<< "编译文件路径  词法分析结果路径  语法格式文件  项目集结果文件  ACTION表文件  FIRST集文件    语法分析过程文件" << endl;
		cout << "例如:    " << argv[0] << " "<< "text.cpp      product.txt       grammar.txt   items.txt       action.txt    first_set.txt  procedure.txt" << endl;
		return 0;
	}
	LX.lexicalAnalyser(argv[1]);
	LX.outputToFile(argv[2]);

	PS.parseAnalyser(argv[3], argv[2], argv[4], argv[5], argv[6], argv[7]);
	

	if (LX.outputToFile(argv[2]))
		cout << "生成成功,请在" << argv[2] << "中查看" << endl;
	return 0;
}