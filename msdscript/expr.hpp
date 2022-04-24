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

typedef enum {
    prec_none,      // = 0
    prec_let,       // = 1
    prec_add,       // = 2
    prec_mult       // = 3
} precedence_t;
class Val;
class expr{
public:

    /*The value of a number is the number
The value of an addition expression is the sum of the subexpression values
The value of a multiplication expression is the product of the subexpression values*/
    virtual Val* interp() = 0;
    virtual bool equals(expr *e) = 0;
    virtual bool has_variable() = 0;
    virtual expr* subst(string s1, Val *val) = 0;
    virtual void print(ostream &out) = 0;
    virtual void pretty_print(ostream &out);
    virtual void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    virtual string to_string();
    string to_pretty_string();
};

class NumExpr : public expr {
public:
    int rep;
    Val* val;

    NumExpr(int val) ;
    bool equals(expr *e);
    Val * interp();
    bool has_variable();
    expr* subst(string s1, Val *val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
};

class AddExpr : public expr {
public:
    expr *lhs;
    expr *rhs;

    AddExpr(expr *lhs, expr *rhs);
    bool equals(expr *e);
    Val* interp();
    bool has_variable();
    expr* subst(string s1, Val *val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class MultExpr : public expr {
public:
    expr *lhs;
    expr *rhs;

    MultExpr(expr* lhs, expr *rhs);
    bool equals(expr *e);
    Val* interp();
    bool has_variable();
    expr* subst(string s1, Val *val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class VarExpr : public expr{
public:
    string s;

    VarExpr(string input);
    bool equals(expr *e);
    Val* interp();
    bool has_variable();
    expr* subst(string s1, Val *val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class letExpr : public expr{
public:
    VarExpr* variable;
    expr* rhs;
    expr* body;

    letExpr(VarExpr* variable, expr* rhs, expr* body);
    bool equals(expr *e);
    Val* interp();
    bool has_variable();
    expr* subst(string s1, Val *val);
    void print(ostream &out);
    void pretty_print(ostream &out);
    void pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy);
    string to_string();
};

class BoolExpr : public expr{
public:
    bool var;

    BoolExpr(bool var);
    bool equals(expr* expr);
    bool has_variable();
    expr* subst(string s1, Val *val);
    virtual std::string to_string();
    /*The value of a number is the number
    The value of an addition expression is the sum of the subexpression values
    The value of a multiplication expression is the product of the subexpression values*/
    Val* interp();
    void print(ostream &out);

};

class EqualExpr : public expr {
public:
    expr* lhs;
    expr* rhs;

    EqualExpr(expr* lhs, expr* rhs);
    bool equals(expr* other_expr);
    bool has_variable();

    expr* subst(string s1, Val* val);
    virtual std::string to_string();
    Val* interp();
    void print(ostream &out);
};

class IfExpr : public expr {
public:
    expr*  test_part;
    expr*  then_part;
    expr*  else_part;

    IfExpr(expr* test_part, expr* then_part, expr* else_part);
    bool equals(expr* other_expr);
    bool has_variable();

    expr* subst(std::string var, Val* val);
    virtual std::string to_string();
    Val* interp();
    void print(ostream &out);

};

class FunExpr : public expr {
public:
    std::string formal_arg;
    expr* body;

    FunExpr(std::string arg, expr* body);
    bool equals(expr* other_expr);
    bool has_variable();
    Val* interp();
    expr* subst(std::string var, Val* val);
    std::string to_string();
    void print(ostream &out);
};

class CallExpr : public expr {
public:
    expr* to_be_called;
    expr* actual_arg;

    CallExpr(expr* to_be, expr* actual);
    bool equals(expr* other_expr);
    bool has_variable();
    Val* interp();
    expr* subst(std::string var, Val* val);
    std::string to_string();
    void print(ostream &out);
};




#endif //expr_hpp
