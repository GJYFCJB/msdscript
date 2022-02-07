//
//  cmdline.cpp
//  msdscript
//
//  Created by 谷金 on 1/11/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "main.hpp"
#include "expr.hpp"
#include "parse.cpp"
using namespace std;

string s1 = "--help";
string s2 = "--test";
int use_arguments(int argc, char **argv){
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
    while (1) {
        expr *e = parse_expr(std::cin);
        e->pretty_print(std::cout);
        std::cout << "\n";
        skip_whitespace(std::cin);
        if (std::cin.eof())
            break; }
    return 0;

    return 0;
}

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
TEST_CASE("interp"){
    Num n1(3);
    Variable v1("Jin");
    expr *f1 = new Num(2);
    expr *f2 = new Num(3);
    expr *f3 = new Num(4);
    expr *f4 = new Num(5);
    Add a1(f1, f2);
    Mult m1(f3,f4);
    int res_Num = n1.interp();
    int res_Add = a1.interp();
    int res_Mult = m1.interp();
    expr *f5 = new Add((new Variable("s")),
                       (new Num(1)));
    expr *f6 = new Mult((new Variable("s")),
                        (new Num(1)));
//test interp
    CHECK(res_Num==3);
    CHECK(res_Add==5);
    CHECK(res_Mult==20);
    CHECK_THROWS_WITH(f5->interp(),"Variable has no value" );
    CHECK_THROWS_WITH( v1.interp(), "Variable has no value" );
    CHECK_THROWS_WITH(f6->interp(),"Variable has no value" );
}


TEST_CASE("has_variable"){
//test has_variable
    CHECK((new Num(5))->has_variable()== false);
    CHECK((new Add((new Variable("s")),(new Num(1)))));
    CHECK((new Mult((new Variable("s")),(new Num(1)))));
    CHECK((new Add((new Num(3)),(new Num(1)))));
    CHECK((new Mult((new Num(3)),(new Num(1)))));
    CHECK(new Variable("s"));
}
TEST_CASE("subst"){
//test subst

    expr *add = new Add(new Add(new Variable("xyz"), new Num(10)), new Num(11));
    expr *resAdd = new Add(new Add(new Num(20), new Num(10)), new Num(11));
    expr *mut = new Mult(new Add(new Variable("xyz"), new Num(10)), new Num(11));
    expr *resMut = new Mult(new Add(new Num(20), new Num(10)), new Num(11));
    CHECK( (new Add(new Variable("x"), new Num(7)))
                   ->subst("x", new Variable("y"))
                   ->equals(new Add(new Variable("y"), new Num(7))) );

    CHECK(add->subst("xyz", new Num(20))->equals(resAdd));
    CHECK(mut->subst("xyz", new Num(20))->equals(resMut));

    CHECK( (new Mult(new Variable("x"), new Num(7)))
                   ->subst("x", new Variable("y"))
                   ->equals(new Mult(new Variable("y"), new Num(7))) );


}

TEST_CASE("print && pretty_print"){

//-----test print
    stringstream out("");
    expr *f1 = new Num(2);
    f1->print(out);
    CHECK( out.str() == "2");

    stringstream out1("");
    expr *f2 = new Add(new Num(1), new Add(new Num(2), new Num(3)));
    f2->print(out1);
    CHECK(out1.str() == "(1+(2+3))");

    stringstream out2("");
    expr *f3 = new Add(new Num(1), new Mult(new Num(2), new Num(3)));
    f3->print(out2);
    CHECK(out2.str() == "(1+(2*3))");

//---test pretty_print
    Num* num1 = new Num(1);
    Num* num2 = new Num(2);
    Num* num3 = new Num(3);
    Mult* m0 = new Mult(num1,num2);
    Mult* m2 = new Mult(m0,num3);
    Mult* m4 = new Mult(num3,m0);
    Add* add = new Add(num1,m0);
    Add* add2 = new Add(m0,m2);
    Add* add1 = new Add(num2,num3);
    Mult* m3 = new Mult(num3,add1);
    Variable *v = new Variable("s");

    CHECK(m2->to_string() == "(1 * 2) * 3");
    CHECK(m4->to_string() == "3 * 1 * 2");
    CHECK(add->to_string() == "1 + 1 * 2");
//    CHECK(add2->to_string() =="(1 * 2) + (1 * 2) * 3");
    CHECK(m3->to_string() == "3 * (2 + 3)");
    CHECK(v->to_string()=="s" );

}

