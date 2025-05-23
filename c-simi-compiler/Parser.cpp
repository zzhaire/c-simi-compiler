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
						if (item != EPSILON) 
							if (first[left].insert(item).second) // 判断元素是否成功加入到left的FIRST集，只有当新元素被加入时才需要继续遍历
								continue_flag = true;
					// 如果当前FIRST(right)中包含epsilon，则需要将right下一个字符的FIRST也放到当前非终结符的FIRST中
					if (first[right].count(EPSILON)) {
						is_empty = true;
						right = g_production[i].second[++index];
						continue;
					}
				}
				// 走到这里表明是A->a类型的产生式(a可能是epsilon)，因此可以直接将a加入FIRST(left)
				else
					if (first[left].insert(right).second) //如果新元素被加入到FIRST集，则需要再次遍历
						continue_flag = true; // 执行到这里FIRST(left)已经构建好，不会继续向下找right了
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
			first_set_file << "}" << endl;
		}
	}
}

void Parser::getItemOutLook(ITEMS production)
{
	int out_look_index = production.now + 1;
	while (1) {
		// 遍历到产生式的结尾，准备返回
		if (out_look_index == g_production[production.no].second.size()) {	
			outlooks.push_back(production.outlook);
			return;
		}
		// 如果是终结符，直接加入并退出
		char next_char = g_production[production.no].second[out_look_index];
		if (ifTerminal(next_char) == true) {	
			outlooks.push_back(next_char);
			return;
		}
		// 如果是非终结符，加入其FIRST集
		for (auto ch : first[next_char])
			if (ch != EPSILON)	// 不加入epsilon
				outlooks.push_back(ch);
		if (first[next_char].count(EPSILON) > 0)	// 当前非终结符的FIRST可能产生epsilon，需要检查下一个符号
			out_look_index++;
		else // 否则已经查看完毕，直接返回
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
	for (int j = 0; j < items[i].size(); j++) {	// 遍历当前项目的所有产生式
		int no = items[i][j].no, now = items[i][j].now;
		if (ifTerminal(g_production[no].second[now]) == false)
			// 如果当前右部圆点的下一个位置是非终结符，将所有左部是该非终结符的产生式加入该项目中
			getItemOutLook(items[i][j]);	// 构造当前产生式的展望符
			for (int k = 0; k <= g_number; k++)
				if (g_production[k].first == g_production[no].second[now]) {
					ITEMS production;
					production.no = k, production.now = 0;
					for (int z = 0; z < outlooks.size(); z++) {
						production.outlook = outlooks[z];
						if (!inItem(production, i))	// 如果当前产生式不在项目中，则将其加入当前项目
							items[i].push_back(production);
					}
				}
			// 开始求下一条产生式的闭包
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
	// 对于文法中的每个符号，计算它到来后该项目产生出的后继项目
	bool has_trans = false;
	for (auto now_ch : symbol_set) {
		has_trans = false;
		ITEMS production;
		for (int j = 0; j < items[id].size(); j++) {	// 遍历项目中的每一条产生式，遇到当前字符后是否会转移
			int no = items[id][j].no, now = items[id][j].now;
			if (now_ch == g_production[no].second[now]) {	// 如果匹配，该产生式的圆点后移一位，展望符不变
				production = { no,now + 1, items[id][j].outlook };
				if (!inItem(production, item_num)) {	// 这里是item_num，其实就是新创建了一个项目
					has_trans = true;
					items[item_num].push_back(production);	// 把当前产生式放到新的项目中
				}
			}
		}
		if (!has_trans)	// 没有产生新的产生式
			continue;
		getClosure(item_num);	// 求解新得到的项目的闭包
		int next_item = getNextItem(item_num);
		TRANSFER_RELATION transfer_relation;
		transfer_relation = { id, next_item, now_ch };
		trans.push_back(transfer_relation);
		if (next_item < item_num) // 如果next_item不是最后一个，说明该项目已经在项目集中，不需要增加新的项目，把items最后一个清空
			items[item_num].clear();
		else
			item_num++;
	}
}

void Parser::getItemSet()
{
	// 第一个是拓广文法的
	items[0].push_back({ 0,0,'#' });	
	item_num++;
	getClosure(0);

	// 之后不断处理后续的项目，直到不再有新项目产生
	for (int i = 0; i < item_num; i++) {
		goToActions(n_terminal, i);
		goToActions(terminal, i);
	}
}

void Parser::writeItems()
{
	for (int i = 0; i < item_num; i++) {
		items_file << "-------------------------I" << i  << "-------------------------" << endl;
		for (int j = 0; j < items[i].size(); j++) {
			int no = items[i][j].no, now = items[i][j].now;
			char symbol = g_production[no].first;
			string production = g_production[no].second;
			production.insert(now, "·");
			items_file << symbol << "->" << production << ",    outlook: " << items[i][j].outlook << endl;
		}
	}
}

void Parser::getActionTable()
{
	// 构造ACTION表
	action_table = vector<vector<ACTION>>(N);
	state_num = item_num;

	// 获取归约项目
	for (int i = 0; i < state_num; i++) {
		for (int j = 0; j < items[i].size(); j++) {
			int no = items[i][j].no, now = items[i][j].now;
			if (now == g_production[no].second.size() || g_production[no].second[now] == EPSILON)
				// 说明是形似 A->a·的归约项目，或者是 A->·@的项目，这两种可以直接归约
				action_table[i].push_back({items[i][j].outlook, -no});	// 注意这里负号表示归约，使用文法中编号为no的产生式进行归约
		}
	}

	// 获取移进项目
	for (int i = 0; i < trans.size(); i++)
		action_table[trans[i].src].push_back({ trans[i].ch, trans[i].des });
}

void Parser::writeActionTable()
{
	for (int i = 0; i < state_num; i++)
		for (int j = 0; j < action_table[i].size(); j++)
			if(action_table[i][j].next_state > 0)
				action_file << "(" << i << "," << action_table[i][j].ch << "," << action_table[i][j].next_state << ", 移进 )" << endl;
			else
				action_file << "(" << i << "," << action_table[i][j].ch << "," << -action_table[i][j].next_state << ", 归约 )" << endl;
}

void Parser::writeAnalyzeProcedure(int& step)
{
	procedure_file << step++ << "\t\t" << endl;
	stack<int> tmp_state(state_stack);	// state_stack栈的临时拷贝
	stack<TREENODE> tmp_sign(sign_stack);	// sign_state栈的临时拷贝
	stack<int> reverse_state;	// 状态栈内容翻转，用于正序输出
	stack<TREENODE>reverse_sign;	// 符号栈内容翻转，用于正序输出

	// 翻转state栈和sign栈
	while (!tmp_state.empty()) {
		reverse_state.push(tmp_state.top());
		tmp_state.pop();
	}
	while (!tmp_sign.empty()) {
		reverse_sign.push(tmp_sign.top());
		tmp_sign.pop();
	}
	// 然后正序输出
	procedure_file << '#';
	while (!reverse_state.empty()) {
		procedure_file << reverse_state.top() << ',';
		reverse_state.pop();
	}
	procedure_file << "\t\t";
	while (!reverse_sign.empty()) {
		procedure_file << reverse_sign.top().value;
		reverse_sign.pop();
	}
}

bool Parser::analyseInputString()
{
	ifstream lexical_output;
	lexical_output.open("./grammers/LexicalGrammerProduct.txt");
	//lexical_output.open("./lexical_output.txt");
	int id = 0;
	lexical_output >> input;
	state_stack.push(0);	// 从第0个状态开始分析
	sign_stack.push(TREENODE(id++, '#'));
	int read_pin = 0, step = 0;
	procedure_file << "步骤\t\t状态\t\t符号\t\t输入串\t\tACTION\t\tGOTO" << endl;
	procedure_file << "----------------------------------------------------------------------------------------" << endl;

	/* 开始进行归约分析 */
	while (sign_stack.top().value != 'S') {
		int cur_state = state_stack.top();
		char cur_input = input[read_pin++];
		//bool actionFound = false;

		// 输出分析过程
		writeAnalyzeProcedure(step); // step只在这里面使用，用于记录分析步数 
		int j;
		// 确定下一状态
		for ( j= 0; j < action_table[cur_state].size(); j++) {	// 遍历ACTION表的第i个状态的所有转移
			char cur_char = action_table[cur_state][j].ch;
			int next_state = action_table[cur_state][j].next_state;
			if (cur_char == cur_input) {
				//actionFound = true;
				if (next_state > 0) {	// 移进项目
					procedure_file << "\t\ts" << next_state << endl;
					sign_stack.push(TREENODE(id++, cur_char));
					state_stack.push(next_state);
				}
				else if (next_state == 0) {	// 接受态
					procedure_file << "\t\tAccept" << endl;
					cout << "语法分析通过\n在./products下查看表达式产生文件" << endl;
					return true;
				}
				else {	// 归约项目
					int no = -next_state;	// 得到归约使用的产生式的编号
					int cnt = g_production[no].second.size();	// 需要连续出栈的符号和状态数
					//int child_nums = cnt;	// 该父结点拥有的孩子数
					read_pin--;
					procedure_file << "\t\t" << "r" << no << endl;

					// 连续出栈符号和状态
					vector<TREENODE> nodes;
					while (cnt--) {
						nodes.push_back(sign_stack.top());
						sign_stack.pop();
						state_stack.pop();
					}

					// 建立父节点并添加到栈中
					TREENODE parent(id++, g_production[no].first);
					for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
						parent.child.push_back(it->id);
						it->parent = parent.id;
					}
					sign_stack.push(parent);
					if (parent.value == 'P') {
						//说明是最后一次归约
						nodes.push_back(parent);
					}
					//nodes.push_back(parent);

					// 更新状态栈,根据GOTO表找到下一个状态
					int next_parent_state = -1;
					for (auto action : action_table[state_stack.top()]) {
						if (action.ch == parent.value) {
							next_parent_state = action.next_state;
							break;
						}
					}
					if (next_parent_state != -1) {
						state_stack.push(next_parent_state);  // 压入下一个状态
					}
					else {
						//cout << "parent.value   " << parent.value << "  top  " << state_stack.top() << endl;
						for (auto action : action_table[state_stack.top()]) {
							//cout << "1  " << action.ch << "  2  " << action.next_state << endl;
							if (action.ch == parent.value) {
								next_parent_state = action.next_state;
								break;
							}
						}
						cout << "错误:找不到合适的GOTO动作." << endl;
						return false;
					}
				}
				break;


			}
		}
		if (j == action_table[cur_state].size())
		{
			cout << "ERROR!所给程序不能被编译器接受！" << endl;
			cout << "可能是缺少:\n";
			for (int i = 0; i < action_table[cur_state].size(); i++) {
				cout << action_table[cur_state][i].ch << " ";
			}
			cout << endl;
			exit(-1);
		}
	}
}

void Parser::parseAnalyser()
{
	string grammar_file = "./grammers/parseGrammar.txt";
	string lexical_file = "./products/LexicalProduct.txt";
	string items_file = "./products/itemsResult.txt";
	string action_file = "./products/actionResult.txt";
	string first_set_file = "./products/firstSet.txt";
	string procedure_file = "./products/ParseProduct.txt";
	openFile(grammar_file, lexical_file, items_file, action_file, first_set_file, procedure_file);
	getGrammer();
	getFirst();
	writeFirst();
	getItemSet();
	writeItems();
	getActionTable();
	writeActionTable();
	analyseInputString();
	closeFile();
}
