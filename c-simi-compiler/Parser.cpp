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
	if (if_terminal == false) {	// ���ս��
		if (!n_terminal.count(ch))
			n_terminal.insert(ch);
	}
	else if (if_terminal == true) {	// �ս��
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
	// ��ȡ�﷨�����ĸ�ʽ�ļ��������ʽΪpair<���ս��������ʽ�Ҳ�>
	char symbol, _;
	grammar_file >> g_number;	// ��ȡ����ʽ����

	// ���ķ�G��Ϊ�ع��ķ�
	g_production[0].first = 'S';

	// ֮��ʼ��ȡ�﷨����ʽ
	for (int i = 1; i <= g_number; i++)
	{
		// ��ȡ��ʽΪ��Ԫ->����ʽ�Ҳ����Ȳ����Ҳ�
		grammar_file >> symbol >> _ >> _ ;	
		g_production[i].first = symbol;
		terminalInsert(symbol);

		// ���������Ҳ�
		grammar_file >> symbol;
		while (symbol != END_SIGN) 
		{
			g_production[i].second += symbol;
			terminalInsert(symbol);
			grammar_file >> symbol;
		}
	}
	g_production[0].second = g_production[1].first;	// ���ϵ�һ��ع��ķ���
}

void Parser::getFirst()
{
	bool continue_flag = true, is_empty = true;
	while (continue_flag)
	{
		continue_flag = false;
		for (int i = 1; i <= g_number; i++){

			// ÿһ�ζ�����ÿһ���ķ������ұߵ�һ���ַ���ʼ��飬ÿ��ֻ����һ�����ս����FIRST��
			int index = 0;
			is_empty = true;
			char left = g_production[i].first, right = g_production[i].second[index];
			while (is_empty && index < g_production[i].second.size()) {	// ���ѭ������ΪFIRST(right)�п��ܰ���epsilon
				is_empty = false;

				// ����ұߵ�ǰ�ַ�Ϊ���ս��������FIRST������ⷽ����FIRST(right)�ŵ���ǰ���ս����FIRST��
				if (ifTerminal(right) == false) {
					for(auto item : first[right])
						if (item != EPSILON) {
							first[left].insert(item);
							continue_flag = true;
						}
					// �����ǰFIRST(right)�а���epsilon������Ҫ��right��һ���ַ���FIRSTҲ�ŵ���ǰ���ս����FIRST��
					if (first[right].count(EPSILON)) {
						is_empty = true;
						right = g_production[i].second[++index];
						continue;
					}
				}
				// �ߵ����������A->a���͵Ĳ���ʽ(a������epsilon)����˿���ֱ�ӽ�a����FIRST(left)
				else {
					first[left].insert(right);
					continue_flag = true;	// ִ�е�����FIRST(left)�Ѿ������ã��������������right��
				}
			}
			// ��һֱ�ߵ�����ʽ��β����Ӧ����FIRST(left)�м���epsilon
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
	
	/* ��ʵ�� */

	closeFile();
}
