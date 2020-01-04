#include <bits/stdc++.h>
#include "scanner.h"
#include "function.h"

using namespace std;

Token token;

vector < Object > stacks;
stack < int > procedure_mem;
vector < Instruction > code;

int cur_scope = 0, offset = 0;

int insert_code(OpCode op, int p, int q) {
	code.push_back(Instruction(op, p, q));
	return code.size() - 1;
}

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

void show_stack() {
	for (int i = 0; i < stacks.size(); i++) {
		Object p = stacks[i];
		cout << p.name << " " << p.scope << " " << p.offset << endl;
	}
	cout << endl;
}

void enter(string name, ObjectType type) { // push name, type to stacks
	if (check_ident(name, type)) {
		error(name + " has been declared");
	}
	stacks.push_back(Object(type, token.name, cur_scope, offset + 4));
	if (type == T_CONST) {
		insert_code(OP_LC, 0, token.num);
		offset++;
	}
	if (type == T_VAR || type == T_ARRAY) {
		insert_code(OP_INT, 0, 1);
		offset++;
	}
}

void open_scope() {
	++cur_scope;
	procedure_mem.push(offset);
}


void close_scope() {
	int last = (int)stacks.size() - 1;
	for (int i = last; i >= 0; i--) {
		if (stacks[i].scope == cur_scope) {
			stacks.pop_back();
		}
	}
	--cur_scope;
	offset = procedure_mem.top();
	procedure_mem.pop();
}

bool term() {
	int is_refer = factor();
	while (token.type == TIMES || token.type == SLASH) {
		Token op = token;
		token = get_token();
		factor();
		is_refer = false;
		if (op.type == TIMES) {
			insert_code(OP_MUL, 0, 0);
		} else {
			insert_code(OP_DIV, 0, 0);
		}
	}
	return is_refer;
}

bool expression() {
	int is_refer = 1;
	bool is_negative = false;
	if (token.type == PLUS || token.type == MINUS) {
		token = get_token();
		is_refer = false;
		is_negative = (token.type == MINUS);
	}
	is_refer &= term();
	if (is_negative) {
		insert_code(OP_NEG, 0, 0);
	}
	while (token.type == PLUS || token.type == MINUS) {
		bool is_sub = (token.type == MINUS);
		token = get_token();
		is_refer = false;
		term();
		if (is_sub) {
			insert_code(OP_SUB, 0, 0);
		} else {
			insert_code(OP_ADD, 0, 0);
		}
	}
	return is_refer;
}


bool factor() {
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
			insert_code(OP_LA, cur_scope - stacks[id].scope, stacks[id].offset);
			token = get_token();
			expression();
			if (token.type == RBRACK) {
				token = get_token();
			} else {
				error("expected ]");
			}
			insert_code(OP_ADD, 0, 0);
			insert_code(OP_LI, 0, 0);
		} else {
			if (stacks[id].type != T_VAR) {
				error(stacks[id].name + " is not a variable");
			}
			insert_code(OP_LV, cur_scope - stacks[id].scope, stacks[id].offset);
			if (stacks[id].is_refer) {
				insert_code(OP_LI, 0, 0);
			}
			if (stacks[id].type == T_CONST) {
				return false;
			}
		}
		return true;
	}

	if (token.type == NUMBER) {
		insert_code(OP_LC, 0, token.num);
		token = get_token();
		return false;
	}

	if (token.type == LPARENT) {
		token = get_token();
		expression();
		if (token.type == RPARENT) {
			token = get_token();
		} else {
			error("expected character )");
		}
		return false;
	}
	error("factor: syntax error");
}

void condition() {
	expression();
	if (token.type == EQU || token.type == NEQ || token.type == LSS ||   
       	token.type == LEQ || token.type == GTR || token.type == GEQ) {
		Token op = token;
		token = get_token();
		expression();
		if (op.type == EQU) insert_code(OP_EQ, 0, 0);
		if (op.type == NEQ) insert_code(OP_NE, 0, 0);
		if (op.type == LSS) insert_code(OP_LT, 0, 0);
		if (op.type == LEQ) insert_code(OP_LE, 0, 0);
		if (op.type == GTR) insert_code(OP_GT, 0, 0);
		if (op.type == GEQ) insert_code(OP_GE, 0, 0);
		return;
	}
	error("condition: syntax error");
}

