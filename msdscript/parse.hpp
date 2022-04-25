//
// Created by 谷金 on 2/6/22.
//

#ifndef parse_hpp
#define parse_hpp

#include <sstream>
#include <iostream>
#include "expr.hpp"

PTR(expr)parse_expr(std::istream &in);
PTR(expr)parse_num(std::istream &in);
PTR(expr)parse_var(std::istream &in);
PTR(expr)parse_let(std::istream &in);
PTR(expr)parse_if(std::istream &in);
PTR(expr)parse_inner(std::istream &in);
PTR(expr)parse_comparg(std::istream &in);
PTR(expr)parse_expr(std::istream &in);
PTR(expr)parse_addend(std::istream &in);
void skip_whitespace(std::istream &in);
void consume(std::istream &in, int expect);
PTR(expr)parse_multicand(std::istream &in);
PTR(expr) parse_str(std::string s);
PTR(expr) parse(std::istream &in);
string parse_alphabetic(std::istream &in, std::string prefix);
PTR(expr)parse_fun(std::istream &in);

#endif //CS6015PROJECT_PARSE_HPP
