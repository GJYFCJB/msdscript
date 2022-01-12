//
//  cmdline.cpp
//  msdscript
//
//  Created by 谷金 on 1/11/22.
//

#include "cmdline.hpp"
#include <iostream>
using namespace std;

string s1 = "--help";
string s2 = "--test";

int use_arguments(int argc, char **argv)
{
    //first identify the length of command, if the length is 1 no command input
    if(argc>1){
    //help command input
    if(argv[1] == s1&&argc == 2){
        cout<<"loading the help function, please wait...."<<endl;
        exit(0);
    }
    //test command input
    else if(argv[1] == s2 && argc==2){
            cout<<"Tests passed"<<endl;
            exit(0);
    }
    //if test 2 times more or input other command not valid
    else
    {
        cerr << "wrong input, please type in another command\n";
        exit(1);
    }
    }
    else cerr<<"please type in your cmmand"<<endl;
    return 0;
}

int main(int argc,char **argv){
    use_arguments(argc, argv);
    }
    
