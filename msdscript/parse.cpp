//
// Created by 谷金 on 2/6/22.
//

#include "parse.hpp"
#include "expr.hpp"

static void consume(std::istream &in, int expect){
    int c = in.get();
    if(c!=expect)
        throw std::runtime_error("consume mismatch");
}

static void skip_whitespace(std::istream &in) {
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

expr *parse_expr(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
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
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

expr *parse_addend(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
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
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

//static expr *parse_expr(std::istream &in) {
//    expr *e;
//    e = parse_addend(in);
//    skip_whitespace(in);
//    int c = in.peek();
//    if (c == '+') {
//        consume(in, '+');
//        expr *rhs = parse_expr(in);
//        return new Add(e, rhs);
//    } else
//        return e;
//}

