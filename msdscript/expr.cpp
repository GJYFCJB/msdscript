//
// Created by 谷金 on 1/17/22.
//

#include "expr.hpp"
#include<iostream>
#include "catch.hpp"
#include<cstring>

using namespace std;

Num::Num(int val){
    this->val = val;
}

//override pure virtual method of interface
bool Num::equals(expr *e) {
    Num *t = dynamic_cast<Num*>(e);
    if (t == NULL)
        return false;
    else
        return (this->val == t->val);
}

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
        return ((this->lhs == t->lhs && this->rhs == t -> rhs)||(
                this->lhs == t->rhs && this->rhs == t -> lhs
                ));
}

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
        return ((this->lhs == t->lhs && this->rhs == t->rhs)||(
                this->lhs == t->rhs && this->rhs == t->lhs
        ));
}

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

//bool Car::equals(Car *a) {
//
//    Car *t = dynamic_cast<Car*>(a);
//    if(t==NULL)
//        return false;
//    else
//        return(this->brand == a->brand && this->body_styles == a-> body_styles);
//}

//bool Monitor::equals(Monitor *m) {
//
//    Monitor *t = dynamic_cast<Monitor*>(m);
//    if(t==NULL)
//        return false;
//    else
//        return(this->size == m->size && this->resolution == m->resolution);
//}

TEST_CASE("equals"){

    Num n1(3);
    Variable v1("Jin");


    expr *f1 = new Num(2);
    expr *f2 = new Num(3);
    expr *f3 = new Num(4);
    expr *f4 = new Num(5);

    expr *e0 = new Num(3);
    expr *e1 = new Num(4);
    expr *e2 = new Add(f2,f1);
    expr *e3 = new Add(f3,f4);
    expr *e4 = new Mult(f3,f4);
    expr *e5 = new Mult(f3,f3);
    expr *e6 = new Variable("Jin");
    expr *e7 = new Variable("Jia");


    Add a1(f1, f2);

    Mult m1(f3,f4);

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



}