//
// Created by 谷金 on 2/16/22.
//

#include "Val.h"
#include "expr.hpp"
#include "env.h"


PTR(Val) NumVal::addTo (PTR(Val) input){

    PTR(NumVal) other_num_val = CAST(NumVal)(input);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return NEW(NumVal)(val + other_num_val->val);
};

bool NumVal::equals(PTR(Val) input){
    PTR(NumVal)t = CAST(NumVal)(input);
    if (t == NULL)
        return NEW(BoolVal)(false)->bool_;
    else
        return NEW(BoolVal)((this->val == t->val)) -> bool_;
}

PTR(Val) NumVal::multWith (PTR(Val) input){
    PTR(NumVal) other_num_val = CAST(NumVal)(input);
    if (other_num_val == nullptr)
        throw std::runtime_error("not a number");
    else
        return NEW(NumVal)( this->val * other_num_val->val);
};

PTR(expr) NumVal::to_expr() {
    return NEW(NumExpr)(val);
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

PTR(Val) NumVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("cannot call on a number");
}

//boolval
BoolVal::BoolVal(bool input){
    this->bool_ = input;
}

bool BoolVal::equals(PTR(Val) val){
    PTR(BoolVal) other_bool_val = CAST(BoolVal)(val);
    if (other_bool_val == nullptr)
        return false;
    else
        return this->bool_ == other_bool_val->bool_;
}

bool BoolVal::is_true(){
    return this->bool_;
}

PTR(Val) BoolVal::addTo(PTR(Val) other_val){
    throw std::runtime_error("adding boolean makes no sense");
}

PTR(Val) BoolVal::multWith(PTR(Val) other_val){
    throw std::runtime_error("mult boolean makes no sense");
}

PTR(expr) BoolVal::to_expr(){
    return NULL;
}

string BoolVal::to_string() {
    if(bool_) return "_true";
    else return "_false";
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("cannot call on a boolean");
}

//FunVal
FunVal::FunVal(std::string arg, PTR(expr) body, PTR(Env) env){
    this->formal_arg = arg;
    this->body = body;
    this->env = env;
}

bool FunVal::equals(PTR(Val) other_val){
    PTR(FunVal) other_fun_val = CAST(FunVal)(other_val);
    if (other_fun_val == nullptr)
        return false;
    else
        return (formal_arg == other_fun_val->formal_arg && body->equals(other_fun_val->body));
}

bool FunVal::is_true(){
    throw std::runtime_error("functions' true/false makes no sense");
}

PTR(Val) FunVal::addTo(PTR(Val) other_val){
    throw std::runtime_error("no adding functions");
}

PTR(Val) FunVal::multWith(PTR(Val) other_val){
    throw std::runtime_error("no multiplying functions");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg){
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
}

PTR(expr) FunVal::to_expr(){
    return NEW(FunExpr)(this->formal_arg, this->body);
}

std::string FunVal::to_string(){
    return "_fun (" + this->formal_arg + ") " + this->body->to_string();
}


