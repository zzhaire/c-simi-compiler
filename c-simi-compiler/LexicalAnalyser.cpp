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
	// �����
	lexer_map["+"] = ADD;
	lexer_map["-"] = MINUS;
	lexer_map["*"] = MULT;
	lexer_map["/"] = DIV;
	lexer_map["="] = ASSIGN;
	// �ȽϷ�
	lexer_map[">"] = GT;
	lexer_map["<"] = LT;
	lexer_map[">="] = GTE;
	lexer_map["<="] = LTE;
	lexer_map["=="] = EQ;
	// ����
	lexer_map["{"] = LBRACE;
	lexer_map["}"] = RBRACE;
	lexer_map["("] = LPAREN;
	lexer_map[")"] = RPAREN;
	// ����
	for (int i = 0; i < KN; i++)
		key_words.insert(KEY_WORDS[i]);
	// ��ʼ����Ŀ���������
	for (int i = 0; i < MON; i++)
		mo_operators.insert(MO_OPT[i]);
	// ��ʼ��˫Ŀ���������
	for (int i = 0; i < BIN; i++)
		bi_operators.insert(BI_OPT[i]);
	// ��ʼ���������
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
State Lexer::getEpsilonClosure(const State st) // ������Ӽ���epsilon�հ�
{
	State ans = st;		// ��ֵ��ǰ״̬����
	char tmp_state[NN];	// ��ʱ״̬��¼��ǰ������Ԫ��(�������)
	int idx = 0;

	for (auto ch : st)
		tmp_state[idx++] = ch;

	for (int i = 0; i < idx; i++)	// ���������������ַ�
		for (auto ch : transfer_states[tmp_state[i]][EPSILON])	// �ҵ�ÿһ���ַ�����epsilon����Ե����״̬
			if (!inState(ch, st))	// ��ǰ�ַ��Ƿ��Ѿ��ڼ�����
			{
				tmp_state[idx++] = ch;	// !��仰��Ҫ,��һ��������հ��Ĺ���
				ans.insert(ch);
			}
	return ans;	// �������epsilon�հ���ļ���
}
vector<Word>& Lexer::getRes()
{
	return res;
}
void Lexer::buildNFA(string grammer_file)
{
	int grammer_n;	// ����ʽ����
	char left;		// ����ʽ��
	string  right;	// ����ʽ�Ҳ�
	char _;			// �ָ���ռλ��
	ifstream in;	// �����ļ�,ʹ��������
	in.open(grammer_file);
	/* �����ļ� */
	in >> grammer_n;	// �����ķ�����ʽ����
	for (int i = 0; i < grammer_n; i++)
	{
		in >> left >> _ >> _ >> right;	// ��ȡһ������ʽ
		if (!i)
			DFA_start_state = left;		// ��һ������ʽ��������ʼ��Ԫ
		if (!inState(left))
			state.insert(left);		// ���û���ֹ�,���ñ�Ԫ����NFA������
		if (!inFinalState(right[0]))
			final_state.insert(right[0]);	// �Ҳ�ֻ�е�һ���ַ��������ս��,�����������ս������
		if (right.length() > 1)
			transfer_states[left][right[0]].insert(right[1]);
		else
			transfer_states[left][right[0]].insert(FINAL_STATE);
	}
	in.close();
}
void Lexer::convertToDFA()	// ʹ���Ӽ����취תΪDFA
{
	DFA_states_n = 0;
	State now_state, next_state;    // ��¼����״̬
	queue<State> state_queue;	// ��Ҫ������Ӽ�����

	/* ���ڿ�ʼ�����Ӽ� */
	now_state.insert(DFA_start_state);	// �õ���ʼ״̬�Ŀ��ֱհ�
	now_state = getEpsilonClosure(now_state);
	state_queue.push(now_state);
	DFA_states[DFA_states_n++] = now_state;

	if (isFinalState(now_state))
		DFA_is_final[DFA_states_n - 1] = true;

	/* ��ʼ���� */
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
		cout << "???" << file_name << "?????" << endl;
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
		// ���ע��
		if (ch == '/') {
			char next_ch = fgetc(code); // ��ȡ��һ���ַ��Լ��ע��
			if (next_ch == '/') { // ������ע��
				while (ch != '\n' && ch != EOF) { // ����ֱ����β���ļ�����
					ch = fgetc(code);
				}
			}
			else if (next_ch == '*') { // �������ע��
				while (true) { // ѭ��ֱ���ҵ�ע�ͽ���
					ch = fgetc(code);
					if (ch == '*' && (next_ch = fgetc(code)) == '/') {
						ch = fgetc(code); // ����ע�ͽ��������һ���ַ�
						break;
					}
				}
			}
			else { // �������ע�ͣ�����Ҫ����'/'
				ungetc(next_ch, code); // �ѷ�ע�͵��ַ��Ż�ȥ
			}
		}
		if (isdigit(ch)) // ��һ��ch
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
		cout << "��" << file_src << "�ļ�ʧ��" << endl;
		return 0;
	}
	for (auto& e : res)
		out << e.first << " " << WordTypeName[e.second] << endl;
	out.close();
	return 1;
}
