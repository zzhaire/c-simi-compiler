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
		/* ������ */
		"IF", "ELSE", "INT", "RETURN", "VOID", "WHILE",
		/* ������������ */
		"ID", "NUM",
		/* �����ַ� */
		/*  {       }         >=     <=    !=     ==      =         */
		"LBRACE", "RBRACE", "GTE", "LTE", "NEQ", "EQ", "ASSIGN",
		// <    >     +        -        *        /      (          )      ;        ,  
		"LT", "GT", "ADD", "MINUS", "MULT", "DIV", "LPAREN", "RPAREN", "SEMI", "COMMA",
		/* ��ע��  ��ע�� */
		"LCOMMENT", "PCOMMENT",
		/*���з�*/
		"NEXTLINE",
		"SEMICO"
};
typedef enum WordType
{
	/* ������־�ʹ����־ */
	ENDFILE, ERROR,
	/* ������ */
	IF, ELSE, INT, RETURN, VOID, WHILE,
	/* ������������ */
	ID, NUM,
	/* �����ַ� */
	/*  {       }    >=   <=   !=   ==    =    <   >    +      -     *     /     (        )     ;     ,   */
	LBRACE, RBRACE, GTE, LTE, NEQ, EQ, ASSIGN, LT, GT, ADD, MINUS, MULT, DIV, LPAREN, RPAREN, SEMI, COMMA,
	/* ��ע��  ��ע�� */
	LCOMMENT, PCOMMENT,
	/*���з�*/
	NEXTLINE,
	SEMICO
} Token;
typedef pair<string, Token> Word;
typedef set<char> State;

bool isNum(string num);
string charToString(char ch);
void errorPrint(const char* info, Token kind);