TEST_CASE("_let Subst"){
// test1 =
//          _let x = 4;
//          _in 7;          -> subst ("x", 1)
    expr *test1 = new _let(new Variable("x"), new Num(4), new Num(7));
    CHECK((test1 -> subst("x", new Num(1))) -> equals(test1));
// test2 =
//          _let x = x + 2;
//          _in x + 2;          -> subst ("x", 5)
    expr *test2 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(2)),
                           new Add(new Variable("x"), new Num(2)));
    CHECK(test2 -> subst("x", new Num(5)) -> equals(
            new _let(new Variable("x"),
                     new Add(new Num(5), new Num(2)),
                     new Add(new Variable("x"), new Num(2)))));
// test3 =
//          _let x = x + 2;
//          _in x + 2;          -> subst ("y", 5)
    expr *test3 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(2)),
                           new Add(new Variable("x"), new Num(2)));
    CHECK(test3 -> subst("y", new Num(5)) -> equals(test3));
}

TEST_CASE("_let Equals"){
// _let x = 6;
// _in x + 3;
    _let *test = new _let(new Variable("x"), new Num(6), new Add(new Variable("x"), new Num(3)));
    CHECK(test-> equals(new _let(new Variable("x"), new Num(6), new Add(new Variable("x"), new Num(3)))) == true);
    CHECK(test-> equals(new _let(new Variable("y"), new Num(6), new Add(new Variable("y"), new Num(3)))) == false);
    CHECK(test-> equals(new Num(6)) == false);
}

TEST_CASE("_let Has Variable"){
// test1 =
//          _let x = 2;
//          _in 4;
    expr *test1 = new _let(new Variable("x"), new Num(2), new Num(4));
    CHECK(test1 -> has_variable() == false);
// test2 =
//          _let x = x + 4;
//          _in 7;
    expr *test2 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(4)),
                           new Num(7));
    CHECK(test2 -> has_variable() == true);
// test3 =
//          _let x = 2;
//          _in x + 7;
    expr *test3 = new _let(new Variable("x"),
                           new Num(2),
                           new Add(new Variable("x"), new Num(7)));
    CHECK(test3 -> has_variable() == true);
// test4 =
//          _let x = x + 6;
//          _in x + 5;
    expr *test4 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(6)),
                           new Add(new Variable("x"), new Num(5)));
    CHECK(test4 -> has_variable() == true);
}

TEST_CASE("_let Print"){
// test1 =
//        _let x = 12;
//        _in x + 1;
    expr *test1 = new _let(new Variable("x"),
                           new Num(12),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test1 -> to_string()) == "(_let x=12 _in (x+1))");
// test2 =
//        _let x = 12;
//        _in _let x = 4;
//            _in x + 2;
    expr *test2 = new _let(new Variable("x"),
                           new Num(12),
                           new _let(new Variable("x"),
                                    new Num(4),
                                    new Add(new Variable("x"), new Num(2))));
    CHECK((test2 -> to_string()) == "(_let x=12 _in (_let x=4 _in (x+2)))");
// test3 =
//        _let x = y + 2;
//        _in y + 3;
    expr *test3 = new _let(new Variable("x"),
                           new Add(new Variable("y"), new Num(2)),
                           new Add(new Variable("y"), new Num(3)));
    CHECK((test3 -> to_string()) == "(_let x=(y+2) _in (y+3))");
}

