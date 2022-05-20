void Executer::execute(vector<Lex> & poliz){
    Lex pc_el;
    stack<int> args_int;
    stack<string> args_str;
    int lst_stack; 
    int i, j, index = 0, size = poliz.size();
    string i_str, j_str;
    while(index < size){
        pc_el = poliz[index];
        switch(pc_el.get_type()){
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: 
            case POLIZ_ADDRESS: case POLIZ_LABEL: 
                args_int.push(pc_el.get_value_int());
                lst_stack = 0;
                break;

            case LEX_STR:
                args_str.push(pc_el.get_value_string());
                lst_stack = 1;
                break;

            case LEX_ID:
                i = pc_el.get_value_int();
                if(TID[i].get_assign()){
                    if(TID[i].get_type() == LEX_STRING){
                        args_str.push(TID[i].get_value_string());
                        lst_stack = 1;
                    }
                    else{
                        args_int.push(TID[i].get_value_int());
                        lst_stack = 0;
                    }
                }
                else
                  throw "POLIZ: indefinite identifier";
                break;

            case LEX_NOT:
                from_st(args_int, i);
                args_int.push(!i);
                lst_stack = 0;
                break;

            case LEX_OR:
                from_st(args_int, i); 
                from_st(args_int, j);
                args_int.push(j || i);
                lst_stack = 0;
                break;
 
            case LEX_AND:
                from_st(args_int, i);
                from_st(args_int, j);
                args_int.push(j && i);
                lst_stack = 0;
                break;
 
            case POLIZ_GO:
                from_st(args_int, i);
                index = i - 1;
                break;
 
            case POLIZ_FGO:
                from_st(args_int, i);
                from_st(args_int, j);
                if(!j) index = i - 1;
                break;
 
            case LEX_WRITE:
                if(lst_stack){
                    from_st(args_str, i_str);
                    cout << i_str << endl;
                }
                else{
                    from_st(args_int, i);
                    cout << i << endl;
                }
                break;
 
            case LEX_READ:
                int k;
                from_st(args_int, i);
                if(TID[i].get_type () == LEX_INT){
                    cout << "Input int value for" << TID[i].get_name () << endl;
                    cin >> k;
                    TID[i].put_value_int(k);
                    TID[i].put_assign();
                }
                else if(TID[i].get_type() == LEX_BOOLEAN){
                    string s;
                    while (1) {
                        cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                        cin >> s;
                        if ( s != "true" && s != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( s == "true" ) ? 1 : 0;
                        break;
                        TID[i].put_value_int(k);
                        TID[i].put_assign();
                    }
                }
                else{
                    string s;
                    cout << "Input string value for" << TID[i].get_name() << endl;
                    cin >> s;
                    TID[i].put_value_string(s);
                    TID[i].put_assign();
                }
                break;
 
            case LEX_PLUS:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_str, j_str);
                    args_str.push(j_str + i_str);
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    args_int.push(i + j);
                }
                break;
 
            case LEX_TIMES:
                from_st(args_int, i);
                from_st(args_int, j);
                args_int.push(i * j);
                lst_stack = 0;
                break;
 
            case LEX_MINUS:
                from_st(args_int, i);
                from_st(args_int, j);
                args_int.push(j - i);
                lst_stack = 0;
                break;
 
            case LEX_SLASH:
                from_st(args_int, i);
                from_st(args_int, j);
                if(i){
                    args_int.push(j / i);
                    lst_stack = 0;
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
 
            case LEX_DEQ:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_str, j_str);
                    args_int.push(i_str == j_str);
                    lst_stack = 0;
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    args_int.push(i == j);
                }
                break;
 
            case LEX_LSS:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_str, j_str);
                    args_int.push(j_str < i_str);
                    lst_stack = 0;
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    args_int.push(j < i);
                }
                break;
 
            case LEX_GTR:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_str, j_str);
                    args_int.push(j_str > i_str);
                    lst_stack = 0;
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    args_int.push(j > i);
                }
                break;
 
            case LEX_LEQ:
                from_st(args_int, i);
                from_st(args_int, j);
                args_int.push(j <= i);
                lst_stack = 0;
                break;
 
            case LEX_GEQ:
                from_st(args_int, i);
                from_st(args_int, j);
                args_int.push(j >= i);
                lst_stack = 0;
                break;
 
            case LEX_NEQ:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_str, j_str);
                    args_int.push(j_str != i_str);
                    lst_stack = 0;
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    args_int.push(j != i);
                    lst_stack = 0;
                }
                break;

            case LEX_UMINUS:
                from_st(args_int, i);
                args_int.push(-i);
                lst_stack = 0;
                break;

            case LEX_EQ:
                if(lst_stack){
                    from_st(args_str, i_str);
                    from_st(args_int, j);
                    TID[j].put_value_string(i_str);
                    TID[j].put_assign(); 
                    args_str.push(i_str);
                }
                else{
                    from_st(args_int, i);
                    from_st(args_int, j);
                    TID[j].put_value_int(i);
                    TID[j].put_assign(); 
                    args_int.push(i);
                }
                break;
            default:
                throw "POLIZ: unexpected elem";
        }
        ++index;
    };
    cout << "Errors: 0. Finished." << endl;
}
