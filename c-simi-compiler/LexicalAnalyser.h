#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "common.h" 
using namespace std;
/***********const****************/
const char FINAL_STATE = '#';
const char STACK_SIG = '$';
const char EPSILON = '@';
const int KN = 22;
const string KEY_WORDS[KN] = {
	"break", "case", "char", "continue",
	"do", "default", "double", "else",
	"float", "for", "if", "int", "include",
	"long", "main", "return", "switch", "typedef",
	"void", "unsigned", "while", "iostream"
};
// 双字运算符
const int BIN = 12;
const string BI_OPT[BIN] = {
	"++", "--", "&&", "||", "<=", "!=", "==", ">=", "+=", "-=", "*=", "/="
};
//单字运算符
const int MON = 13;
const char MO_OPT[MON] = {
	'+', '-', '*', '/', '!', '%', '~', '&', '|', '^', '=', '>', '<'
};
// 界符
const int SEPN = 8;
const char SEP_OPT[SEPN] = {
	',', '(', ')', '{', '}', ';', '#', '$'
};
const int NN = 200; //DFA状态数量
class Lexer {
private:
	// 用来判断状态
	set<string> key_words;
	set<string> bi_operators;
	State seperators;
	State mo_operators;
	State state;		//中间变量记录state
	State final_state;	//结束状态

	// 文件属性
	FILE* code = NULL;
	map<string, Token> lexer_map;
	vector<Word> res;  // typedef pair<string,WordType> Word;

	char DFA_start_state;	// 初始态是一个字符
	bool DFA_is_final[NN];
	State transfer_states[NN][NN];
	State DFA_states[NN];	//所有的DFA状态
	int DFA_states_n = 0;
	int DFA_transfer_states[NN][NN];

	void Init();
	bool isKeyWord(string s);
	bool isBiOperator(string s);
	bool isMoOperator(char c);
	bool isSeperator(char c);
	bool isFinalState(State st);
	bool inFinalState(char c);
	bool inState(char x);
	bool inState(char x, State st);
	int  getDFAStateIdx(State now);
	State getEpsilonClosure(const State st);
	vector<Word>& getRes();
	void buildNFA(string grammer_file);
	void convertToDFA();
	bool canReachFinal(string str);
	void inputFile(string file_name);
public:
	void lexicalAnalyser(string file_name);
	bool outputToFile(string file_src);
};