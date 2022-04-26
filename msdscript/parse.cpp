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

//parse expr element
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

PTR(expr)parse_num(std::istream &in) {
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
    return NEW(NumExpr)(n);
}

PTR(expr)parse_var(std::istream &in) {
    string s = "";
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            s += c;
        } else
            break;
    }
    return NEW(VarExpr)(s);
}

PTR(expr)parse_let(std::istream &in) {
    skip_whitespace(in);
    string s = "";
    PTR(expr)var = parse_var(in);
    s += var->to_string();
    skip_whitespace(in);
    in.get();
    skip_whitespace(in);
    PTR(expr)e0 = parse_expr(in);
    PTR(expr)e1 = parse_expr(in);
    PTR(letExpr)let0;
    let0 = NEW(letExpr)((s), e0, e1);
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

std::string parse_equal(std::istream &in) {
    in.get(); // consume `_`
    std::string name = "=";
    while (1) {
        char c = in.peek();
        if (isalpha(c) || isxdigit(c) || isspace(c))
            break;
        name += in.get();
    }
    return name;
}

//parse method
PTR(expr)parse(std::istream &in) {
    PTR(expr)e = parse_expr(in);
    skip_whitespace(in);
    char c = in.get();
    if (!in.eof())
        throw std::runtime_error((std::string) "end of file at " + c);
    return e;
}

PTR(expr)parse_expr(std::istream &in) {

    PTR(expr)e = parse_comparg(in);
    char c = peek_after_spaces(in);
    if (c == '=') {
        if (parse_equal(in) == "==") {
            PTR(expr)res = parse_expr(in);
            e = NEW(EqualExpr)(e, res);
        }
    }
    return e;
}

PTR(expr)parse_comparg(std::istream &in) {
    PTR(expr)e = parse_addend(in);

    char c = peek_after_spaces(in);
    if (c == '+') {
        in >> c;
        PTR(expr)rhs = parse_comparg(in);
        e = NEW (AddExpr)(e, rhs);
    }
    return e;
}

PTR(expr)parse_addend(std::istream &in) {

    PTR(expr)e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(expr)rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    } else
        return e;
}

PTR(expr)parse_multicand(std::istream &in) {
    PTR(expr)e = parse_inner(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(expr)actual_arg = parse_expr(in); // try parse inner
        e = NEW(CallExpr)(e, actual_arg);
        consume(in, ')');
    }
    return e;
}

PTR(expr)parse_inner(std::istream &in) {
    PTR(expr)expr;

    char c = peek_after_spaces(in);
    if (c == '(') {
        c = in.get();
        expr = parse_expr(in);
        c = peek_after_spaces(in);
        if (c == ')') {
            c = in.get();
        } else
            throw std::runtime_error("parenthesis need to be closed");
    }
    else if (c == '-' || isdigit(c)) {
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
            return NEW(BoolExpr)(true);
        } else if (keyword == "_false") {
            return NEW(BoolExpr)(false);
        } else if (keyword == "_if") {
            expr = parse_if(in);
        } else if (keyword == "_fun" ){
            expr = parse_fun(in);
        } else {
            throw std::runtime_error((std::string) "unexpected keyword " + keyword);
        }
    }
    else {
        throw std::runtime_error((std::string) "expected a digit or open parenthesis at " + c);
    }
    return expr;
}

PTR(expr)parse_if(std::istream &in) {
    PTR(expr)test_case = parse_expr(in);
    std::string keyword = parse_keyword(in);
    if (keyword != "_then")
        throw std::runtime_error("expected keyword _then");
    PTR(expr)then_case = parse_expr(in);
    keyword = parse_keyword(in);
    if (keyword != "_else")
        throw std::runtime_error("expected keyword _else");
    PTR(expr)else_case = parse_expr(in);
    return NEW(IfExpr)(test_case, then_case, else_case);
}

PTR(expr)parse_fun(std::istream &in) {
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
    PTR(expr) e = parse_expr(in);
    return NEW(FunExpr)(variable, e);
}


PTR(expr)parse_str(std::string s) {
    std::istringstream in(s);
    return parse(in);
}





