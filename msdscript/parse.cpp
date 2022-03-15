//
// Created by 谷金 on 2/6/22.
//

#include "parse.hpp"
#include<cctype>
void consume(std::istream &in, int expect){
    int c = in.get();
    if(c!=expect)
        throw std::runtime_error("consume mismatch");
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    } }

expr* parse_num(std::istream &in)
{
    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n*10 + (c - '0');
        } else
            break; }
    if (negative)
        n = -n;
    return new Num(n);
}

expr* parse_var(std::istream &in){
    string s = "";
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            s += c;
        } else
            break; }
    return new Variable(s);
}

expr* parse_let(std::istream &in){
    skip_whitespace(in);
    string s = "";
    expr* var = parse_var(in);
    Variable* v = dynamic_cast<Variable *>(var);
    s+=v->s;
    skip_whitespace(in);
    in.get();
    skip_whitespace(in);
    expr* e0 = parse_expr(in);
    expr* e1 = parse_expr(in);
    _let *let0;
    let0 = new _let(new Variable(s),e0,e1);
    return let0;
}

std::string parse_keyword(std::istream &in) {
    in.get(); // consume `_`
    std::string name = "_";
    while (1) {
        char c = in.peek();
        if (!isalpha(c))
            break;
        name += in.get();
    }
    return name;
}
//expr *parse_expr(std::istream &in) {
//    skip_whitespace(in);
//    int c = in.peek();
//    if ((c == '-') || isdigit(c))
//        return parse_num(in);
//    else if (c == '(') {
//        consume(in, '(');
//        expr *e = parse_expr(in);
//        skip_whitespace(in);
//        c = in.get();
//        if (c != ')')
//            throw std::runtime_error("missing close parenthesis");
//        return e;
//    } else {
//        consume(in, c);
//        throw std::runtime_error("invalid input");
//    }
//}

//expr *parse_addend(std::istream &in) {
//    skip_whitespace(in);
//    int c = in.peek();
//    if ((c == '-') || isdigit(c))
//        return parse_num(in);
//    else if (c == '(') {
//        consume(in, '(');
//        expr *e = parse_expr(in);
//        skip_whitespace(in);
//        c = in.get();
//        if (c != ')')
//            throw std::runtime_error("missing close parenthesis");
//        return e;
//    } else {
//        consume(in, c);
//        throw std::runtime_error("invalid input");
//    }
//}

expr *parse_expr(std::istream &in) {
    expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}

expr *parse_addend(std::istream &in) {
    expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
}

expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    char c = in.peek();

    if ((c == '-') || isdigit(c))
        return parse_num(in);

    else if (c == '(') {
        consume(in, '(');
        expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    }

    else if(isalpha(c))
        return parse_var(in);

    else if(c=='_'){
        string keyword = parse_keyword(in);
        if(keyword =="_let") return parse_let(in);
        else if(keyword == "_in") {
            skip_whitespace(in);
            return parse_expr(in);
        }
        else {
            throw std::runtime_error((std::string)"unexpected keyword " + keyword);
        }
    }

    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

expr* parse(std::istream &in) {
    expr* e = parse_expr(in);
    skip_whitespace(in);
    char c = in.get();
    if (!in.eof())
        throw std::runtime_error((std::string)"end of file at " + c);
    return e;
}

expr* parse_str(std::string s) {
std::istringstream in(s);
return parse(in);
}



