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
    this ->print(ss);
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

//
//bool expr::equals(expr *e){
//    return false;
//}

//override pure virtual method of interface
//Class NumExpr--------------------------------------------------------------------------
NumExpr::NumExpr(int val_){
    this->rep = val_;
    Val* val = new NumVal(val_);
}

bool NumExpr::equals(expr *e) {
    NumExpr *t = dynamic_cast<NumExpr*>(e);
    if (t == NULL)
        return false;
    else
        return (this->rep == t->rep);
}

Val * NumExpr::interp(){
    return val;
}

bool NumExpr::has_variable(){
    return false;
}

expr* NumExpr::subst(string s1, Val *new_val){
    return new NumExpr(rep);
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
AddExpr::AddExpr(expr *lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool AddExpr::equals(expr *e) {
    AddExpr *t = dynamic_cast<AddExpr*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs));
}

Val* AddExpr::interp() {

        return lhs->interp()->addTo(rhs->interp());
}

bool AddExpr::has_variable(){
    return this->lhs->has_variable()||this->rhs->has_variable();
}

expr* AddExpr::subst(string s1, Val *new_val){
    return (new AddExpr((this -> lhs) -> subst(s1, new_val),
                        (this -> rhs) ->subst(s1,new_val)));
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
MultExpr::MultExpr(expr* lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool MultExpr::equals(expr *e) {
    MultExpr *t = dynamic_cast<MultExpr*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs) );
}

Val* MultExpr::interp() {

        return (this->rhs->interp()->multWith(this->lhs->interp()));

}

bool MultExpr::has_variable(){
    return (this->lhs->has_variable()||this->rhs->has_variable());
}

expr* MultExpr::subst(string s1, Val *new_val){
    return (new MultExpr((this->lhs) ->subst(s1, new_val), (this -> rhs) ->subst(s1, new_val)));
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

bool VarExpr::equals(expr *e) {
    VarExpr *t = dynamic_cast<VarExpr*>(e);
    if (t == NULL)
        return false;
    else
        return (this->s == t->s);
}

Val* VarExpr::interp(){
    throw std::runtime_error("VarExpr has no value");
}


bool VarExpr::has_variable(){
    return true;
}

expr* VarExpr::subst(string s1, Val *val){
    if(this->s == s1){
        return val->to_expr();
    }else {
        return this;
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

letExpr::letExpr(VarExpr* variable, expr* rhs, expr* body){
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool letExpr::equals(expr *e){
    letExpr *t = dynamic_cast<letExpr*>(e);
    if (t == NULL)
        return false;
    else
        return (this->variable->equals(t->variable)
        && this->rhs->equals(t->rhs)
        && this->body->equals(t->body));
}

Val* letExpr::interp(){
    Val *newRhs = rhs -> interp();
    return (this->body)
            -> subst ((this->variable)->to_string(), newRhs)
            -> interp();
}

bool letExpr::has_variable(){
    return this->rhs->has_variable()||this->body->has_variable();
}

expr* letExpr::subst(string s1, Val *new_val){

    return new letExpr(this->variable,
                           this->rhs->subst(s1, new_val),
                           this->body->subst(s1, new_val));
}

void letExpr::print(ostream &out){
    out << "(letExpr ";
    this->variable->print(out);
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
        this->variable->pretty_print_at(out, p, false, false, occupy+1);
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
        this->variable->pretty_print_at(out, p, false, false, occupy);
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

bool BoolExpr::equals(expr* expr){
    BoolExpr* other_bool_expr = dynamic_cast<BoolExpr*>(expr);
    if (other_bool_expr == nullptr)
        return false;
    else
        return var == other_bool_expr->var;
}

bool BoolExpr::has_variable(){
    return false;
}

expr* BoolExpr::subst(std::string input, Val* new_val){
    BoolExpr* res = new BoolExpr(this->var);
    return res;
}

std::string BoolExpr::to_string(){
    if (var == true)
        return "_true";
    else
        return "_false";
}

Val* BoolExpr::interp(){
    return NULL;
}

void BoolExpr::print(ostream &out){

}

//EqualExpr
EqualExpr::EqualExpr(expr* lhs, expr* rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(expr* other_expr){
    EqualExpr* t = dynamic_cast<EqualExpr*>(other_expr);
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

expr* EqualExpr::subst(string var, Val* new_val){
    return new EqualExpr( lhs->subst(var, new_val),
                          rhs->subst(var, new_val));
}

std::string EqualExpr::to_string(){
    return lhs->to_string() + " == " + rhs->to_string();
}

Val* EqualExpr::interp(){
    return NULL;
}

void EqualExpr::print(ostream &out){

}

//IfExpr
IfExpr::IfExpr(expr* test_part, expr* then_part, expr* else_part){
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(expr* other_expr){
    IfExpr* other_if_expr = dynamic_cast<IfExpr*>(other_expr);
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

expr* IfExpr::subst(std::string var, Val* new_val){

    return new IfExpr(test_part->subst(var, new_val),
                       then_part->subst(var, new_val),
                       else_part->subst(var, new_val));
}

std::string IfExpr::to_string(){
    return "(_if " + test_part->to_string() +
           " _then " + then_part->to_string() +
           " _else " + else_part->to_string() + ")";
}

Val* IfExpr::interp(){
    return NULL;
}

void IfExpr::print(ostream &out){

}













