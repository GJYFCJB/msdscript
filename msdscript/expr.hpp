//
// Created by 谷金 on 1/17/22.
//

#ifndef expr_hpp
#define expr_hpp
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;
class expr{
public:
    virtual bool equals(expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual expr* subst(string s1, expr *e) = 0;
};

class Num : public expr {
public:
    int val;

    Num(int val) ;

    bool equals(expr *e);

    int interp();

    bool has_variable();

    expr* subst(string s1, expr *e);
};

class Add : public expr {
public:
    expr *lhs;
    expr *rhs;

    Add(expr *lhs, expr *rhs);

    bool equals(expr *e);

    int interp();

    bool has_variable();

    expr* subst(string s1, expr *e);
};

class Mult : public expr {
public:
    expr *lhs;
    expr *rhs;

    Mult(expr* lhs, expr *rhs);

    bool equals(expr *e);

    int interp();

    bool has_variable();

    expr* subst(string s1, expr *e);
};

class Variable : public expr{
public:
    string s;

    Variable(string input);

    bool equals(expr *e);

    int interp();

    bool has_variable();

    expr* subst(string s1, expr *e);
};


#endif //MSDSCRIPT_EXPR_HPP
