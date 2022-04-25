//
// Created by 谷金 on 4/24/22.
//

#ifndef CS6015PROJECT_ENV_H
#define CS6015PROJECT_ENV_H

#include "pointer.h"
#include <string>
using namespace std;
class Val;

class Env CLASS(Env){
public:
    virtual PTR(Val) lookup(string find_name) = 0;
};

class EmptyEnv : public Env {
public:
    EmptyEnv();
    PTR(Val) lookup(std::string find_name);
};

class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name);
};


#endif //CS6015PROJECT_ENV_H
