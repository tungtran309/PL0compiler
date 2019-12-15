#include <bits/stdc++.h>

using namespace std;

const int MAX_IDENT_LEN = 10;
const int MAX_NUMBER_LEN = 9;
const int NUM_KEYWORDS = 15;
const int MAX_CHAR_VALUE = 128;

typedef enum {//Các loại Token được sử dụng trong PL/0
	NONE = 0, IDENT, NUMBER, BEGIN, CALL, CONST, DO,  ELSE, END, FOR, IF, ODD, PROCEDURE, PROGRAM,
	THEN, TO, VAR, WHILE, PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ, GTR, GEQ, LPARENT, RPARENT, 
	LBRACK, RBRACK, PERIOD, COMMA, SEMICOLON, ASSIGN, PERCENT
} TokenType;

typedef enum {
	T_CONST = 0, T_VAR, T_ARRAY, T_PROCEDURE
} ObjectType;

const TokenType KEYWORDS_TYPE[NUM_KEYWORDS] = {BEGIN, END, IF, THEN, WHILE, DO, CALL, ODD, TO, CONST, VAR, PROCEDURE, PROGRAM, ELSE, FOR};
map < string, TokenType > KEYWORDS_ID;

string TOKEN_STRING[] = {
    "NONE", "IDENT", "NUMBER", "BEGIN", "CALL", "CONST", "DO",  "ELSE", "END", "FOR", "IF", "ODD", "PROCEDURE", "PROGRAM", 
    "THEN", "TO", "VAR", "WHILE", "PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LPARENT", "RPARENT", 
    "LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON", "ASSIGN", "PERCENT"
};

TokenType CHAR_TOKEN[MAX_CHAR_VALUE];