Object check_assign_variable(string name) {
	int id = find_location(name);
	if (id == -1 || stacks[id].type == T_PROCEDURE) {
		error(token.name + " was not declared");
	}
	if (stacks[id].type == T_CONST) {
		error(token.name + " can not change value of a constant");
	}
	return stacks[id];
}


void get_address_ident() {
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	int id = find_location(token.name);
	if (id == -1) {
		error(token.name + " was not declared");
	}
	insert_code(OP_LA, cur_scope - stacks[id].scope, stacks[id].offset);
	if (stacks[id].is_refer) {
		code.back().op = OP_LV;
	}
	token = get_token();
	if (token.type == LBRACK) {
		if (stacks[id].type != T_ARRAY) {
			error(stacks[id].name + " is not an array");
		}
		token = get_token();
		expression();
		if (token.type != RBRACK) {
			error("expected ]");
		}
		insert_code(OP_ADD, 0, 0);
		token = get_token();
	} else {
		if (stacks[id].type != T_VAR) {
			error(stacks[id].name + " is not a variable");
		}
	}
}

void statement() {
	if (token.type == IDENT) {
		get_address_ident();
		if (token.type == ASSIGN) {
			token = get_token();
			expression();
			insert_code(OP_ST, 0, 0);
			return;
		}
		error("expected character := ");
	}

	if (token.type == CALL) {
		token = get_token();
		if (token.type == IDENT && token.name != "WRITEI" && token.name != "WRITELN" && token.name != "READI") {
			insert_code(OP_INT, 0, 4);
			int id = find_location(token.name);
			if (id == -1 || stacks[id].type != T_PROCEDURE) {
				error("can not find PROCEDURE " + token.name);
			}
			token = get_token();
			int num_var = 0;
			if (token.type == LPARENT) {
				do {
					if (num_var >= stacks[id].var_type.size()) {
						error("wrong number of variable in CALL " + stacks[id].name);
					}
					token = get_token();
					if (stacks[id].var_type[num_var] == 1) {
						get_address_ident();
					} else {
						expression();
					}
					num_var++;
				}
				while (token.type == COMMA);

				if (token.type == RPARENT) {
					token = get_token();
				} else {
					error("expected character )");
				}
			}
			if (stacks[id].var_type.size() != num_var) {
				error("wrong number of variable in CALL " + stacks[id].name);
			}
			insert_code(OP_DCT, 0, 4 + num_var);
			insert_code(OP_CALL, cur_scope - stacks[id].scope, stacks[id].pos_q);
		} else if (token.type == IDENT && token.name == "WRITEI") {
			token = get_token();
            if(token.type == LPARENT){
                token = get_token();
                expression();
                insert_code(OP_WRI, 0, 0);
                if(token.type == RPARENT){
                    token = get_token();
                }
                else error("expected character )");
            }
            else error("expected character (");
		} else if (token.type == IDENT && token.name == "WRITELN") {
			token = get_token();
			insert_code(OP_WLN, 0, 0);
		} else if (token.type == IDENT && token.name == "READI") {
			token = get_token();
            if (token.type != LPARENT) error("expected (");
            token = get_token();
            get_address_ident();
            insert_code(OP_RI, 0, 0);
            if (token.type != RPARENT) error("expected )");
            token = get_token();
		} else {
			error("expected an IDENT");
		}
		return;
	}

	if (token.type == BEGIN) {
		do {
			token = get_token();
			statement();
		}
		while (token.type == SEMICOLON);
		if (token.type != END) {
			error("expected END");
		}
		token = get_token();
		return;
	}

	if (token.type == IF) {
		token = get_token();
		condition();
		int id_if = insert_code(OP_FJ, 0, 0);
		if (token.type != THEN) {
			error("expected THEN");
		}
		token = get_token();
		statement();
		if (token.type == ELSE) {
			int id_else = insert_code(OP_J, 0, 0);
			code[id_if].q = code.size();
			token = get_token();
			statement();
			code[id_else].q = code.size();
		} else {
			code[id_if].q = code.size();
		}
		return;
	}

	if (token.type == WHILE) {
		int l1 = code.size();
		token = get_token();
		condition();
		int l2 = insert_code(OP_FJ, 0, 0);
		if (token.type == DO) {
			token = get_token();
			statement();
		} else {
			error("expected DO");
		}
		insert_code(OP_J, 0, l1);
		code[l2].q = code.size();
		return;
	}

	if (token.type == FOR) {
		token = get_token();
		if (token.type != IDENT) {
			error("expected an IDENT");
		}
		Object cur_var = check_assign_variable(token.name);
		insert_code(OP_LA, cur_scope - cur_var.scope, cur_var.offset);
		if (cur_var.is_refer) {
			code.back().q = OP_LV;
		}
		insert_code(OP_CV, 0, 0);
		token = get_token();
		if (token.type != ASSIGN) {
			error("expected ASSIGN");
		}
		token = get_token();
		expression();
		insert_code(OP_ST, 0, 0);
		int l1 = insert_code(OP_CV, 0, 0);
		insert_code(OP_LI, 0, 0);
		if (token.type != TO) {
			error("expected TO");
		}
		token = get_token();
		expression();
		insert_code(OP_LE, 0, 0);
		int l2 = insert_code(OP_FJ, 0, 0);
		if (token.type != DO) {
			error("expected DO");
		}
		token = get_token();
		statement();
		insert_code(OP_CV, 0, 0);
		insert_code(OP_CV, 0, 0);
		insert_code(OP_LI, 0, 0);
		insert_code(OP_LC, 0, 1);
		insert_code(OP_ADD, 0, 0);
		insert_code(OP_ST, 0, 0);
		insert_code(OP_J, 0, l1);
		code[l2].q = code.size();
		insert_code(OP_DCT, 0, 1);
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
		offset += token.num - 1;
		insert_code(OP_INT, 0, token.num - 1);
		token = get_token();
		if (token.type != RBRACK) {
			error("expected ]");
		}
		token = get_token();
	}
}

