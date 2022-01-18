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
    int tar = 3;
    Num n1(3);

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


    Add a1(f1, f2);

    Mult m1(f3,f4);

    //test expression Num
    CHECK(n1.equals(e0));
    CHECK(!n1.equals(e1));

    //test expression Add
    CHECK(a1.equals(e2));
    CHECK(!a1.equals(e3));

    //test expression Muti
    CHECK(m1.equals(e4));
    CHECK(!m1.equals(e5));
    CHECK(!m1.equals(e3));

}