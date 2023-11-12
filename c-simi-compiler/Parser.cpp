#include "Parser.h"
#include "common.h"
using namespace std;

int Parser::ifTerminal(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return false;
	else if (ch != EPSILON)
		return true;
	return -1;
}

void Parser::terminalInsert(char ch)
{
	int if_terminal = ifTerminal(ch);
	if (if_terminal == false) {	// 非终结符
		if (!n_terminal.count(ch))
			n_terminal.insert(ch);
	}
	else if (if_terminal == true) {	// 终结符
		if (!terminal.count(ch))
			terminal.insert(ch);
	}
}

void Parser::openFile(string grammar_path, string lexical_path, string items_path, string action_path, string first_set_path, string procedure_path)
{
	grammar_file.open(grammar_path);
	lexical_file.open(lexical_path);
	items_file.open(items_path);
	action_file.open(action_path);
	first_set_file.open(first_set_path);
	procedure_file.open(procedure_path);
}

void Parser::closeFile()
{
	grammar_file.close();
	lexical_file.close();
	items_file.close();
	action_file.close();
	first_set_file.close();
	procedure_file.close();
}

void Parser::getGrammer()
{
	// 获取语法分析的格式文件，保存格式为pair<非终结符，产生式右部>
	char symbol, _;
	grammar_file >> g_number;	// 获取产生式数量

	// 把文法G变为拓广文法
	g_production[0].first = 'S';

	// 之后开始读取语法产生式
	for (int i = 1; i <= g_number; i++)
	{
		// 读取格式为变元->产生式右部，先不读右部
		grammar_file >> symbol >> _ >> _ ;	
		g_production[i].first = symbol;
		terminalInsert(symbol);

		// 单独处理右部
		grammar_file >> symbol;
		while (symbol != END_SIGN) 
		{
			g_production[i].second += symbol;
			terminalInsert(symbol);
			grammar_file >> symbol;
		}
	}
	g_production[0].second = g_production[1].first;	// 补上第一项（拓广文法）
}

void Parser::getFirst()
{
	bool continue_flag = true, is_empty = true;
	while (continue_flag)
	{
		continue_flag = false;
		for (int i = 1; i <= g_number; i++){

			// 每一次都遍历每一条文法，从右边第一个字符开始检查，每次只求解出一个非终结符的FIRST集
			int index = 0;
			is_empty = true;
			char left = g_production[i].first, right = g_production[i].second[index];
			while (is_empty && index < g_production[i].second.size()) {	// 这层循环是因为FIRST(right)中可能包含epsilon
				is_empty = false;

				// 如果右边当前字符为非终结符，根据FIRST集的求解方法将FIRST(right)放到当前非终结符的FIRST中
				if (ifTerminal(right) == false) {
					for(auto item : first[right])
						if (item != EPSILON) {
							if (first[left].insert(item).second) // 判断元素是否成功加入到left的FIRST集，只有当新元素被加入时才需要继续遍历
							{
								continue_flag = true;
							}
						}
					// 如果当前FIRST(right)中包含epsilon，则需要将right下一个字符的FIRST也放到当前非终结符的FIRST中
					if (first[right].count(EPSILON)) {
						is_empty = true;
						right = g_production[i].second[++index];
						continue;
					}
				}
				// 走到这里表明是A->a类型的产生式(a可能是epsilon)，因此可以直接将a加入FIRST(left)
				else {
					if (first[left].insert(right).second) //如果新元素被加入到FIRST集，则需要再次遍历
					{
						continue_flag = true; // 执行到这里FIRST(left)已经构建好，不会继续向下找right了
					}
				}
			}
			// 能一直走到产生式结尾，则应该在FIRST(left)中加入epsilon
			if (index == g_production[i].second.size())
				first[left].insert(EPSILON);
		}
	}
}

void Parser::writeFirst()
{
	for (int i = 'A'; i <= 'Z'; i++) {
		if (!first[i].empty()) {
			first_set_file << "FIRST(" << char(i) << ")={";
			for (auto item : first[i])
				first_set_file << char(item) << ",";
		}
		first_set_file << "}" << endl;
	}
}

void Parser::parseAnalyser(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file)
{
	openFile(grammar_file, lexical_file, items_file, action_file, first_set_file, procedure_file);
	getGrammer();
	getFirst();
	writeFirst();
	
	/* 待实现 */

	closeFile();
}
