#pragma once
#include "common.h"
using namespace std;

const int N = 200;			// 产生式的数量







class Parser {
private:
	/* 产生式相关变量 */
	int g_number;						// 记录拓广文法G'的产生式数量
	pair<char, string>g_production[N];	// 记录所有的产生式
	set<char>n_terminal;				// 非终结符集
	set<char>terminal;					// 终结符集
	
	/* FIRST集相关变量 */
	set<char> first[N];		// 存放每个字符的FIRST集
	vector<char> outlooks;	// 存放当前产生式的所有展望符

	/* 文件读写相关 */
	ifstream grammar_file;		// 文法文件
	ifstream lexical_file;		// 词法分析器给出的分析结果
	ofstream items_file;		// 项目集
	ofstream action_file;		// ACTION表
	ofstream first_set_file;	// 文法中所有非终结符的First集
	ofstream procedure_file;	// 分析过程
public:
	int ifTerminal(char ch);	// 检查当前字符是否为终结符
	void terminalInsert(char ch);	// 检查当前字符，并根据需要和字符属性放到非终结符或终结符集中
	void openFile(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// 打开需要的文件
	void closeFile();	// 关闭所有文件
	void getGrammer();	// 获取语法分析的格式文件
	void getFirst();	// 获取文法中所有非终结符的FIRST集
	void writeFirst();	// 将所有非终结符的FIRST集写入文本中
	void parseAnalyser(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// 核心函数，用于语法分析
};
