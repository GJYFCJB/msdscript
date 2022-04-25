//
// Created by 谷金 on 2/16/22.
//

#ifndef CS6015PROJECT_VAL_H
#define CS6015PROJECT_VAL_H

#include <string>
#include "pointer.h"
using namespace std;

class expr;

class Val {
public:
    virtual PTR(expr) to_expr() = 0;
    virtual PTR(Val) multWith (PTR(Val) input) = 0;
    virtual PTR(Val) addTo (PTR(Val) input) = 0;
    virtual bool equals(PTR(Val) input) = 0;
    virtual string to_string() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

class NumVal : public Val{
public:
    int val;
    NumVal(int n);
    virtual PTR(expr) to_expr();
    virtual PTR(Val) addTo (PTR(Val) input);
    virtual PTR(Val) multWith (PTR(Val) input);
    virtual bool equals(PTR(Val) input) ;
    string to_string();
    bool is_true();
    PTR(Val) call(PTR(Val) actual_arg);
};

class BoolVal : public Val {
public:
    bool bool_;

    BoolVal(bool input);
    bool equals(PTR(Val) val);
    bool is_true();

    PTR(Val) addTo(PTR(Val) other_val);
    PTR(Val) multWith(PTR(Val) other_val);
    PTR(expr) to_expr();
    string to_string();
    PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(expr) body;

    FunVal(std::string arg, PTR(expr) body);
    bool equals(PTR(Val) val);
    bool is_true();

    PTR(Val) addTo(PTR(Val) other_val);
    PTR(Val) multWith(PTR(Val) other_val);
    PTR(Val) call(PTR(Val) actual_arg);
    PTR(expr) to_expr();
    std::string to_string();
};







#endif //CS6015PROJECT_VAL_H
