//
// Created by 谷金 on 4/24/22.
//

#include "env.h"

EmptyEnv::EmptyEnv() {

}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if(find_name == name)
        return val;
    else
        return rest->lookup(find_name);
}