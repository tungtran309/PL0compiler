#include <bits/stdc++.h>
#include "scanner.h"
#include "function.h"

using namespace std;

Token token;

vector < Object > stacks;

int cur_scope = 0;

int find_location(string name) { // return id of name in stacks
	for (int i = (int)stacks.size() - 1; i >= 0; i--) {
		if (stacks[i].name == name) {
			return i;
		}
	}
	return -1;
}

bool check_ident(string name, ObjectType type) { // in stacks or not
	int id = find_location(name);
	if (id == -1) {
		return false;
	}
	if (stacks[id].scope != cur_scope) {
		return false;
	}
	if (stacks[id].type == type) {
		return true;
	}
	if (stacks[id].type == T_PROCEDURE || type == T_PROCEDURE) {
		return false;
	}
	return true;
}

void enter(string name, ObjectType type) { // push name, type to stacks
	if (check_ident(name, type)) {
		error(name + " has been declared");
	}
	stacks.push_back(Object(type, token.name, cur_scope));
}

void open_scope() {
	++cur_scope;
}


void show_stack() {
	for (int i = 0; i < stacks.size(); i++) {
		Object p = stacks[i];
		cout << p.name << " " << p.scope << endl;
	}
	cout << endl;
}

void close_scope() {
	int last = (int)stacks.size() - 1;
	for (int i = last; i >= 0; i--) {
		if (stacks[i].scope == cur_scope) {
			stacks.pop_back();
		}
	}
	--cur_scope;
}

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
		int id = find_location(token.name);
		if (id == -1) {
			error(token.name + " was not declared");
		}
		token = get_token();
		if (token.type == LBRACK) {
			if (stacks[id].type != T_ARRAY) {
				error(stacks[id].name + " is not an array");
			}
			token = get_token();
			expression();
			if (token.type == RBRACK) {
				token = get_token();
			} else {
				error("expected ]");
			}
		}

		if (stacks[id].type != T_VAR) {
			error(stacks[id].name + " is not a variable");
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

void check_assign_variable(string name) {
	int id = find_location(name);
	if (id == -1 || stacks[id].type == T_PROCEDURE) {
		error(token.name + " was not declared");
	}
	if (stacks[id].type == T_CONST) {
		error(token.name + " can not change value of a constant");
	}
}

void statement() {
	if (token.type == IDENT) {
		int id = find_location(token.name);
		check_assign_variable(token.name);
		token = get_token();
		if (token.type == LBRACK) {
			if (stacks[id].type != T_ARRAY) {
				error(stacks[id].name + " is not an array");
			}
			token = get_token();
			expression();
			if (token.type == RBRACK) {
				token = get_token();
			} else {
				error("expected character )");
			}
		} else {
			if (stacks[id].type != T_VAR) {
				error(stacks[id].name + " is not a variable");
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
			int id = find_location(token.name);
			if (id == -1 || stacks[id].type != T_PROCEDURE) {
				error("can not find PROCEDURE " + token.name);
			}
			token = get_token();
			int var_number = 0;
			if (token.type == LPARENT) {
				token = get_token();
				expression();
				var_number++;
				while (token.type == COMMA) {
					token = get_token();
					expression();
					var_number++;
				}
				if (token.type == RPARENT) {
					token = get_token();
				} else {
					error("expected character )");
				}
			}
			if (stacks[id].var_number != var_number) {
				error("wrong number of variable in CALL " + stacks[id].name);
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
		//cout << "name : " << token.name << endl;
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
		check_assign_variable(token.name);
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

void declare_const() { //ident = number ,
	token = get_token();
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	enter(token.name, T_CONST);
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


void declare_var() { // ident [number] ,
	token = get_token();
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	enter(token.name, T_VAR);
	token = get_token();
	if (token.type == LBRACK) {
		stacks.back().type = T_ARRAY;
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

void declare_var_procedure() { // VAR IDENT ;
	token = get_token();
	if (token.type == VAR) {
		token = get_token();
	}
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	enter(token.name, T_VAR);
	token = get_token();
}

void block() {
	if (token.type == CONST) { // CONST IDENT = NUMBER,
		declare_const();
		while (token.type == COMMA) {
			declare_const();
		}
		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	if (token.type == VAR) { // VAR IDENT[NUMBER],
		declare_var();
		while (token.type == COMMA) {
			declare_var();
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
		int id = stacks.size();
		enter(token.name, T_PROCEDURE);
		open_scope();
		token = get_token();
		if (token.type == LPARENT) {
			int var_number = 1;
			declare_var_procedure();
			while (token.type == SEMICOLON) {
				var_number++;
				declare_var_procedure();
			}
			stacks[id].set_var_number(var_number);
			if (token.type != RPARENT) {
				error("expected character )");
			}
			token = get_token();
		}

		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
		block();
		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
		close_scope();
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