bool declare_var_procedure() { // VAR IDENT ;
	token = get_token();
	bool is_refer = false;
	if (token.type == VAR) {
		token = get_token();
		is_refer = true;
	}
	if (token.type != IDENT) {
		error("expected an IDENT");
	}
	enter(token.name, T_VAR);
	stacks[find_location(token.name)].set_is_refer(is_refer);
	token = get_token();
	return is_refer;
}

void block() {
	if (token.type == CONST) { // CONST IDENT = NUMBER,
		do {
			declare_const();
		}
		while (token.type == COMMA);
		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	if (token.type == VAR) { // VAR IDENT[NUMBER],
		do {
			declare_var();
		}
		while (token.type == COMMA);

		if (token.type != SEMICOLON) {
			error("expected ;");
		}
		token = get_token();
	}

	int l2 = insert_code(OP_J, 0, 0);
	while(token.type == PROCEDURE) { // PROCEDURE
		offset = 0;
		token = get_token();
		if (token.type != IDENT) {
			error("expected an IDENT");
		}
		int id = stacks.size();
		enter(token.name, T_PROCEDURE);
		stacks.back().set_pos_q(code.size());
		open_scope();
		insert_code(OP_INT, 0, 4);
		token = get_token();
		if (token.type == LPARENT) {
			do {
				bool is_refer = declare_var_procedure();
				stacks[id].push_var_type(is_refer);
			}
			while (token.type == SEMICOLON);
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
		insert_code(OP_EP, 0, 0);
	}
	code[l2].q = code.size();
	//cout << "OP_J : " << " " << code.size() << endl;

	if (token.type == BEGIN) {
		do {
			token = get_token();
			statement();
		}
		while (token.type == SEMICOLON);
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
				insert_code(OP_INT, 0, 4);
				block();
				if(token.type == PERIOD) {
					//cout << "parse successfully" << endl;
					insert_code(OP_HLT, 0, 0);
				} else error("expected .");

			} else error("expected ;");

		} else error("expected a program name");

	} else error("expected PROGRAM");
}


void print_code() {
	for (int i = 0; i < code.size(); i++) {
		cout << CODE_STRING[code[i].op] << " ";
		if (NUM_ARGS[code[i].op] == 1) {
			cout << code[i].q << " ";
		}
		if (NUM_ARGS[code[i].op] == 2) {
			cout << code[i].p << " " << code[i].q << " ";
		}
		cout << endl;
	}
}

int main(int argc, char* argv[]) {
	f_in = fopen(argv[1], "r");
	init_scanner();
	token = get_token();
	program();
	print_code();
}