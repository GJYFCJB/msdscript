//
// Created by 谷金 on 2/16/22.
//

#ifndef CS6015PROJECT_VAL_H
#define CS6015PROJECT_VAL_H

#include <string>

using namespace std;

class expr;

class Val {
public:
    virtual expr* to_expr() = 0;
    virtual Val* multWith (Val* input) = 0;
    virtual Val* addTo (Val* input) = 0;
    virtual bool equals(Val* input) = 0;
    virtual string to_string() = 0;
    virtual bool is_true() = 0;
    virtual Val* call(Val* actual_arg) = 0;
};

class NumVal : public Val{
public:
    int val;
    NumVal(int n);
    virtual expr* to_expr();
    virtual Val* addTo (Val* input);
    virtual Val* multWith (Val* input);
    virtual bool equals(Val* input) ;
    string to_string();
    bool is_true();
    Val* call(Val* actual_arg);
};

class BoolVal : public Val {
public:
    bool bool_;

    BoolVal(bool input);
    bool equals(Val* val);
    bool is_true();

    Val* addTo(Val* other_val);
    Val* multWith(Val* other_val);
    expr* to_expr();
    string to_string();
    Val* call(Val* actual_arg);
};

class FunVal : public Val {
public:
    std::string formal_arg;
    expr* body;

    FunVal(std::string arg, expr* body);
    bool equals(Val* val);
    bool is_true();

    Val* addTo(Val* other_val);
    Val* multWith(Val* other_val);
    Val* call(Val* actual_arg);
    expr* to_expr();
    std::string to_string();
};







#endif //CS6015PROJECT_VAL_H
