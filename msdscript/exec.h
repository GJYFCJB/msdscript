//
// Created by 谷金 on 2/15/22.
//

#ifndef CS6015PROJECT_EXEC_H
#define CS6015PROJECT_EXEC_H


#include <string>
#include <iostream>
class ExecResult {
public:
    int exit_code;
    std::string out;
    std::string err;
    ExecResult() {
        exit_code = 0;
        out = "";
        err = "";
    }
};

extern ExecResult exec_program(int argc, const char * const *argv, std::string input);
#endif //CS6015PROJECT_EXEC_H
