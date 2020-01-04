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
    OP_LA, OP_LV, OP_LC, OP_LI, OP_INT, OP_DCT,
    OP_J, OP_FJ, OP_HLT, OP_ST, OP_CALL, OP_EP, OP_EF,
    OP_RC, OP_RI, OP_WRC, OP_WRI, OP_WLN,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_NEG, OP_CV,
    OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE
} OpCode;

int NUM_ARGS[] = {
    2, 2, 1, 0, 1, 1,
    1, 1, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

string CODE_STRING[] = {
	"LA", "LV", "LC", "LI", "INT", "DCT",
    "J", "FJ", "HLT", "ST", "CALL", "EP", "EF",
    "RC", "RI", "WRC", "WRI", "WLN",
    "ADD", "SUB", "MUL", "DIV", "NEG", "CV",
    "EQ", "NE", "GT", "LT", "GE", "LE"
};

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