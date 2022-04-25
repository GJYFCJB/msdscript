//
// Created by 谷金 on 1/17/22.
//

#ifndef expr_hpp
#define expr_hpp

#include "pointer.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

typedef enum {
    prec_none,      // = 0
    prec_let,       // = 1
    prec_add,       // = 2
    prec_mult       // = 3
} precedence_t;
class Val;
class Env;
class expr{
public:

    /*The value of a number is the number
The value of an addition expression is the sum of the subexpression values
The value of a multiplication expression is the product of the subexpression values*/
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual bool equals(PTR(expr)e) = 0;
    virtual bool has_variable() = 0;
    //ignore as we have added environment class
    virtual PTR(expr) subst(string s1, PTR(Val)val) = 0;
    virtual void print(ostream &out) = 0;
    virtual void pretty_print(ostream &out);
    virtual void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    virtual string to_string();
    string to_pretty_string();
};

class NumExpr : public expr {
public:
    int rep;
    PTR(Val) val;

    NumExpr(int val) ;
    bool equals(PTR(expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
};

class AddExpr : public expr {
public:
    PTR(expr)lhs;
    PTR(expr)rhs;

    AddExpr(PTR(expr)lhs, PTR(expr)rhs);
    bool equals(PTR(expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class MultExpr : public expr {
public:
    PTR(expr)lhs;
    PTR(expr)rhs;

    MultExpr(PTR(expr) lhs, PTR(expr)rhs);
    bool equals(PTR(expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class VarExpr : public expr{
public:
    string s;

    VarExpr(string input);
    bool equals(PTR(expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class letExpr : public expr{
public:
    string variable;
    PTR(expr) rhs;
    PTR(expr) body;

    letExpr(string variable, PTR(expr) rhs, PTR(expr) body);
    bool equals(PTR(expr)e);
    PTR(Val) interp(PTR(Env) env);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class BoolExpr : public expr{
public:
    bool var;

    BoolExpr(bool var);
    bool equals(PTR(expr) expr);
    bool has_variable();
    PTR(expr) subst(string s1, PTR(Val)val);
    virtual std::string to_string();
    /*The value of a number is the number
    The value of an addition expression is the sum of the subexpression values
    The value of a multiplication expression is the product of the subexpression values*/
    PTR(Val) interp(PTR(Env) env);
    void print(ostream &out);

};

class EqualExpr : public expr {
public:
    PTR(expr) lhs;
    PTR(expr) rhs;

    EqualExpr(PTR(expr) lhs, PTR(expr) rhs);
    bool equals(PTR(expr) other_expr);
    bool has_variable();

    PTR(expr) subst(string s1, PTR(Val) val);
    virtual std::string to_string();
    PTR(Val) interp(PTR(Env) env);
    void print(ostream &out);
};

class IfExpr : public expr {
public:
    PTR(expr)  test_part;
    PTR(expr)  then_part;
    PTR(expr)  else_part;

    IfExpr(PTR(expr) test_part, PTR(expr) then_part, PTR(expr) else_part);
    bool equals(PTR(expr) other_expr);
    bool has_variable();

    PTR(expr) subst(std::string var, PTR(Val) val);
    virtual std::string to_string();
    PTR(Val) interp(PTR(Env) env);
    void print(ostream &out);

};

class FunExpr : public expr {
public:
    std::string formal_arg;
    PTR(expr) body;

    FunExpr(std::string arg, PTR(expr) body);
    bool equals(PTR(expr) other_expr);
    bool has_variable();
    PTR(Val) interp(PTR(Env) env);
    PTR(expr) subst(std::string var, PTR(Val) val);
    std::string to_string();
    void print(ostream &out);
};

class CallExpr : public expr {
public:
    PTR(expr) to_be_called;
    PTR(expr) actual_arg;

    CallExpr(PTR(expr) to_be, PTR(expr) actual);
    bool equals(PTR(expr) other_expr);
    bool has_variable();
    PTR(Val) interp(PTR(Env) env);
    PTR(expr) subst(std::string var, PTR(Val) val);
    std::string to_string();
    void print(ostream &out);
};




#endif //expr_hpp
