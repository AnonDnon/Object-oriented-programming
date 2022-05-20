class Parser{
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    string c_str;
    Scanner scan;
    bool flag, flagLbl;
    stack<type_of_lex> st_lex;
    vector<int> Lbl_vector;
    struct s_gt{
        int index;
        int ps;
    } _gt;
    vector<s_gt> goto_vector;
    int _lbl[2];
    void Program();
    void Descriptions();
    void Description();
    void Type();
    void Variable(type_of_lex t_lex);
    void Constant(type_of_lex _t_lex);
    void Integer();
    void Sign();
    void StringConst();
    void BoolConst();
    void Operators();
    void Operator();
    void LabelOp();
    void Expression();
    void CompoundOp();
    void ExpressionOp();
    void E();
    void A();
    void C();
    void PM();
    void TS();
    void N();
    void UPM();
    void F();
    void check_id();
    void check_op ();
    void check_not();
    void check_unary(); 
    void eq_type();
    void eq_boolean();
    void check_id_in_read();
    void check_Lbl();
    void  gl(){
        curr_lex  = scan.get_lex ();
        c_type = curr_lex.get_type ();
        c_val = curr_lex.get_value_int();
        c_str = curr_lex.get_value_string();
    }
public:
    vector<Lex> poliz;
    Parser(const char * program) : scan(program){};
    void analyze();
};

void Parser::analyze(){
    gl();
    Program();
    if(c_type != LEX_FIN) 
        throw curr_lex;
    check_Lbl();
    cout << endl; 
}

void Parser::Program(){
    if(c_type == LEX_PROGRAM){ gl (); }
    else throw curr_lex;     
    if(c_type == LEX_BEGIN){ gl (); }
    else throw curr_lex;  
    Descriptions();
    Operators();
    if(c_type == LEX_END) gl();
    else throw curr_lex;
}

void Parser::Descriptions(){
    while((c_type == LEX_INT) || (c_type == LEX_STRING) || (c_type == LEX_BOOLEAN)){
        Description();
        if(c_type == LEX_SEMICOLON){ gl (); }
        else throw curr_lex; 
    }
}

void Parser::Description(){
    type_of_lex tp = curr_lex.get_type();
    Type();
    Variable(tp);
    while(c_type == LEX_COMMA){
        gl();
        Variable(tp);
    }
}

void Parser::Type(){
    if((c_type == LEX_INT) || (c_type == LEX_STRING) || (c_type == LEX_BOOLEAN))
        gl();
    else throw curr_lex; 
}

void Parser::Variable(type_of_lex t_lex){
    if(c_type == LEX_ID){
        if(TID[curr_lex.get_value_int()].get_declare()) throw "twice";
        else{
            TID[curr_lex.get_value_int()].put_declare();
            TID[curr_lex.get_value_int()].put_type(t_lex);
        } 
        gl();
    }
    else throw curr_lex;
    if(c_type == LEX_EQ){
        gl();
        Constant(t_lex);
        TID[TID.size() - 1].put_assign();
    };
}

void Parser::Constant(type_of_lex _t_lex){
    if(c_type == LEX_STR && _t_lex == LEX_STRING) StringConst();
    else if(((c_type == LEX_TRUE) || (c_type == LEX_FALSE)) && (_t_lex == LEX_BOOLEAN)) BoolConst();
    else if(_t_lex == LEX_INT) Integer();
    else throw "wrong types are in initialization";
}

void Parser::Integer(){
    int sign = 1;
    if(c_type == LEX_PLUS) gl(); 
    else if(c_type == LEX_MINUS){ sign = -1; gl(); }
    if(c_type == LEX_NUM){ 
        TID[TID.size() - 1].put_value_int(c_val * sign);
        gl();
    }
    else throw curr_lex;
}

void Parser::StringConst(){
    if(c_type == LEX_STR){
        TID[TID.size() - 1].put_value_string(c_str);
        gl();
    }
    else throw curr_lex;
}

void Parser::BoolConst(){
    if(c_type == LEX_TRUE){
        TID[TID.size() - 1].put_value_int(1);
        gl();
    }
    else if(c_type == LEX_FALSE){
        TID[TID.size() - 1].put_value_int(0);
        gl();
    }
    else throw curr_lex;
}

void Parser::Operators(){
    while(c_type != LEX_END) Operator();
}

