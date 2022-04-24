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

string NumVal::to_string(){
    return std::to_string(val);
}

bool NumVal::is_true(){
    throw std::runtime_error("numbers is_true makes no sense");
}

Val* NumVal::call(Val* actual_arg){
    throw std::runtime_error("cannot call on a number");
}

//boolval
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

Val* BoolVal::call(Val* actual_arg){
    throw std::runtime_error("cannot call on a boolean");
}

//FunVal
FunVal::FunVal(std::string arg, expr* body){
    this->formal_arg = arg;
    this->body = body;
}

bool FunVal::equals(Val* other_val){
    FunVal* other_fun_val = dynamic_cast<FunVal*>(other_val);
    if (other_fun_val == nullptr)
        return false;
    else
        return (formal_arg == other_fun_val->formal_arg && body->equals(other_fun_val->body));
}

bool FunVal::is_true(){
    throw std::runtime_error("functions' true/false makes no sense");
}

Val* FunVal::addTo(Val* other_val){
    throw std::runtime_error("no adding functions");
}

Val* FunVal::multWith(Val* other_val){
    throw std::runtime_error("no multiplying functions");
}

Val* FunVal::call(Val* actual_arg){
    return body->interp();
}

expr* FunVal::to_expr(){
    return new(FunExpr)(this->formal_arg, this->body);
}

std::string FunVal::to_string(){
    return "_fun (" + this->formal_arg + ") " + this->body->to_string();
}


