//
// Created by 谷金 on 1/17/22.
//


#include "expr.hpp"
#include<iostream>
#include "catch.hpp"
#include<cstring>
#include<stdexcept>

using namespace std;
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
    return 0;
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
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs) );
}

int Add::interp() {
    Num *l = dynamic_cast<Num*>(this->lhs);
    Num *r = dynamic_cast<Num*>(this->rhs);
    if (l == NULL || r == NULL) return 0;
    else return ((l->val) + (r->val));
}

bool Add::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Add::subst(string s1, expr *e){
//    Variable *v = new Variable(s1);
    return new Add(e,this->rhs);
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
    Num *l = dynamic_cast<Num*>(this->lhs);
    Num *r = dynamic_cast<Num*>(this->rhs);
    if (l == NULL || r == NULL) return 0;
    else return ((l->val) * (r->val));
}

bool Mult::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Mult::subst(string s1, expr *e){

    return new Mult(e,this->rhs);
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
   return true;
}

expr* Variable::subst(string s1, expr *e){

    return 0;
}

/*
 * Test
 */
TEST_CASE("equals"){

    //test objects
    Num n1(3);
    Variable v1("Jin");

    expr *f1 = new Num(2);
    expr *f2 = new Num(3);
    expr *f3 = new Num(4);
    expr *f4 = new Num(5);

    expr *e0 = new Num(3);
    expr *e1 = new Num(4);
    expr *e2 = new Add(f1,e0);
    expr *e3 = new Add(f3,f4);
    expr *e4 = new Mult(f3,f4);
    expr *e5 = new Mult(f3,f3);
    expr *e6 = new Variable("Jin");
    expr *e7 = new Variable("Jia");

    Add a1(f1, f2);

    Mult m1(f3,f4);

    int res_Num = n1.interp();
    int res_Add = a1.interp();
    int res_Mult = m1.interp();

    //test expression Num
    CHECK(n1.equals(e0));
    CHECK(!n1.equals(e1));
    CHECK(!n1.equals(e2));
    CHECK(!n1.equals(e4));
    CHECK(!n1.equals(e7));

    //test expression Add
    CHECK(a1.equals(e2));
    CHECK(!a1.equals(e3));
    CHECK(!a1.equals(e1));
    CHECK(!a1.equals(e5));
    CHECK(!a1.equals(e7));

    //test expression Muti
    CHECK(m1.equals(e4));
    CHECK(!m1.equals(e5));
    CHECK(!m1.equals(e3));
    CHECK(!m1.equals(e0));
    CHECK(!m1.equals(e7));

    //test expression Variable
    CHECK(v1.equals(e6));
    CHECK(!v1.equals(e7));
    CHECK(!v1.equals(e1));
    CHECK(!v1.equals(e2));
    CHECK(!v1.equals(e3));
    CHECK(!v1.equals(e4));

    //test interp
    CHECK(res_Num==3);
    CHECK(res_Add==5);
    CHECK(res_Mult==20);
    CHECK_THROWS_WITH( v1.interp(), "Variable has no value" );

    //test has_variable
    CHECK((new Num(5))->has_variable()== false);
    CHECK((new Add((new Variable("s")),(new Num(1)))));
    CHECK((new Mult((new Variable("s")),(new Num(1)))));
    CHECK((new Add((new Num(3)),(new Num(1)))));
    CHECK((new Mult((new Num(3)),(new Num(1)))));
    CHECK(new Variable("s"));

    //test subst
    CHECK( (new Add(new Variable("x"), new Num(7)))
                   ->subst("x", new Variable("y"))
                   ->equals(new Add(new Variable("y"), new Num(7))) );

    CHECK( (new Mult(new Variable("x"), new Num(7)))
                   ->subst("x", new Variable("y"))
                   ->equals(new Mult(new Variable("y"), new Num(7))) );


}