void Parser::Operator(){
    flag = true;
    int pl0, pl1, pl2, pl3;
    if(c_type == LEX_IF){
        gl();
        if(c_type == LEX_LPAREN) gl();
        else throw curr_lex;
        Expression();
        eq_boolean();
        pl0 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        st_lex.pop();
        if(c_type == LEX_RPAREN) gl();
        else throw curr_lex;
        Operator();
        if(c_type == LEX_ELSE){
            pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            poliz[pl0] = Lex(POLIZ_LABEL, poliz.size());
            gl();
            Operator();
            poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
        }
        else poliz[pl0] = Lex(POLIZ_LABEL, poliz.size());
    }
    else if(c_type == LEX_WHILE){
        gl();
        if(c_type == LEX_LPAREN) gl();
        else throw curr_lex;
        pl3 = poliz.size();
        Expression();
        eq_boolean();
        st_lex.pop();
        pl2 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        if(c_type == LEX_RPAREN) gl();
        else throw curr_lex;
        Operator();
        poliz.push_back(Lex(POLIZ_LABEL, pl3));
        poliz.push_back(Lex(POLIZ_GO));
        poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
    }
    else if(c_type == LEX_READ){
        gl();
        if(c_type == LEX_LPAREN) gl();
        else throw curr_lex;
        if(c_type == LEX_ID){
            check_id_in_read();
            poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
            gl();
        }
        else throw curr_lex;
        if(c_type == LEX_RPAREN) gl();
        else throw curr_lex;
        poliz.push_back(Lex(LEX_READ));
        if(c_type == LEX_SEMICOLON) gl();
        else throw curr_lex;
    }
    else if(c_type == LEX_WRITE){
        gl();
        if(c_type == LEX_LPAREN) gl();
        else throw curr_lex;
        Expression();
        poliz.push_back(Lex(LEX_WRITE));
        while(c_type == LEX_COMMA){
            gl();
            Expression();
            poliz.push_back(Lex(LEX_WRITE));
        };
        if(c_type == LEX_RPAREN) gl();
        else throw curr_lex;
        if(c_type == LEX_SEMICOLON) gl();
        else throw curr_lex;
    }
    else if(c_type == LEX_BEGIN) CompoundOp();
    else if(c_type == LEX_GOTO){
        gl();
        if(c_type == LEX_ID){
            _gt.index = c_val;
            _gt.ps = poliz.size();
            goto_vector.push_back(_gt);
            poliz.push_back(Lex());
            poliz.push_back(POLIZ_GO);
            gl();
        }
        else throw curr_lex;
        if(c_type == LEX_SEMICOLON) gl();
        else throw curr_lex;
    }
    else ExpressionOp();
}

void Parser::CompoundOp(){
    if(c_type == LEX_BEGIN) gl();
    else throw curr_lex;
    Operators();
    if(c_type == LEX_END) gl();
    else throw curr_lex;
}

void Parser::ExpressionOp(){
    flagLbl = false;
    Expression();
    if(!flagLbl){
        if(c_type == LEX_SEMICOLON){ 
            gl(); 
            st_lex.pop();
        }
        else throw curr_lex;
    }
    else LabelOp();
}

void Parser::LabelOp(){
    if(c_type == LEX_COLON){
        gl();
        if(TID[_lbl[0]].get_declare())
            throw "twice";
        else{
            TID[_lbl[0]].put_declare();
            TID[_lbl[0]].put_value_int(_lbl[1]);
            Lbl_vector.push_back(_lbl[0]);
        }
    }
    else throw curr_lex;
    Operator();
}

void Parser::Expression(){
    int _id;
    E();
    if(c_type == LEX_EQ){
        st_lex.push(c_type);
        if(poliz.back().get_type() == LEX_ID) {
            _id = poliz.back().get_value_int();
            TID[_id].put_assign();
            poliz.pop_back();
            poliz.push_back(Lex(POLIZ_ADDRESS, _id));
        }
        else 
            throw "wrong types are in =";
        gl(); 
        Expression(); 
        eq_type();
        poliz.push_back(Lex(LEX_EQ));
    }
}

void Parser::E(){
    A();
    while(c_type == LEX_OR){
        st_lex.push(c_type);
        gl(); 
        A(); 
        check_op();
    }
}

void Parser::A(){
    C();
    while((c_type == LEX_AND)){
        st_lex.push(c_type);
        gl(); 
        C(); 
        check_op();
    }
}

