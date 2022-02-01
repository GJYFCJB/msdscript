//
// Created by 谷金 on 1/17/22.
//

#ifndef expr_hpp
#define expr_hpp
#define CATCH_CONFIG_RUNNER
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include "catch.hpp"


using namespace std;

typedef enum {
    prec_none,      // = 0
    prec_let,       // = 1
    prec_add,       // = 2
    prec_mult       // = 3
} precedence_t;

class expr{
public:
    virtual bool equals(expr *e) = 0;
    /*The value of a number is the number
The value of an addition expression is the sum of the subexpression values
The value of a multiplication expression is the product of the subexpression values*/
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual expr* subst(string s1, expr *e) = 0;
    virtual void print(ostream &out) = 0;
    virtual void pretty_print(ostream &out) = 0;
    virtual void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy)= 0;
    string to_string();
    string to_string_p();

};

class Num : public expr {
public:
    int val;

    Num(int val) ;
    bool equals(expr *e);
    int interp();
    bool has_variable();
    expr* subst(string s1, expr *e);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
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
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
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
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class Variable : public expr{
public:
    string s;

    Variable(string input);
    bool equals(expr *e);
    int interp();
    bool has_variable();
    expr* subst(string s1, expr *e);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class _let : public expr{
public:
    Variable* variable;
    expr* rhs;
    expr* body;

    _let(Variable* variable, expr* rhs, expr* body);
    bool equals(expr *e);
    int interp();
    bool has_variable();
    expr* subst(string s1, expr *e);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};


#endif //expr_hpp
