#pragma once
#include"common.h"

using namespace std;

const int N = 150;			// ����ʽ������

/* LR(1)��Ŀ���ṹ�� */
typedef struct ITEMS {
	int no;	// ��ʾ�ò���ʽ���ķ��еı��
	int now;	// ��ʾԲ�㡤���ڵ�λ�ã�����ʽ�Ҳ���һ��λ��Ϊ0
	char outlook;	// ��ʾչ����
	friend bool operator == (ITEMS item1, ITEMS item2) {	// ����==�����
		return (item1.no == item2.no) && (item1.now == item2.now) && (item1.outlook == item2.outlook);
	}
}ITEMS;


/* ��Ŀ���淶�壬������Ŀ���ת�ƹ�ϵ */
typedef struct TRANSFER_RELATION {
	int src;	// ��ʼ״̬
	int des;	// ת��״̬
	char ch;	// ��ȡ�����ַ�
}TRANSFER_RELATION;


/* ACNTION��ṹ�� */
typedef struct ACTION {
	char ch;	// ��ȡ�����ַ�
	int next_state;	// ������ʾ�ƽ���ת�Ƶ���next_state����Ŀ��������ʾ�õ�|next_state|������ʽ��Լ
}ACTION;


/* �﷨���ṹ�� */
typedef struct TREENODE {
	int id;		// ����Ψһ��ʶ
	char value;	// �ַ�ֵ
	int parent;	// ��ǰ���ĸ���
	vector<int> child;	// ��ǰ�������ж���
	TREENODE(int id, char value) {	// ���캯��
		this->id = id;
		this->value = value;
	}
}TREENODE;

class Parser {
private:
	/* ����ʽ��ر��� */
	int g_number;						// ��¼�ع��ķ�G'�Ĳ���ʽ����
	pair<char, string>g_production[N];	// ��¼���еĲ���ʽ
	set<char>n_terminal;				// ���ս����
	set<char>terminal;					// �ս����
	
	/* FIRST����ر��� */
	set<char> first[N];		// ���ÿ���ַ���FIRST��

	/* ��Ŀ����ر��� */
	vector<char> outlooks;	// ��ŵ�ǰ����ʽ������չ����
	vector<ITEMS> items[N];	// �洢������Ŀ�����в���ʽ
	int item_num;			// ��Ŀ������
	vector<TRANSFER_RELATION> trans;	//��ʾһ����Ŀ���淶���ڵ�ת��

	/* ACTION����� */
	vector<vector<ACTION>> action_table;	// action_table[i][j]��ʾ״̬i�ĵ�j������
	int state_num;			// ��ʾһ���ж��ٸ�״̬������ֵӦ�õ���items_num

	/* ��Լ������� */
	stack<int> state_stack;		// ״̬ջ
	stack<TREENODE> sign_stack;		// ����ջ
	string input;	// ���봮�����ʷ��������ó��Ľ��

	/* �ļ���д��� */
	ifstream grammar_file;		// �ķ��ļ�
	ifstream lexical_file;		// �ʷ������������ķ������
	ofstream items_file;		// ��Ŀ��
	ofstream action_file;		// ACTION��
	ofstream first_set_file;	// �ķ������з��ս����First��
	ofstream procedure_file;	// �洢��������
public:
	int ifTerminal(char ch);	// ��鵱ǰ�ַ��Ƿ�Ϊ�ս��
	void terminalInsert(char ch);	// ��鵱ǰ�ַ�����������Ҫ���ַ����Էŵ����ս�����ս������
	void openFile(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// ����Ҫ���ļ�
	void closeFile();	// �ر������ļ�
	void getGrammer();	// ��ȡ�﷨�����ĸ�ʽ�ļ�
	void getFirst();	// ��ȡ�ķ������з��ս����FIRST��
	void writeFirst();	// �����з��ս����FIRST��д���ı���
	void getItemOutLook(ITEMS production);	// ��ȡ��������ʽ��չ����
	void getClosure(int i);	// �����Ŀ��i�ıհ�
	bool inItem(ITEMS production, int i);	// �жϲ���ʽ�Ƿ��ڵ�ǰ��Ŀ��
	int getNextItem(int item_num);	// �ҵ�items[item_num]��Ŀ��һ�γ��ֵ�λ��
	void goToActions(set<char>symbol_set, int id);	// GOTO��⵱ǰ��Ŀ�ڶ�ȡsymbol_set���ַ��󣬲����ĺ����Ŀ
	void getItemSet();	// ������Ŀ��
	void writeItems();	// �����ɺõ���Ŀ��д���ļ���
	void getActionTable();	// ����ACTION��
	void writeActionTable();	// �����ɺõ�ACTION��д���ļ���
	void writeAnalyzeProcedure(int& step);	// ����������д���ļ�
	bool analyseInputString();	// �����ʷ����������������봮�Ƿ�Ϊ���ķ����ܵĴ�������Լ���̣�
	bool parseAnalyser();	// ���ĺ����������﷨����
};