void Parser::C(){
    PM();
    while((LEX_LSS <= c_type) || (c_type >= LEX_DEQ)){
        st_lex.push(c_type);
        gl(); 
        PM(); 
        check_op();
    }
}

void Parser::PM(){
    TS();
    while((c_type == LEX_PLUS) || (c_type == LEX_MINUS)){
        st_lex.push(c_type);
        gl(); 
        TS(); 
        check_op();
    }
}

void Parser::TS(){
    F();
    while((c_type == LEX_TIMES) || (c_type == LEX_SLASH)){
        st_lex.push ( c_type );
        gl(); 
        F(); 
        check_op();
    }
}

void Parser::F(){ 
    if(c_type == LEX_NOT){
        gl();
        F();
        eq_boolean();
    }
    else if(c_type == LEX_PLUS){
        gl();
        F();
        check_unary();
    } 
    else if(c_type == LEX_MINUS){
        gl();
        F();
        check_unary();
        poliz.push_back(Lex(LEX_UMINUS));
    } 
    else if(c_type == LEX_ID){
        if(TID[c_val].get_declare()){
            st_lex.push(TID[c_val].get_type());
            poliz.push_back(curr_lex);
            gl();
        }
        else{
            _lbl[0] = c_val;
            _lbl[1] = poliz.size();
            gl();
            if(!flag || c_type != LEX_COLON) throw "not declared";
            flagLbl = true;
        }        
    }
    else if(c_type == LEX_NUM){
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        gl ();
    }
    else if(c_type == LEX_STR){
        st_lex.push(LEX_STRING);
        poliz.push_back(curr_lex);
        gl ();
    }
    else if(c_type == LEX_TRUE){
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    }
    else if(c_type == LEX_FALSE){
        st_lex.push(LEX_BOOLEAN);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl ();
    }
    else if(c_type == LEX_LPAREN){
        gl(); 
        Expression();
        if(c_type == LEX_RPAREN)
            gl ();
        else 
            throw curr_lex;
    }
    else throw curr_lex;
    flag = false; 
}

void Parser::check_id(){
    if(TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else 
        throw "not declared";
}

void Parser::check_op(){
    type_of_lex t1, t2, op, r = LEX_NULL;
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
    if(t1 == LEX_INT && t2 == LEX_INT){
        if(op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH)
            r = LEX_INT;
        if(LEX_LSS <= op || op <= LEX_EQ)
            r = LEX_BOOLEAN;
    }
    if(t1 == LEX_BOOLEAN && t2 == LEX_BOOLEAN){
        if (op == LEX_OR || op == LEX_AND)
            r = LEX_BOOLEAN;
    }
    if(t1 == LEX_STRING && t2 == LEX_STRING){
        if(op == LEX_PLUS)
            r = LEX_STRING;
        if(LEX_LSS <= op || op <= LEX_EQ)
            r = LEX_BOOLEAN;
    }
    if(r == LEX_NULL) throw "wrong types are in operation";
    st_lex.push(r);
    poliz.push_back(Lex(op));
}

void Parser::check_not(){
    if(st_lex.top() != LEX_BOOLEAN)
        throw "wrong type is in not";
    else  
        poliz.push_back(Lex(LEX_NOT));
}

void Parser::check_unary(){
    if(st_lex.top() != LEX_INT)
        throw "wrong type is in unary";
}

void Parser::eq_type(){
    type_of_lex t;
    from_st(st_lex, t);
    st_lex.pop(); 
    if(t != st_lex.top())
        throw "wrong types are in =";
}

void Parser::eq_boolean(){
    if(st_lex.top() != LEX_BOOLEAN)
        throw "expression is not boolean";
}

void Parser::check_id_in_read(){
    if(!TID[c_val].get_declare())
        throw "not declared";
    if(TID[c_val].get_type() == LEX_BOOLEAN)
        throw "Boolean in read";
}

void Parser::check_Lbl(){
    int lbl_ps;
    vector<int>::iterator it;
    for(s_gt j : goto_vector){
        it = find(Lbl_vector.begin(), Lbl_vector.end(), j.index);
        if(it == Lbl_vector.end())
            throw "not declared";
        else{
            lbl_ps = TID[j.index].get_value_int();
            poliz[j.ps] = Lex(POLIZ_LABEL, lbl_ps);
        }
    }
}
