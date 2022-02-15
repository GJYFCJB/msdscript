//
// Created by 谷金 on 2/15/22.
//

#include "exec.h"
#include <stdlib.h>

std::string random_expr_string();

int main(int argc, char **argv){
    const char * const interp_argv[] = { "msdscript", "--interp" };
    const char * const print_argv[] = { "msdscript", "--print" };
    for (int i = 0; i < 100; i++) {
        std::string in = random_expr_string();
        std::cout << "Trying " << in << "\n";
        ExecResult interp_result = exec_program(2, interp_argv, in);
        ExecResult print_result = exec_program(2, print_argv, in);
        ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
        if (interp_again_result.out != interp_result.out)
            throw std::runtime_error("different result for printed");
    }
    return 0;
}

std::string random_expr_string() {
    if ((rand() % 10) < 6)
        return std::to_string(rand());
    else return random_expr_string() + "+" + random_expr_string();
}
