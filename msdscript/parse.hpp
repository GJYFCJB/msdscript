//
// Created by 谷金 on 2/6/22.
//

#ifndef parse_hpp
#define parse_hpp

#include <sstream>
#include <iostream>
#include "expr.hpp"

expr *parse_expr(std::istream &in);
expr *parse_num(std::istream &in);
expr *parse_var(std::istream &in);
expr *parse_let(std::istream &in);
expr *parse_if(std::istream &in);
expr *parse_inner(std::istream &in);
//expr *parse_expr_(std::istream &in);
expr *parse_expr(std::istream &in);
expr *parse_addend(std::istream &in);
void skip_whitespace(std::istream &in);
void consume(std::istream &in, int expect);
expr *parse_multicand(std::istream &in);
expr* parse_str(std::string s);
expr* parse(std::istream &in);


#endif //CS6015PROJECT_PARSE_HPP
