#pragma once
#include"common.h"

using namespace std;

const int N = 200;			// ����ʽ������

class Parser {
private:
	/* ����ʽ��ر��� */
	int g_number;						// ��¼�ع��ķ�G'�Ĳ���ʽ����
	pair<char, string>g_production[N];	// ��¼���еĲ���ʽ
	set<char>n_terminal;				// ���ս����
	set<char>terminal;					// �ս����
	
	/* FIRST����ر��� */
	set<char> first[N];		// ���ÿ���ַ���FIRST��
	vector<char> outlooks;	// ��ŵ�ǰ����ʽ������չ����

	/* �ļ���д��� */
	ifstream grammar_file;		// �ķ��ļ�
	ifstream lexical_file;		// �ʷ������������ķ������
	ofstream items_file;		// ��Ŀ��
	ofstream action_file;		// ACTION��
	ofstream first_set_file;	// �ķ������з��ս����First��
	ofstream procedure_file;	// ��������
public:
	int ifTerminal(char ch);	// ��鵱ǰ�ַ��Ƿ�Ϊ�ս��
	void terminalInsert(char ch);	// ��鵱ǰ�ַ�����������Ҫ���ַ����Էŵ����ս�����ս������
	void openFile(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// ����Ҫ���ļ�
	void closeFile();	// �ر������ļ�
	void getGrammer();	// ��ȡ�﷨�����ĸ�ʽ�ļ�
	void getFirst();	// ��ȡ�ķ������з��ս����FIRST��
	void writeFirst();	// �����з��ս����FIRST��д���ı���
	void parseAnalyser(string grammar_file, string lexical_file, string items_file, string action_file, string first_set_file, string procedure_file);	// ���ĺ����������﷨����
};
