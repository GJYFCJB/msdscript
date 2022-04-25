//
// Created by 谷金 on 1/17/22.
//


#include "expr.hpp"
#include "Val.h"
#include<iostream>
#include<stdexcept>
#include <sstream>
using namespace std;


string expr::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

void expr::pretty_print(std::ostream &out){
    this -> pretty_print_at(out, prec_none, 0, 0, 0);
}

string expr::to_pretty_string(){
    std::stringstream ss;
    this -> pretty_print(ss);
    return ss.str();
}

void expr::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){

}



//override pure virtual method of interface
//Class NumExpr--------------------------------------------------------------------------
NumExpr::NumExpr(int val_){
    this->rep = val_;
    this->val = NEW(NumVal)(val_);
}

bool NumExpr::equals(PTR(expr)e) {
    PTR(NumExpr)t = CAST(NumExpr)(e);
    if (t == NULL)
        return false;
    else
        return (this->rep == t->rep);
}

PTR(Val) NumExpr::interp(){
    return val;
}

bool NumExpr::has_variable(){
    return false;
}

PTR(expr) NumExpr::subst(string s1, PTR(Val)NEW_val){
    return NEW(NumExpr)(rep);
}

void NumExpr::print(ostream &out){
    out<<this->rep;
}

void NumExpr::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, false, 0,0);
}

void NumExpr:: pretty_print_at(ostream &out, precedence_t p, bool isLeftInside, bool isNested, int occupy){

    out<<this->rep;
}


//Class AddExpr--------------------------------------------------------------------------
AddExpr::AddExpr(PTR(expr)lhs, PTR(expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool AddExpr::equals(PTR(expr)e) {
    PTR(AddExpr)t = CAST(AddExpr)(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs));
}

PTR(Val) AddExpr::interp() {
    return lhs->interp()->addTo(rhs->interp());
}

bool AddExpr::has_variable(){
    return this->lhs->has_variable()||this->rhs->has_variable();
}

PTR(expr) AddExpr::subst(string s1, PTR(Val)NEW_val){
    return (NEW (AddExpr)((this -> lhs) -> subst(s1, NEW_val),
                        (this -> rhs) ->subst(s1,NEW_val)));
}

void AddExpr::print(ostream &out){
    out<<"(";
    lhs->print(out);
    out<<"+";
    rhs->print(out);
    out<<")";
}

void AddExpr::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, 0,0,0);
}

void AddExpr::pretty_print_at(ostream &out, precedence_t p, bool isLeftInside, bool isNested, int occupy){

    long begin = out.tellp();
    if(p<2||(p == 2&&!isLeftInside)){
        this->lhs->pretty_print_at(out,prec_add,true,true,occupy);
        out<<" + ";
        long end1 = out.tellp();
        this->rhs->pretty_print_at(out,prec_add, false,isNested,occupy+(end1-begin));
    }else{
        out << "(";
        long end2 = out.tellp();
        this->lhs->pretty_print_at(out,prec_add,true,true,occupy+(int)(end2-begin));
        out << " + ";
        long end3 = out.tellp();
        this->rhs->pretty_print_at(out,prec_add,false,isNested,occupy+(int)(end3-begin));
        out << ")";
    }
}

