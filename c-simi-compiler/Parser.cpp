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
						if (item != EPSILON) 
							if (first[left].insert(item).second) // �ж�Ԫ���Ƿ�ɹ����뵽left��FIRST����ֻ�е���Ԫ�ر�����ʱ����Ҫ��������
								continue_flag = true;
					// �����ǰFIRST(right)�а���epsilon������Ҫ��right��һ���ַ���FIRSTҲ�ŵ���ǰ���ս����FIRST��
					if (first[right].count(EPSILON)) {
						is_empty = true;
						right = g_production[i].second[++index];
						continue;
					}
				}
				// �ߵ����������A->a���͵Ĳ���ʽ(a������epsilon)����˿���ֱ�ӽ�a����FIRST(left)
				else
					if (first[left].insert(right).second) //�����Ԫ�ر����뵽FIRST��������Ҫ�ٴα���
						continue_flag = true; // ִ�е�����FIRST(left)�Ѿ������ã��������������right��
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

void Parser::getItemOutLook(ITEMS production)
{
	int out_look_index = production.now + 1;
	while (1) {
		// ����������ʽ�Ľ�β��׼������
		if (out_look_index == g_production[production.no].second.size()) {	
			outlooks.push_back(production.outlook);
			return;
		}
		// ������ս����ֱ�Ӽ��벢�˳�
		char next_char = g_production[production.no].second[out_look_index];
		if (ifTerminal(next_char) == true) {	
			outlooks.push_back(next_char);
			return;
		}
		// ����Ƿ��ս����������FIRST��
		for (auto ch : first[next_char])
			if (ch != EPSILON)	// ������epsilon
				outlooks.push_back(ch);
		if (first[next_char].count(EPSILON) > 0)	// ��ǰ���ս����FIRST���ܲ���epsilon����Ҫ�����һ������
			out_look_index++;
		else // �����Ѿ��鿴��ϣ�ֱ�ӷ���
			return;
	}
}

bool Parser::inItem(ITEMS production, int i)
{
	for (int k = 0; k < items[i].size(); k++)
		if (items[i][k] == production) 
			return true;
	return false;
}

void Parser::getClosure(int i)
{
	for (int j = 0; j < items[i].size(); j++) {	// ������ǰ��Ŀ�����в���ʽ
		int no = items[i][j].no, now = items[i][j].now;
		if (ifTerminal(g_production[no].second[now]) == false)
			// �����ǰ�Ҳ�Բ�����һ��λ���Ƿ��ս�������������Ǹ÷��ս���Ĳ���ʽ�������Ŀ��
			getItemOutLook(items[i][j]);	// ���쵱ǰ����ʽ��չ����
			for (int k = 0; k <= g_number; k++)
				if (g_production[k].first == g_production[no].second[now]) {
					ITEMS production;
					production.no = k, production.now = 1;
					for (int z = 0; z < outlooks.size(); z++) {
						production.outlook = outlooks[z];
						if (!inItem(production, i))	// �����ǰ����ʽ������Ŀ�У�������뵱ǰ��Ŀ
							items[i].push_back(production);
					}
				}
			// ��ʼ����һ������ʽ�ıհ�
			outlooks.clear();
	}
}

int Parser::getNextItem(int item_num)
{
	int i,j;
	for (i = 0; i < item_num; i++) {
		for (j = 0; j < items[i].size(); j++) {
			if (!(items[i][j] == items[item_num][j]))
				break;
		}
		if (j == items[i].size())
			return i;
	}
	return item_num;
}

