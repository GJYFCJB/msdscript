//
// Created by 谷金 on 1/17/22.
//

#ifndef expr_hpp
#define expr_hpp
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;
class expr{
public:
    virtual bool equals(expr *e) = 0;
};

class Num : public expr {

public:
    int val;

    Num(int val) ;

    bool equals(expr *e);
};

class Add : public expr {
public:
    expr *lhs;
    expr *rhs;

    Add(expr *lhs, expr *rhs);

    bool equals(expr *e);
};

class Mult : public expr {
public:
    expr *lhs;
    expr *rhs;
    Mult(expr* lhs, expr *rhs);

    bool equals(expr *e);
};

//class Car : public expr {
//public:
//    string brand;
//    string body_styles;
//
//    Car(string a, string b) {
//        this -> brand = a;
//        this -> body_styles = b;
//    }
//
//    bool equals(expr *a);
//};

//class Monitor : public expr {
//public:
//    int size;
//    int resolution;
//
//    Monitor(int a, int b) {
//        this -> size = a;
//        this -> resolution = b;
//    }
//
//    bool equals(expr *m);
//
//};
#endif //MSDSCRIPT_EXPR_HPP
