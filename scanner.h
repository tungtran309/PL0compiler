#include <bits/stdc++.h>
#include "const.h"

using namespace std;



string prefix = "";



int count_endline(string prefix) {
	int cnt = 1;
	for (int i = 0; i < prefix.size(); i++) {
		cnt += prefix[i] == '\n';
	}
	return cnt;
}

void error(string message) {
	prefix = prefix.substr(0, prefix.size() - 1);
	cout << prefix << endl;
	cout << "error in line " << count_endline(prefix) << endl;
	cout << message << endl;
	exit(0);
}


struct Token
{
	TokenType type;	//Token nhận dạng được
	int	num;		//Từ vựng khi Token là NUMBER
	string name;
	Token(){};
	Token(TokenType _type, string _name, int _num = 0) {
		type = _type;
		num = _num;
		name = _name;
	}

	bool operator < (const Token &p) const {
		if (type != p.type) {
			return type < p.type;
		}
		if (name != p.name) {
			return name < p.name;
		}
		return num < p.num;
	}
};

Token NULL_TOKEN = Token(NONE, "");

FILE* f_in;
char get_char() {
	char p;
	p = getc(f_in);
	prefix += p;
	return p;
}


bool is_useless(char p) {
	return (p == ' ' || p == '	' || p == '\n' || p == '\t' || p == '\r');
}

char cur;

int get_int(string number) {
	int x = 0;
	for (int i = 0; i < number.size(); i++) {
		x = x * 10 + number[i] - '0';
	}

	return x;
}

string to_upper(string name) {
	for (int i = 0; i < name.size(); i++) {
		name[i] = toupper(name[i]);
	}
	return name;
}


Token get_token() {
	if (is_useless(cur)) {
		while (is_useless(cur)) {
			cur = get_char();
		}
	}

	if (isalpha(cur)) {
		string name = "";
		while (isalpha(cur) || isdigit(cur)) {
			name += cur;
			cur = get_char();
		}
		name = name.substr(0, MAX_IDENT_LEN);
		TokenType key_id = KEYWORDS_ID[to_upper(name)];
		if (key_id) {
			//cout << key_id << " " << name << endl;
			return Token(key_id, name);
		}

		return Token(IDENT, name);
	}

	if (isdigit(cur)) {
		string number = "";
		while (isdigit(cur) || isdigit(cur)) {
			number += cur;
			cur = get_char();
		}
		if (number.size() > MAX_NUMBER_LEN) {
			error((string)"too large number : " + number);
			return NULL_TOKEN;
		}
		int val = get_int(number);
		return Token(NUMBER, number, val);
	}

	if (cur == '(') {
		string name = "(";
        cur = get_char();
        name += cur;
        if (cur == '*') {
            do cur = get_char(), name += cur; while (cur != EOF && cur != '*');
            do cur = get_char(), name += cur; while (cur != EOF && cur != ')');
            cur = get_char();
            return Token(NONE, name);
        }
        return Token(LPARENT, "(");
    }

	if (cur == ':') {
		cur = get_char();
		if (cur == '=') {
			cur = get_char();
			return Token(ASSIGN, ":=");
		}
		error((string)"wrong token : " + cur);
		return NULL_TOKEN;
	}

	if (cur == '<') {
		cur = get_char();
		if (cur == '=') {
			cur = get_char();
			return Token(LEQ, "<=");
		}
		if (cur == '>') {
			cur = get_char();
			return Token(NEQ, "<>");
		}
		return Token(LSS, "<");
	}

	if (cur == '>') {
		cur = get_char();
		if (cur == '=') {
			cur = get_char();
			return Token(GEQ, ">=");
		}
		return Token(GTR, ">");
	}

	TokenType char_id = CHAR_TOKEN[cur];
	if (char_id) {
		string name = "";
		name += cur;
		cur = get_char();
		return Token(char_id, name);
	}
	if (cur != EOF) {
		error((string)"unknown character : " + cur);
	}
	return NULL_TOKEN;

}

void init_scanner() {
	cur = get_char();
	for (int i = 0; i < NUM_KEYWORDS; i++) {
		//cout << TOKEN_STRING[KEYWORDS_TYPE[i]] << endl;
		KEYWORDS_ID[TOKEN_STRING[KEYWORDS_TYPE[i]]] = KEYWORDS_TYPE[i];
	}
    CHAR_TOKEN['+'] = PLUS;
    CHAR_TOKEN['-'] = MINUS;
    CHAR_TOKEN['*'] = TIMES;
    CHAR_TOKEN['/'] = SLASH;
    CHAR_TOKEN['%'] = PERCENT;
    CHAR_TOKEN['='] = EQU;
    CHAR_TOKEN[')'] = RPARENT;
    CHAR_TOKEN['['] = LBRACK;
    CHAR_TOKEN[']'] = RBRACK;
    CHAR_TOKEN[';'] = SEMICOLON;
    CHAR_TOKEN[','] = COMMA;
    CHAR_TOKEN['.'] = PERIOD;
}


/*int main(int argc, char* argv[]) {
	f_in = fopen(argv[1], "r");
	init_scanner();
	cur = get_char();
	while (cur != EOF) {
		Token token = go();
		if (token.name == "") {
			break;
		}
		//set_token.insert(token);
		cout << TOKEN_STRING[token.type];
		if (token.type == IDENT) {
			cout << "(" << token.name << ")";
		}
		if (token.type == NUMBER) {
			cout << "(" << token.num << ")";
		}
		cout << endl;
	}
}*/