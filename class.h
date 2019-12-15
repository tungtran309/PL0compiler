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
	int var_number;
	Object(){};
	Object(ObjectType _type, string _name, int _scope) {
		type = _type;
		name = _name;
		scope = _scope;
		var_number = 0;
	}

	void set_var_number(int _var_number) {
		var_number = _var_number;
	}
};
