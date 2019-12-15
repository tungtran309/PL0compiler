#include <bits/stdc++.h>
#include "scanner.h"
#include "function.h"

using namespace std;

Token token;



void term() {
	factor();
	while (token.type == TIMES || token.type == SLASH) {
		token = get_token();
		factor();
	}
}

void expression() {
	if (token.type == PLUS || token.type == MINUS) {
		token = get_token();
	}
	term();
	while (token.type == PLUS || token.type == MINUS) {
		token = get_token();
		term();
	}
}


void factor() {
	if (token.type == IDENT) {
		token = get_token();
		if (token.type == LBRACK) {
			token = get_token();
			expression();
			if (token.type == RBRACK) {
				token = get_token();
			} else {
				error("expected ]");
			}
		}
		return;
	}

	if (token.type == NUMBER) {
		token = get_token();
		return;
	}

	if (token.type == LPARENT) {
		token = get_token();
		expression();
		if (token.type == RPARENT) {
			token = get_token();
		} else {
			error("expected character )");
		}
		return;
	}
	error("factor: syntax error");
}



void condition() {
	expression();
	if (token.type == EQU || token.type == NEQ || token.type == LSS ||   
       	token.type == LEQ || token.type == GTR || token.type == GEQ) {
		token = get_token();
		expression();
		return;
	}
	error("condition: syntax error");
}

void statement() {
	if (token.type == IDENT) {
		token = get_token();
		if (token.type == LBRACK) {
			token = get_token();
			expression();
			if (token.type == RBRACK) {
				token = get_token();
			} else {
				error("expected character )");
			}
		}

		if (token.type == ASSIGN) {
			token = get_token();
			expression();
			return;
		}
		error("expected character := ");
	}

	if (token.type == CALL) {
		token = get_token();
		if (token.type == IDENT) {
			token = get_token();
			if (token.type == LPARENT) {
				token = get_token();
				expression();
				while (token.type == COMMA) {
					token = get_token();
					expression();
				}
				if (token.type == RPARENT) {
					token = get_token();
				} else {
					error("expected character )");
				}
			}
		} else {
			error("expected an IDENT");
		}
		return;
	}

	if (token.type == BEGIN) {
		token = get_token();
		statement();
		while (token.type == SEMICOLON) {
			token = get_token();
			statement();
		}
		if (token.type != END) {
			error("expected END");
		}
		token = get_token();
		return;
	}

	if (token.type == IF) {
		token = get_token();
		condition();
		if (token.type != THEN) {
			error("expected THEN");
		}
		token = get_token();
		statement();
		if (token.type == ELSE) {
			token = get_token();
			statement();
		}
		return;
	}

	if (token.type == WHILE) {
		token = get_token();
		condition();
		if (token.type == DO) {
			token = get_token();
			statement();
		} else {
			error("expected DO");
		}
		return;
	}

	if (token.type == FOR) {
		token = get_token();
		if (token.type != IDENT) {
			error("expected an IDENT");
		}
		token = get_token();
		if (token.type != ASSIGN) {
			error("expected ASSIGN");
		}
		token = get_token();
		expression();
		if (token.type != TO) {
			error("expected TO");
		}
		token = get_token();
		expression();
		if (token.type != DO) {
			error("expected DO");
		}
		token = get_token();
		statement();
		return;
	}
}

void do_block_type1() { //ident = number ,
	token = get_token();
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	token = get_token();
	if (token.type != EQU) {
		error("expected =");
	}
	token = get_token();
	if (token.type != NUMBER) {
		error("expected a NUMBER");
	}
	token = get_token();
}

void do_block_type2() { // ident [number] ,
	token = get_token();
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	token = get_token();
	if (token.type == LBRACK) {
		token = get_token();
		if (token.type != NUMBER) {
			error("expected a NUMBER");
		}
		token = get_token();
		if (token.type != RBRACK) {
			error("expected ]");
		}
		token = get_token();
	}
}

void do_block_type3() { // VAR IDENT ;
	token = get_token();
	if (token.type == VAR) {
		token = get_token();
	}
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	token = get_token();
}

void block() {
	if (token.type == CONST) { // CONST IDENT = NUMBER,
		do_block_type1();
		while (token.type == COMMA) {
			do_block_type1();
		}
		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	if (token.type == VAR) { // VAR IDENT[NUMBER],
		do_block_type2();
		while (token.type == COMMA) {
			do_block_type2();
		}

		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	while(token.type == PROCEDURE) { // PROCEDURE
		token = get_token();
		if (token.type != IDENT) {
			error("expected an IDENT");
		}
		token = get_token();
		if (token.type == LPARENT) {
			do_block_type3();
			while (token.type == SEMICOLON) {
				do_block_type3();
			}
			if (token.type != RPARENT) {
				error("expected character )");
			}
			token = get_token();
		}

		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
		//cout << token.name << endl;
		block();
		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	if (token.type == BEGIN) {
		token = get_token();
		statement();
		while (token.type == SEMICOLON) {
			token = get_token();
			statement();
		}
		if (token.type != END) {
			error("expected END");
		}
		token = get_token();
		return;
	}
	if (token.type == NONE && token.name == "") { // end of 
		return;
	}
	error("block: syntax error");
}



void program(){
	if(token.type == PROGRAM) {
		token = get_token();
		if (token.type == IDENT) {
			token = get_token();
			if(token.type == SEMICOLON){
				token = get_token();
				block();
				if(token.type == PERIOD) {
					cout << "parse successfully" << endl;
				} else error("expected .");

			} else error("expected ;");

		} else error("expected a program name");

	} else error("expected PROGRAM");
}

int main(int argc, char* argv[]) {
	f_in = fopen(argv[1], "r");
	init_scanner();
	token = get_token();
	program();
}