void Parser::goToActions(set<char>symbol_set, int id)
{
	// �����ķ��е�ÿ�����ţ����������������Ŀ�������ĺ����Ŀ
	bool has_trans = false;
	for (auto now_ch : symbol_set) {
		has_trans = false;
		ITEMS production;
		for (int j = 0; j < items[id].size(); j++) {	// ������Ŀ�е�ÿһ������ʽ��������ǰ�ַ����Ƿ��ת��
			int no = items[id][j].no, now = items[id][j].now;
			if (now_ch == g_production[no].second[now]) {	// ���ƥ�䣬�ò���ʽ��Բ�����һλ��չ��������
				production = { no,now, items[id][j].outlook };
				if (!inItem(production, item_num)) {	// ������item_num����ʵ�����´�����һ����Ŀ
					has_trans = true;
					items[item_num].push_back(production);	// �ѵ�ǰ����ʽ�ŵ��µ���Ŀ��
				}
			}
		}
		if (!has_trans)	// û�в����µĲ���ʽ
			continue;
		getClosure(item_num);	// ����µõ�����Ŀ�ıհ�
		int next_item = getNextItem(item_num);
		TRANSFER_RELATION transfer_relation;
		transfer_relation = { id, next_item, now_ch };
		trans.push_back(transfer_relation);
		if (next_item < item_num) // ���next_item�������һ����˵������Ŀ�Ѿ�����Ŀ���У�����Ҫ�����µ���Ŀ����items���һ�����
			items[item_num].clear();
		else
			item_num++;
	}
}

void Parser::getItemSet()
{
	// ��һ�����ع��ķ���
	items[0].push_back({ 0,1,'#' });	
	item_num++;
	getClosure(0);

	// ֮�󲻶ϴ����������Ŀ��ֱ������������Ŀ����
	for (int i = 0; i < item_num; i++) {
		goToActions(n_terminal, i);
		goToActions(terminal, i);
	}
}

void Parser::writeItems()
{
	for (int i = 0; i < item_num; i++) {
		items_file << "-------------------------I" << i + 1 << "-------------------------" << endl;
		for (int j = 0; j < items[i].size(); j++) {
			int no = items[i][j].no, now = items[i][j].now;
			char symbol = g_production[no].first;
			string production = g_production[no].second;
			production.insert(now, "��");
			items_file << symbol << "->" << production << ",    outlook: " << items[i][j].outlook << endl;
		}
	}
}

void Parser::getActionTable()
{
	// ����ACTION��
	action_table = vector<vector<ACTION>>(N);
	state_num = item_num;

	// ��ȡ��Լ��Ŀ
	for (int i = 0; i < state_num; i++) {
		for (int j = 0; j < items[i].size(); j++) {
			int no = items[i][j].no, now = items[i][j].now;
			if (now == g_production[no].second.size() || g_production[no].second[now] == EPSILON)
				// ˵�������� A->a���Ĺ�Լ��Ŀ�������� A->��@����Ŀ�������ֿ���ֱ�ӹ�Լ
				action_table[i].push_back({items[i][j].outlook, -no});	// ע�����︺�ű�ʾ��Լ��ʹ���ķ��б��Ϊno�Ĳ���ʽ���й�Լ
		}
	}

	// ��ȡ�ƽ���Ŀ
	for (int i = 0; i < trans.size(); i++)
		action_table[trans[i].src].push_back({ trans[i].ch, trans[i].des });
}

void Parser::writeActionTable()
{
	for (int i = 0; i < state_num; i++)
		for (int j = 0; j < action_table[i].size(); j++)
			if(action_table[i][j].next_state > 0)
				action_file << "(" << i << "," << action_table[i][j].ch << "," << action_table[i][j].next_state << ", �ƽ� )" << endl;
			else
				action_file << "(" << i << "," << action_table[i][j].ch << "," << -action_table[i][j].next_state << ", ��Լ )" << endl;
}

void Parser::parseAnalyser(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file)
{
	openFile(grammar_file, lexical_file, items_file, action_file, first_set_file, procedure_file);
	getGrammer();
	getFirst();
	writeFirst();
	getItemSet();
	writeItems();
	getActionTable();
	writeActionTable();

	/* ������ */

	closeFile();
}
