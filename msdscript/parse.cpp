//
// Created by 谷金 on 2/6/22.
//

#include "parse.hpp"
#include<cctype>

void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

char peek_after_spaces(std::istream &in) {
    char c;
    skip_whitespace(in);
    c = in.peek();
    return c;
}

std::string parse_alphabetic(std::istream &in, std::string prefix) {
    std::string name = prefix;
    while (1) {
        char c = in.peek();
        if (!isalpha(c))
            break;
        name += in.get();
    }
    return name;
}

expr *parse_num(std::istream &in) {
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
            n = n * 10 + (c - '0');
        } else
            break;
    }
    if (negative)
        n = -n;
    return new NumExpr(n);
}

expr *parse_var(std::istream &in) {
    string s = "";
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            s += c;
        } else
            break;
    }
    return new VarExpr(s);
}

expr *parse_let(std::istream &in) {

    skip_whitespace(in);
    string s = "";
    expr *var = parse_var(in);
    VarExpr *v = dynamic_cast<VarExpr *>(var);
    s += v->s;
    skip_whitespace(in);
    in.get();
    skip_whitespace(in);
    expr *e0 = parse_expr(in);
    expr *e1 = parse_expr(in);
    letExpr *let0;
    let0 = new letExpr(new VarExpr(s), e0, e1);
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


expr *parse_expr(std::istream &in) {
//    expr *e = parse_comparg(in);
//    char c = peek_after_spaces(in);
//
//    if (c == '=') {
//        c = in.get();
//        c = in.get();
//        if (c == '=') {
//            expr *rhs = parse_expr(in);
//            e = new EqualExpr (e, rhs);
//        }
//    }
//        return e;
    expr *e = parse_comparg(in);
    char c = peek_after_spaces(in);
    if (c == '=') {
        c = in.get();
        c = in.get();
        if (c == '=') {
            expr *res = parse_expr(in);
            e = new(EqualExpr)(e, res);
        }
    }
    return e;
}

expr *parse_comparg(std::istream &in) {
    expr *e = parse_addend(in);

    char c = peek_after_spaces(in);
    if (c == '+') {
        in >> c;
        expr *rhs = parse_comparg(in);
        e = new (AddExpr)(e, rhs);
    }
    return e;
}

expr *parse_addend(std::istream &in) {
//    expr *e;
//    e = parse_multicand(in);
//    char c = peek_after_spaces(in);
//    if (c == '*') {
//        consume(in, '*');
//        expr *rhs = parse_addend(in);
//        e =  new MultExpr(e, rhs);
//    }
//    return e;
    expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    } else
        return e;
}

expr *parse_multicand(std::istream &in) {
    expr *e = parse_inner(in);

    while (in.peek() == '(') {
        consume(in, '(');
        expr *actual_arg = parse_expr(in); // try parse inner
        consume(in, '(');
        e = new(CallExpr)(e, actual_arg);
    }
    return e;
}

expr *parse_inner(std::istream &in) {
    expr *expr;

    char c = peek_after_spaces(in);
    if (c == '(') {
        c = in.get();
        expr = parse_expr(in);
        c = peek_after_spaces(in);
        if (c == ')') {
            c = in.get();
        } else
            throw std::runtime_error("parenthesis need to be closed");
    } else if (c == '-' || isdigit(c)) {
        expr = parse_num(in);
    } else if (isalpha(c)) {
        expr = parse_var(in);
    } else if (c == '_') {
        std::string keyword = parse_keyword(in);
        if (keyword == "_let") {
            expr = parse_let(in);
        } else if (keyword == "_in") {
            c = peek_after_spaces(in);
            expr = parse_expr(in);
        } else if (keyword == "_true") {
            return new(BoolExpr)(true);
        } else if (keyword == "_false") {
            return new(BoolExpr)(false);
        } else if (keyword == "_if") {
            expr = parse_if(in);
        } else {
            throw std::runtime_error((std::string) "unexpected keyword " + keyword);
        }
    } else {
        throw std::runtime_error((std::string) "expected a digit or open parenthesis at " + c);
    }
    return expr;
}

expr *parse_if(std::istream &in) {
    expr *test_case = parse_expr(in);
    std::string keyword = parse_keyword(in);
    if (keyword != "_then")
        throw std::runtime_error("expected keyword _then");
    expr *then_case = parse_expr(in);
    keyword = parse_keyword(in);
    if (keyword != "_else")
        throw std::runtime_error("expected keyword _else");
    expr *else_case = parse_expr(in);
    return new(IfExpr)(test_case, then_case, else_case);
}

expr *parse_fun(std::istream &in) {
    char c = peek_after_spaces(in);
    if (c != '(') {
        throw std::runtime_error("expected an open parenthesis");
    }
    c = in.get();
    std::string variable = parse_alphabetic(in, "");
    c = peek_after_spaces(in);
    if (c != ')') {
        throw std::runtime_error("expected a close parenthesis");
    }
    c = in.get();
    expr* e = parse_expr(in);
    return new(FunExpr)(variable, e);
}


expr *parse(std::istream &in) {
    expr *e = parse_expr(in);
    skip_whitespace(in);
    char c = in.get();
    if (!in.eof())
        throw std::runtime_error((std::string) "end of file at " + c);
    return e;
}

expr *parse_str(std::string s) {
    std::istringstream in(s);
    return parse(in);
}




