//
//  cmdline.cpp
//  msdscript
//
//  Created by 谷金 on 1/11/22.
//

#include <iostream>
#include "main.hpp"

using namespace std;


string s1 = "--help";
string s2 = "--test";

int use_arguments(int argc, char **argv)
{
    bool isTest = false;
    if(argc>1) {
        //first identify the length of command, if the length is 1 no command input
        for (int i = 1; i < argc; i++) {
            //help command input
            if (argv[i] == s1) {
                cout << "loading the help function, please wait...." << endl;

                exit(0);
            }
                //test command input
            else if (argv[i] == s2) {
                if (!isTest) {
                    Catch::Session().run(1, argv);
                    isTest = true;
                } else {
                    cerr << "Has tested before." << endl;
                    exit(1);
                }
            }
                //if test 2 times more or input other command not valid
            else {
                cerr << "wrong input, please type in other command\n";
                exit(1);
            }
        }

    }
    return 0;
}

int main(int argc,char **argv){
    use_arguments(argc, argv);
return 0;

}
