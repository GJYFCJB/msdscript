//
// Created by 谷金 on 1/17/22.
//


#include "expr.hpp"

#include<iostream>
#include<stdexcept>
#include <sstream>
using namespace std;


string expr::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

//string expr::to_string_(){
//    std::istream ss;
//    this -> print(ss);
//    return ss.str();
//}


void expr::pretty_print(std::ostream &out){
//    this -> pretty_print_at(os, false, false, false);
    this -> pretty_print_at(out, prec_none, 0, 0, 0);
}



string expr::to_pretty_string(){
    std::stringstream ss;
    this -> pretty_print(ss);
    return ss.str();
}



//override pure virtual method of interface
//Class Num--------------------------------------------------------------------------
Num::Num(int val){
    this->val = val;
}

bool Num::equals(expr *e) {
    Num *t = dynamic_cast<Num*>(e);
    if (t == NULL)
        return false;
    else
        return (this->val == t->val);
}

int Num::interp(){
    return this->val;
}

bool Num::has_variable(){
    return false;
}

expr* Num::subst(string s1, expr *e){
    return this;
}

void Num::print(ostream &out){
    out<<this->val;
}

void Num::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, false, 0,0);
}

void Num:: pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){
//    stringstream ss;
//    ss << this->val;
//    string str = ss.str();
    out<<this->val;
}


//Class Add--------------------------------------------------------------------------
Add::Add(expr *lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool Add::equals(expr *e) {
    Add *t = dynamic_cast<Add*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs));
}

int Add::interp() {
//Those methods should not use `dynamic_cast` (which should only be used in `equals`)
    // You should recur to `interp` for `lhs` and `rhs`, instead.
//    Num *l = dynamic_cast<Num*>(this->lhs);
//    Num *r = dynamic_cast<Num*>(this->rhs);
        return (this->rhs->interp() + this->lhs->interp());
//    else(throw std::runtime_error("Variable has no value"));
}

bool Add::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Add::subst(string s1, expr *e){
    return (new Add((this -> lhs) -> subst(s1,e),
                    (this -> rhs) ->subst(s1,e)));
}

void Add::print(ostream &out){
//    stringstream o("");
//    lhs->print(o);
//    rhs->print(o);
//    out<<"("+lhs->print(o)+ "+" +rhs->print(o)+")";
    out<<"(";
    lhs->print(out);
    out<<"+";
    rhs->print(out);
    out<<")";
//    return "("+lhs->print(o)+ "+" +rhs->print(o)+")";
}

void Add::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, 0,0,0);
}

void Add::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){

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

string Add::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class Mult--------------------------------------------------------------------------
Mult::Mult(expr* lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool Mult::equals(expr *e) {
    Mult *t = dynamic_cast<Mult*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs) );
}

int Mult::interp() {
//    Num *l = dynamic_cast<Num*>(this->lhs);
//    Num *r = dynamic_cast<Num*>(this->rhs);
//    if(this->rhs!=NULL && this->lhs!=NULL && this->interp_())
        return (this->rhs->interp() * this->lhs->interp());
//    else(throw std::runtime_error("Variable has no value"));
}

bool Mult::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Mult::subst(string s1, expr *e){
    return (new Mult((this->lhs) ->subst(s1,e),(this -> rhs) ->subst(s1,e)));
}

void Mult::print(ostream &out){
//    stringstream o("");
//    lhs->print(o);
//    rhs ->print(o);
    out<<"(";
    lhs->print(out);
    out<<"*";
    rhs->print(out);
    out<<")";
//    out<<"("+lhs->print(o)+ "*" +rhs->print(o)+")";
//    return "("+lhs->print(o)+ "*" +rhs->print(o)+")";
}

void Mult::pretty_print(ostream &out){
    pretty_print_at(out, prec_none, false,0,0);
}

void Mult::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){
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

string Mult::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class Variable--------------------------------------------------------------------------
Variable::Variable(string input) {
    this -> s = input;
}

bool Variable::equals(expr *e) {
    Variable *t = dynamic_cast<Variable*>(e);
    if (t == NULL)
        return false;
    else
        return (this->s == t->s);
}

int Variable::interp(){
    throw std::runtime_error("Variable has no value");
}

bool Variable::has_variable(){
    if(this->s != "") return true;
    return false;
}

expr* Variable::subst(string s1, expr *e){
    if(this->s == s1){
        return e;
    }else {
        return this;
    }
}

void Variable::print(ostream &out){
    out<<this->s;
}

void Variable::pretty_print(ostream &out){

    pretty_print_at(out,prec_none, false,0,0);
}

void Variable::pretty_print_at(ostream &out,precedence_t p,bool isInside,bool isNested,int occupy){
    out << (this->s);
}

string Variable::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class _Let--------------------------------------------------------------------------

_let::_let(Variable* variable, expr* rhs, expr* body){
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool _let::equals(expr *e){
    _let *t = dynamic_cast<_let*>(e);
    if (t == NULL)
        return false;
    else
        return (this->variable->equals(t->variable)
        && this->rhs->equals(t->rhs)
        && this->body->equals(t->body));
}

int _let::interp(){
    if(rhs) {
        return ((this->body)->subst(((this->variable)->to_string()),
                                    (this->rhs)))->interp();
    }
    else{
        expr *newRhs = new Num(rhs -> interp());
        return ((this->body)
                -> subst ((this->variable)->to_string(), newRhs))
                -> interp();
    }
}

bool _let::has_variable(){
    return this->rhs->has_variable()||this->body->has_variable();
}

expr* _let::subst(string s1, expr *e){

    if (s1 == this->variable->to_string()){
        return new _let(this->variable,
                        this->rhs->subst(s1,e),
                        this->body);
    }

    else{
        return new _let(this->variable,
                        this->rhs->subst(s1, e),
                        this->body->subst(s1, e));
    }
}
void _let::print(ostream &out){
    out << "(_let ";
    this->variable->print(out);
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}
void _let::pretty_print(ostream &out){
    pretty_print_at(out,prec_none,0,0,0);
}

string _let::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

void _let::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){

    long begin_ = out.tellp();

    if ((p > 1 && isLeftInside) || isNested){
        out << "(_let ";
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
        out << "_let ";
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


