//
// Created by 谷金 on 2/16/22.
//

#ifndef CS6015PROJECT_VAL_H
#define CS6015PROJECT_VAL_H

class expr;
class Val {
public:
    virtual expr* to_expr() = 0;
    virtual Val* multWith (Val* input) = 0;
    virtual  Val* addTo (Val* input) = 0;
    virtual bool equals(Val* input) = 0;
};

class NumVal : public Val{
public:
    int n;
    NumVal(int n);
    virtual expr* to_expr();
    virtual Val* addTo (Val* input);
    virtual Val* multWith (Val* input);
    virtual bool equals(Val* input) ;

};





#endif //CS6015PROJECT_VAL_H
