
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

struct Object
{
	ObjectType type;
	string name;
	int scope;
	vector < bool > var_type;
	int offset;
	bool is_refer;
	int pos_q;
	Object(){};
	Object(ObjectType _type, string _name, int _scope) {
		type = _type;
		name = _name;
		scope = _scope;
		is_refer = 0;
	}
	Object(ObjectType _type, string _name, int _scope, int _offset) {
		type = _type;
		name = _name;
		scope = _scope;
		offset = _offset;	
		is_refer = 0;
	}

	void set_is_refer(bool _is_refer) {
		is_refer = _is_refer;
	}

	void push_var_type(bool is_refer) {
		var_type.push_back(is_refer);
	}
	
	void set_pos_q(int x) {
		pos_q = x;
	}
};

struct Instruction {
    OpCode op;
    int p;
    int q;
    Instruction(){};
    Instruction(OpCode _op, int _p, int _q){
    	op = _op;
    	p = _p;
    	q = _q;
    }
};

