#pragma once
#include"common.h"

using namespace std;

const int N = 150;			// 产生式的数量

/* LR(1)项目集结构体 */
typedef struct ITEMS {
	int no;	// 表示该产生式在文法中的编号
	int now;	// 表示圆点·所在的位置，产生式右部第一个位置为0
	char outlook;	// 表示展望符
	friend bool operator == (ITEMS item1, ITEMS item2) {	// 重载==运算符
		return (item1.no == item2.no) && (item1.now == item2.now) && (item1.outlook == item2.outlook);
	}
}ITEMS;


/* 项目集规范族，体现项目间的转移关系 */
typedef struct TRANSFER_RELATION {
	int src;	// 初始状态
	int des;	// 转移状态
	char ch;	// 读取到的字符
}TRANSFER_RELATION;


/* ACNTION表结构体 */
typedef struct ACTION {
	char ch;	// 读取到的字符
	int next_state;	// 正数表示移进并转移到第next_state个项目，负数表示用第|next_state|条产生式归约
}ACTION;


/* 语法树结构体 */
typedef struct TREENODE {
	int id;		// 结点的唯一标识
	char value;	// 字符值
	int parent;	// 当前结点的父亲
	vector<int> child;	// 当前结点的所有儿子
	TREENODE(int id, char value) {	// 构造函数
		this->id = id;
		this->value = value;
	}
}TREENODE;

class Parser {
private:
	/* 产生式相关变量 */
	int g_number;						// 记录拓广文法G'的产生式数量
	pair<char, string>g_production[N];	// 记录所有的产生式
	set<char>n_terminal;				// 非终结符集
	set<char>terminal;					// 终结符集
	
	/* FIRST集相关变量 */
	set<char> first[N];		// 存放每个字符的FIRST集

	/* 项目集相关变量 */
	vector<char> outlooks;	// 存放当前产生式的所有展望符
	vector<ITEMS> items[N];	// 存储所有项目的所有产生式
	int item_num;			// 项目的数量
	vector<TRANSFER_RELATION> trans;	//表示一个项目集规范族内的转移

	/* ACTION表相关 */
	vector<vector<ACTION>> action_table;	// action_table[i][j]表示状态i的第j个动作
	int state_num;			// 表示一共有多少个状态，它的值应该等于items_num

	/* 归约过程相关 */
	stack<int> state_stack;		// 状态栈
	stack<TREENODE> sign_stack;		// 符号栈
	string input;	// 输入串，即词法分析器得出的结果

	/* 文件读写相关 */
	ifstream grammar_file;		// 文法文件
	ifstream lexical_file;		// 词法分析器给出的分析结果
	ofstream items_file;		// 项目集
	ofstream action_file;		// ACTION表
	ofstream first_set_file;	// 文法中所有非终结符的First集
	ofstream procedure_file;	// 存储分析过程
public:
	int ifTerminal(char ch);	// 检查当前字符是否为终结符
	void terminalInsert(char ch);	// 检查当前字符，并根据需要和字符属性放到非终结符或终结符集中
	void openFile(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// 打开需要的文件
	void closeFile();	// 关闭所有文件
	void getGrammer();	// 获取语法分析的格式文件
	void getFirst();	// 获取文法中所有非终结符的FIRST集
	void writeFirst();	// 将所有非终结符的FIRST集写入文本中
	void getItemOutLook(ITEMS production);	// 获取给定产生式的展望符
	void getClosure(int i);	// 获得项目集i的闭包
	bool inItem(ITEMS production, int i);	// 判断产生式是否在当前项目中
	int getNextItem(int item_num);	// 找到items[item_num]项目第一次出现的位置
	void goToActions(set<char>symbol_set, int id);	// GOTO求解当前项目在读取symbol_set中字符后，产生的后继项目
	void getItemSet();	// 构造项目集
	void writeItems();	// 将生成好的项目集写入文件中
	void getActionTable();	// 构造ACTION表
	void writeActionTable();	// 将生成好的ACTION表写入文件中
	void writeAnalyzeProcedure(int& step);	// 将分析过程写入文件
	bool analyseInputString();	// 分析词法分析器给出的输入串是否为该文法接受的串（即归约过程）
	bool parseAnalyser();	// 核心函数，用于语法分析
};