string AddExpr::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class MultExpr--------------------------------------------------------------------------
MultExpr::MultExpr(PTR(expr) lhs, PTR(expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool MultExpr::equals(PTR(expr)e) {
    PTR(MultExpr)t = CAST(MultExpr)(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs) );
}

PTR(Val) MultExpr::interp() {
    return (this->lhs->interp()->multWith(this->rhs->interp()));
}

bool MultExpr::has_variable(){
    return (this->lhs->has_variable()||this->rhs->has_variable());
}

PTR(expr) MultExpr::subst(string s1, PTR(Val)NEW_val){
    return (NEW(MultExpr)((this->lhs) ->subst(s1, NEW_val), (this -> rhs) ->subst(s1, NEW_val)));
}

void MultExpr::print(ostream &out){

    out<<"(";
    lhs->print(out);
    out<<"*";
    rhs->print(out);
    out<<")";

}

void MultExpr::pretty_print(ostream &out){
    pretty_print_at(out, prec_none, false,0,0);
}

void MultExpr::pretty_print_at(ostream &out, precedence_t p, bool isLeftInside, bool isNested, int occupy){
//opposite to dfs we use true/false and accumulator to identify l/r side of the mult
//from top to bottom -> get the right answer
    long begin = out.tellp();
    if(isLeftInside == true&&p==3){
        out<<"(";
        long end1 = out.tellp();
        lhs->pretty_print_at(out,prec_mult,true,1,occupy+(int)(end1-begin));
        out<<" * ";
        long end2 = out.tellp();
        rhs->pretty_print_at(out, prec_mult, false,isNested,occupy+(int)(end2-begin));
        out<<")";
    }else{
        lhs->pretty_print_at(out,prec_mult,true,1,occupy);
        out<<" * ";
        long end3 = out.tellp();
        rhs->pretty_print_at(out, prec_mult, false,isNested,occupy+(int)(end3-begin));
    }
}

string MultExpr::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class VarExpr--------------------------------------------------------------------------
VarExpr::VarExpr(string input) {
    this -> s = input;
}

bool VarExpr::equals(PTR(expr)e) {
    PTR(VarExpr)t = CAST(VarExpr)(e);
    if (t == NULL)
        return false;
    else
        return (this->s == t->s);
}

PTR(Val) VarExpr::interp(){
    throw std::runtime_error("VarExpr has no value");
}

bool VarExpr::has_variable(){
    return true;
}

PTR(expr) VarExpr::subst(string s1, PTR(Val)val){
    if(this->s == s1){
        return val->to_expr();
    }else {
        return NEW(VarExpr)(s);
    }
}

void VarExpr::print(ostream &out){
    out<<this->s;
}

void VarExpr::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, false,0,0);
}

void VarExpr::pretty_print_at(ostream &out, precedence_t p, bool isInside, bool isNested, int occupy){
    out << (this->s);
}

string VarExpr::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class _Let--------------------------------------------------------------------------