TEST_CASE("_let Interp"){
// test1 =
//        _let x = 4;
//        _in 7;
    expr *test1 = new _let(new Variable("x"), new Num(4), new Num(7));
    CHECK( test1 -> interp() == 7);
// test2 =
//        _let x = 2;
//        _in x + 1;
    expr *test2 = new _let(new Variable("x"),
                           new Num(2),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test2 -> interp()) == 3);
// test3 =
//        _let x = 7;
//        _in y + 2;
    expr *test3 = new _let(new Variable("x"),
                           new Num(7),
                           new Add(new Variable("y"), new Num(2)));
    CHECK_THROWS_WITH( test3 -> interp(),
                       "Variable has no value" );
// test4 =
//        _let x = 4;
//        _in _let x = 7;
//            _in x + 1;
    expr *test4 = new _let(new Variable("x"),
                           new Num(4),
                           new _let(new Variable("x"),
                                    new Num(7),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK(test4 -> interp() == 8);

// test6 =
//        _let x = 9;
//        _in _let x = x + 2;
//            _in x + 2;
    expr *test6 = new _let(new Variable("x"), new Num(9),
                           new _let(new Variable("x"),
                                    new Add(new Variable("x"), new Num(2)),
                                    new Add(new Variable("x"), new Num(2))));
    CHECK((test6 -> interp()) == 13);

}

TEST_CASE("_let Pretty Print"){
// test1 =
//        _let x = 4
//        _in x + 1
    expr *test1 = new _let(new Variable("x"),
                           new Num(4),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test1 -> to_pretty_string()) == "_let x = 4\n_in x + 1");
// test2 =
//        (_let x = 7
//        _in x) + 9
    expr *test2 = new Add(new _let(new Variable("x"), new Num(7), new Variable("x")),
                          new Num(9));
    CHECK((test2 -> to_pretty_string()) == "(_let x = 7\n _in x) + 9");
// test3 =
//        7 * (_let x = 9
//             _in  x) + 3
    expr *test3 = new Add (new Mult(new Num(7),
                                    new _let(new Variable("x"), new Num(9), new Variable("x"))),
                           new Num(3));
    CHECK((test3 -> to_pretty_string()) == "7 * (_let x = 9\n     _in x) + 3");
// test4 =
//        6 * _let x = 9
//            _in  x + 1
    expr *test4 = new Mult(new Num(6),
                           new _let(new Variable("x"),
                                    new Num(9),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK((test4 -> to_pretty_string()) == "6 * _let x = 9\n    _in (x + 1)");
// test5 =
//        _let x = 7
//        _in _let x = 2
//            _in x + 2
    expr *test5 = new _let(new Variable("x"),
                           new Num(7),
                           new _let(new Variable("x"), new Num(2),
                                    new Add(new Variable("x"), new Num(2))));
    CHECK((test5 -> to_pretty_string()) == "_let x = 7\n_in _let x = 2\n    _in x + 2");
// test6 =
//        _let x = _let x = 7
//                 _in x + 2
//        _in x + 2
    expr *test6 = new _let(new Variable("x"),
                           new _let(new Variable("x"), new Num(7),
                                    new Add(new Variable("x"), new Num(2))),
                           new Add(new Variable("x"), new Num(2)));
    CHECK((test6 -> to_pretty_string()) == "_let x = _let x = 7\n         _in x + 2\n_in x + 2");
// test7 =
//       (7 + (_let x = _let x = 2
//                      _in x + 3
//             _in x + 3)) * 6
    expr *test7 = new Mult(new Add(new Num(7),
                                   new _let(new Variable("x"),
                                            new _let(new Variable("x"),
                                                     new Num(2),
                                                     new Add(new Variable("x"), new Num(3))),
                                            new Add(new Variable("x"), new Num(3)))),
                           new Num(6));
    CHECK((test7 -> to_pretty_string()) == "(7 + (_let x = _let x = 2\n               _in x + 3\n      _in x + 3)) * 6");
}