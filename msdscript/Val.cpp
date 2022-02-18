//
// Created by 谷金 on 2/16/22.
//

#include "Val.h"
#include "expr.hpp"


Val* NumVal::addTo (Val* input){

    NumVal* other_num_val = dynamic_cast<NumVal*>(input);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return new NumVal(n + other_num_val->n);
};

bool NumVal::equals(Val* input){
    NumVal *t = dynamic_cast<NumVal *>(input);
    if (t == NULL)
        return false;
    else
        return (this->n == t->n);
}

Val* NumVal::multWith (Val* input){

    NumVal* other_num_val = dynamic_cast<NumVal*>(input);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return new NumVal(n * other_num_val->n);
};

expr* NumVal::to_expr() {
    return new Num(n);
};

NumVal::NumVal(int input) {
    this->n = input;
}