letExpr::letExpr(string variable, PTR(expr) rhs, PTR(expr) body){
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool letExpr::equals(PTR(expr)e){
    PTR(letExpr)t = CAST(letExpr)(e);
    if (t == NULL)
        return false;
    else
        return (this->variable == (t->variable)
        && this->rhs->equals(t->rhs)
        && this->body->equals(t->body));
}

PTR(Val) letExpr::interp(){
    PTR(Val)NEWRhs = rhs -> interp();
    return body
            -> subst ((variable), NEWRhs)
            -> interp();
}

bool letExpr::has_variable(){
    return this->rhs->has_variable()||this->body->has_variable();
}

PTR(expr) letExpr::subst(string s1, PTR(Val)NEW_val){
    if (s1 == this->variable){
        return NEW(letExpr)(this->variable,
                        this->rhs->subst(s1,NEW_val),
                        this->body);
    }
    return NEW(letExpr)(this->variable,
                           this->rhs->subst(s1, NEW_val),
                           this->body->subst(s1, NEW_val));
}

void letExpr::print(ostream &out){
    out << "(letExpr ";
    out << this->variable;
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}

void letExpr::pretty_print(ostream &out){
    pretty_print_at(out,prec_none,0,0,0);
}

string letExpr::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

void letExpr::pretty_print_at(ostream &out, precedence_t p, bool isLeftInside, bool isNested, int occupy){

    long begin_ = out.tellp();

    if ((p > 1 && isLeftInside) || isNested){
        out << "(letExpr ";
        out << this->variable;
        out << " = ";
        long end_p1 = out.tellp();
        this->rhs->pretty_print_at(out, p, false, false,
                                   occupy+(int)(end_p1-begin_));
        out << "\n";

        for(int i=0; i<occupy+1; i++){
            out << " ";
        }
        out << "_in ";

        this->body->pretty_print_at(out, p, false, false, occupy+5);
        out << ")";
    }else{
        out << "letExpr ";
        out << this->variable;
        out << " = ";
        long end_p2 = out.tellp();
        this->rhs->pretty_print_at(out, p, false, false,
                                   occupy+(int)(end_p2-begin_));
        out << "\n";
        for(int i=0; i<occupy; i++){
            out << " ";
        }
        out << "_in ";

        this->body->pretty_print_at(out, p, false, false, occupy+4);
    }
}

//boolean
BoolExpr::BoolExpr(bool var){
    this->var = var;
}

bool BoolExpr::equals(PTR(expr) expr){
    PTR(BoolExpr) other_bool_expr = CAST(BoolExpr)(expr);
    if (other_bool_expr == nullptr)
        return false;
    else
        return var == other_bool_expr->var;
}

bool BoolExpr::has_variable(){
    return false;
}

PTR(expr) BoolExpr::subst(std::string input, PTR(Val) NEW_val){
    PTR(expr) res = NEW(BoolExpr)(this->var);
    return res;
}

std::string BoolExpr::to_string(){
    if (var == true)
        return "_true";
    else
        return "_false";
}

PTR(Val) BoolExpr::interp(){
    return NEW(BoolVal)(var);
}

void BoolExpr::print(ostream &out){

}

//EqualExpr
EqualExpr::EqualExpr(PTR(expr) lhs, PTR(expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(PTR(expr) other_expr){
    PTR(EqualExpr) t = CAST(EqualExpr)(other_expr);
    if (t == nullptr)
        return false;
    else
        return (lhs->equals(t->lhs)
                && rhs->equals(t->rhs));
}

bool EqualExpr::has_variable(){
    return (lhs->has_variable()
            || rhs->has_variable());
}

PTR(expr) EqualExpr::subst(string var, PTR(Val) NEW_val){
    return NEW(EqualExpr)( lhs->subst(var, NEW_val),
                          rhs->subst(var, NEW_val));
}

std::string EqualExpr::to_string(){
    return lhs->to_string() + " == " + rhs->to_string();
}

PTR(Val) EqualExpr::interp(){
    PTR(Val) _lhs = lhs->interp();
    PTR(Val) _rhs = rhs->interp();
    return NEW(BoolVal)(_lhs->equals(_rhs));
}

void EqualExpr::print(ostream &out){

}

//IfExpr
IfExpr::IfExpr(PTR(expr) test_part, PTR(expr) then_part, PTR(expr) else_part){
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(PTR(expr) other_expr){
    PTR(IfExpr) other_if_expr = CAST(IfExpr)(other_expr);
    if (other_if_expr == nullptr)
        return false;
    else
        return (test_part->equals(other_if_expr->test_part)
                && then_part->equals(other_if_expr->then_part)
                && else_part->equals(other_if_expr->else_part));
}

bool IfExpr::has_variable(){
    return (test_part->has_variable()
            || then_part->has_variable()
            || else_part->has_variable());
}

PTR(expr) IfExpr::subst(std::string var, PTR(Val) NEW_val){

    return NEW(IfExpr)(test_part->subst(var, NEW_val),
                       then_part->subst(var, NEW_val),
                       else_part->subst(var, NEW_val));
}

std::string IfExpr::to_string(){
    return "(_if " + test_part->to_string() +
           " _then " + then_part->to_string() +
           " _else " + else_part->to_string() + ")";
}

PTR(Val) IfExpr::interp(){
    if(test_part->interp()->is_true())
        return then_part->interp();
    else
        return else_part->interp();
}

void IfExpr::print(ostream &out){

}

//FunExpr
FunExpr::FunExpr(std::string arg, PTR(expr) body){
    this->formal_arg = arg;
    this->body = body;
}

bool FunExpr::equals(PTR(expr) other_expr){
    PTR(FunExpr) other_fun_expr = CAST(FunExpr)(other_expr);
    if (other_fun_expr == nullptr)
        return false;
    else
        return (formal_arg == other_fun_expr->formal_arg
                && body->equals(other_fun_expr->body));
}

bool FunExpr:: has_variable(){
    return true;
}

PTR(Val) FunExpr::interp(){
    return NEW(FunVal)(formal_arg, body);
}

PTR(expr) FunExpr::subst(std::string var, PTR(Val) val){
    if(var == formal_arg){
        return NEW(FunExpr)(formal_arg, body);
    }
    return NEW(FunExpr)(formal_arg, body->subst(var, val));
}

std::string FunExpr::to_string(){
    return "(_fun (" + formal_arg + ") " + body -> to_string() + ")";
}

void FunExpr::print(ostream &out){

}

//CallExpr
CallExpr::CallExpr(PTR(expr) to_be, PTR(expr) actual){
    this->to_be_called = to_be;
    this->actual_arg = actual;
}

bool CallExpr::equals(PTR(expr) other_expr){
    PTR(CallExpr) other_call_expr = CAST(CallExpr)(other_expr);
    if (other_call_expr == nullptr)
        return false;
    else
        return(to_be_called->equals(other_call_expr->to_be_called)
               && actual_arg->equals(other_call_expr->actual_arg));
}

bool CallExpr::has_variable(){
    return true;
}

PTR(Val) CallExpr::interp(){
    return to_be_called->interp()->call(actual_arg->interp());
}

PTR(expr) CallExpr::subst(std::string var, PTR(Val) val){
    return NEW(CallExpr)(to_be_called->subst(var, val), actual_arg->subst(var, val));
}

std::string CallExpr::to_string(){
    return ", (" + to_be_called->to_string() + "(" + actual_arg->to_string() + "))";
}

void CallExpr::print(ostream &out){

}












