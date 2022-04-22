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
        return new NumVal(val + other_num_val->val);
};

bool NumVal::equals(Val* input){
    NumVal *t = dynamic_cast<NumVal *>(input);
    if (t == NULL)
        return false;
    else
        return (this->val == t->val);
}

Val* NumVal::multWith (Val* input){
    NumVal* other_num_val = dynamic_cast<NumVal*>(input);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return new NumVal(val * other_num_val->val);
};

expr* NumVal::to_expr() {
    return new NumExpr(val);
};

NumVal::NumVal(int input) {
    this->val = input;
}

BoolVal::BoolVal(bool input){
    this->bool_ = input;
}

bool BoolVal::equals(Val* val){
    BoolVal* other_bool_val = dynamic_cast<BoolVal*>(val);
    if (other_bool_val == nullptr)
        return false;
    else
        return this->bool_ == other_bool_val->bool_;
}

bool BoolVal::is_true(){
    return this->bool_;
}

Val* BoolVal::addTo(Val* other_val){
    throw std::runtime_error("adding boolean makes no sense");
}

Val* BoolVal::multWith(Val* other_val){
    throw std::runtime_error("mult boolean makes no sense");
}

expr* BoolVal::to_expr(){
    return NULL;
}

string BoolVal::to_string() {
    if(bool_) return "true";
    else return "false";
}


