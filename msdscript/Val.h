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
};

class NumVal : public Val{
public:
    int val;
    NumVal(int n);
    virtual expr* to_expr();
    virtual Val* addTo (Val* input);
    virtual Val* multWith (Val* input);
    virtual bool equals(Val* input) ;

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
};







#endif //CS6015PROJECT_VAL_H
