#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include<iostream>
#include<map>
#include<string>
#include<fstream>
#include<list>
#include<set>
#include<queue>
using namespace std;

const char FINAL_STATE = '#';
const char STACK_SIG = '$';
const char EPSILON = '@';
const char END_SIGN = '$';

const string WordTypeName[] = {
		"ENDFILE", "ERROR",
		/* 保留字 */
		"IF", "ELSE", "INT", "RETURN", "VOID", "WHILE",
		/* 变量名和数字 */
		"ID", "NUM",
		/* 特殊字符 */
		/*  {       }         >=     <=    !=     ==      =         */
		"LBRACE", "RBRACE", "GTE", "LTE", "NEQ", "EQ", "ASSIGN",
		// <    >     +        -        *        /      (          )      ;        ,  
		"LT", "GT", "ADD", "MINUS", "MULT", "DIV", "LPAREN", "RPAREN", "SEMI", "COMMA",
		/* 行注释  段注释 */
		"LCOMMENT", "PCOMMENT",
		/*换行符*/
		"NEXTLINE",
		"SEMICO"
};
typedef enum WordType
{
	/* 结束标志和错误标志 */
	ENDFILE, ERROR,
	/* 保留字 */
	IF, ELSE, INT, RETURN, VOID, WHILE,
	/* 变量名和数字 */
	ID, NUM,
	/* 特殊字符 */
	/*  {       }    >=   <=   !=   ==    =    <   >    +      -     *     /     (        )     ;     ,   */
	LBRACE, RBRACE, GTE, LTE, NEQ, EQ, ASSIGN, LT, GT, ADD, MINUS, MULT, DIV, LPAREN, RPAREN, SEMI, COMMA,
	/* 行注释  段注释 */
	LCOMMENT, PCOMMENT,
	/*换行符*/
	NEXTLINE,
	SEMICO
} Token;
typedef pair<string, Token> Word;
typedef set<char> State;

bool isNum(string num);
string charToString(char ch);
void errorPrint(const char* info, Token kind);