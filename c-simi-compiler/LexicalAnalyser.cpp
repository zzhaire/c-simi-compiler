#include"LexicalAnalyser.h"
#include"common.h"
void Lexer::Init()
{
	lexer_map["int"] = INT;
	lexer_map["void"] = VOID;
	lexer_map["return"] = RETURN;
	lexer_map["if"] = IF;
	lexer_map["main"] = ID;
	lexer_map["while"] = WHILE;
	lexer_map["else"] = ELSE;
	lexer_map[";"] = SEMICO;
	lexer_map[","] = COMMA;
	lexer_map["$"] = ENDFILE;
	// 运算符
	lexer_map["+"] = ADD;
	lexer_map["-"] = MINUS;
	lexer_map["*"] = MULT;
	lexer_map["/"] = DIV;
	lexer_map["="] = ASSIGN;
	// 比较符
	lexer_map[">"] = GT;
	lexer_map["<"] = LT;
	lexer_map[">="] = GTE;
	lexer_map["<="] = LTE;
	lexer_map["=="] = EQ;
	// 括号
	lexer_map["{"] = LBRACE;
	lexer_map["}"] = RBRACE;
	lexer_map["("] = LPAREN;
	lexer_map[")"] = RPAREN;
	// 集合
	for (int i = 0; i < KN; i++)
		key_words.insert(KEY_WORDS[i]);
	// 初始化单目运算符集合
	for (int i = 0; i < MON; i++)
		mo_operators.insert(MO_OPT[i]);
	// 初始化双目运算符集合
	for (int i = 0; i < BIN; i++)
		bi_operators.insert(BI_OPT[i]);
	// 初始化界符集合
	for (int i = 0; i < SEPN; i++)
		seperators.insert(SEP_OPT[i]);
	memset(DFA_transfer_states, -1, sizeof DFA_transfer_states);
	for (int i = 0; i < NN; i++)
		DFA_is_final[i] = false;

}
bool Lexer::isKeyWord(string s)
{
	return key_words.count(s);
}
bool Lexer::isBiOperator(string s)
{
	return bi_operators.count(s);
}
bool Lexer::isMoOperator(char c)
{
	return mo_operators.count(c);
}
bool Lexer::isSeperator(char c)
{
	return seperators.count(c);
}
bool Lexer::isFinalState(State st)
{
	return st.count(FINAL_STATE);
}
bool Lexer::inFinalState(char c)
{
	return final_state.count(c);
}
bool Lexer::inState(char x)
{
	return state.count(x);
}
bool Lexer::inState(char x, State st)
{
	return st.count(x);
}
int Lexer::getDFAStateIdx(State now)
{
	for (int i = 0; i < DFA_states_n; i++)
	{
		if (now == DFA_states[i])
			return i;
	}
	return -1;
}
State Lexer::getEpsilonClosure(const State st) // 求给定子集的epsilon闭包
{
	State ans = st;		// 赋值当前状态集合
	char tmp_state[NN];	// 临时状态记录当前集合中元素(方便遍历)
	int idx = 0;

	for (auto ch : st)
		tmp_state[idx++] = ch;

	for (int i = 0; i < idx; i++)	// 遍历集合中所有字符
		for (auto ch : transfer_states[tmp_state[i]][EPSILON])	// 找到每一个字符经过epsilon后可以到达的状态
			if (!inState(ch, st))	// 当前字符是否已经在集合中
			{
				tmp_state[idx++] = ch;	// !这句话重要,是一个不断求闭包的过程
				ans.insert(ch);
			}
	return ans;	// 返回求出epsilon闭包后的集合
}
vector<Word>& Lexer::getRes()
{
	return res;
}
void Lexer::buildNFA(string grammer_file)
{
	int grammer_n;	// 产生式数量
	char left;		// 产生式左部
	string  right;	// 产生式右部
	char _;			// 分隔符占位符
	ifstream in;	// 读入文件,使用流对象
	in.open(grammer_file);
	/* 处理文件 */
	in >> grammer_n;	// 读入文法产生式个数
	for (int i = 0; i < grammer_n; i++)
	{
		in >> left >> _ >> _ >> right;	// 读取一个产生式
		if (!i)
			DFA_start_state = left;		// 第一个产生式的左部是起始变元
		if (!inState(left))
			state.insert(left);		// 如果没出现过,将该变元加入NFA集合中
		if (!inFinalState(right[0]))
			final_state.insert(right[0]);	// 右部只有第一个字符可能是终结符,如果是则加入终结符集合
		if (right.length() > 1)
			transfer_states[left][right[0]].insert(right[1]);
		else
			transfer_states[left][right[0]].insert(FINAL_STATE);
	}
	in.close();
}
void Lexer::convertToDFA()	// 使用子集构造法转为DFA
{
	DFA_states_n = 0;
	State now_state, next_state;    // 记录两个状态
	queue<State> state_queue;	// 需要处理的子集队列

	/* 现在开始处理子集 */
	now_state.insert(DFA_start_state);	// 得到初始状态的克林闭包
	now_state = getEpsilonClosure(now_state);
	state_queue.push(now_state);
	DFA_states[DFA_states_n++] = now_state;

	if (isFinalState(now_state))
		DFA_is_final[DFA_states_n - 1] = true;

	/* 开始扩充 */
	while (!state_queue.empty())
	{
		now_state = state_queue.front();
		state_queue.pop();

		for (auto trans_ch : final_state)
		{
			for (auto now_ch : now_state)
				for (auto next_ch : transfer_states[now_ch][trans_ch])
					if (!inState(next_ch, next_state))
						next_state.insert(next_ch);

			next_state = getEpsilonClosure(next_state);
			int next_id = getDFAStateIdx(next_state);
			int now_id = getDFAStateIdx(now_state);
			if (next_state.size() && next_id == -1)
			{
				state_queue.push(next_state);
				DFA_states[DFA_states_n++] = next_state;
				if (isFinalState(next_state))
					DFA_is_final[DFA_states_n - 1] = true;
			}
			if (next_state.size() && next_id != -1 && trans_ch != EPSILON)
				DFA_transfer_states[now_id][trans_ch] = next_id;
			next_state.clear();
		}
	}
}
bool Lexer::canReachFinal(string str)
{
	char cur_state = 0;
	for (int i = 0; i < (int)str.length(); i++)
	{
		cur_state = DFA_transfer_states[cur_state][str[i]];
		if (cur_state == -1)
			return false;
	}
	if (DFA_is_final[cur_state] == true)
		return true;
	return false;
}
void Lexer::inputFile(string file_name)
{
	code = fopen(file_name.c_str(), "r+");
	if (code == nullptr)
	{
		cout << "�ļ�" << file_name << "��ʧ��" << endl;
		exit(0);
	}
	char ch;
	int ptr;
	int keyFlag;
	ch = fgetc(code);
	bool overFlag = false;
	while (!overFlag)
	{
		keyFlag = -1;
		ptr = 0;
		// 检测注释
		if (ch == '/') {
			char next_ch = fgetc(code); // 获取下一个字符以检测注释
			if (next_ch == '/') { // 处理单行注释
				while (ch != '\n' && ch != EOF) { // 跳过直到行尾或文件结束
					ch = fgetc(code);
				}
			}
			else if (next_ch == '*') { // 处理多行注释
				while (true) { // 循环直到找到注释结束
					ch = fgetc(code);
					if (ch == '*' && (next_ch = fgetc(code)) == '/') {
						ch = fgetc(code); // 跳过注释结束后的下一个字符
						break;
					}
				}
			}
			else { // 如果不是注释，则需要处理'/'
				ungetc(next_ch, code); // 把非注释的字符放回去
			}
		}
		if (isdigit(ch)) // 多一个ch
		{
			keyFlag = 1;
			info[ptr++] = ch;
			ch = fgetc(code);
			while (isalpha(ch) || isdigit(ch) || ch == '.' || ch == '+' || ch == '-')
			{
				keyFlag = 1;
				info[ptr++] = ch;
				ch = fgetc(code);
			}
			info[ptr] = '\0';
		}
		if (keyFlag == 1)
		{
			if (isNum(info))
				res.push_back({ info,NUM });
			else
				errorPrint(info, NUM);
			ptr = 0;
			keyFlag = -1;
		}
		if (isalpha(ch))
		{
			keyFlag = 2;
			info[ptr++] = ch;
			ch = fgetc(code);
			while (isalpha(ch) || isdigit(ch))
			{
				keyFlag = 2;
				info[ptr++] = ch;
				ch = fgetc(code);
			}
			info[ptr] = '\0';
		}
		if (keyFlag == 2)
		{
			if (isKeyWord(info))
				res.push_back({ info, lexer_map[info] });
			else
			{
				if (canReachFinal(info))
					res.push_back({ info, ID });
				else
					errorPrint(info, ID);
			}
			ptr = 0;
			keyFlag = -1;
		}
		if (isSeperator(ch))
		{
			string tmp = charToString(ch);
			res.push_back({ tmp, lexer_map[tmp] });
			if ((ch = fgetc(code)) == EOF)
			{
				overFlag = true;
				break;
			}
		}
		if (isMoOperator(ch))
		{
			info[ptr++] = ch;
			if ((ch = fgetc(code)) == EOF)
			{
				overFlag = true;
			}
			info[ptr++] = ch;
			info[ptr] = '\0';
			if (overFlag == false && isBiOperator(info))
			{
				res.push_back({ info, lexer_map[info] });
				ch = fgetc(code);
			}
			else
			{
				string tmp = charToString(info[0]);
				res.push_back({ tmp, lexer_map[tmp] });
			}
			ptr = 0;
		}
		if (ch == '\n')
		{
			// ch = fgetc(fileSource);
			res.push_back({ "nextLine", NEXTLINE });
		}
		if (ch == ' ' || ch == '\n' || ch == '\t')
		{
			if ((ch = fgetc(code)) == EOF)
			{
				overFlag = true;
				break;
			}
			continue;
		}
		memset(info, -1, sizeof info);
	}
	fclose(code);
}
void  Lexer::lexicalAnalyser(string file_name)
{
	Init();
	buildNFA("./LexicalGrammer.txt");
	convertToDFA();
	inputFile(file_name);
}
bool Lexer::outputToFile(string file_src)
{
	ofstream out;
	out.open(file_src);
	if (!out.is_open())
	{
		cout << "打开" << file_src << "文件失败" << endl;
		return 0;
	}
	for (auto& e : res)
		out << e.first << " " << WordTypeName[e.second] << endl;
	out.close();
	return 1;
}
