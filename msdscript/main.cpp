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
#include "parse.hpp"
#include "Val.h"
using namespace std;

string s1 = "--help";
string s2 = "--test";
string s3 = "--interp";
string s4 = "--print";
string s5 = "--pretty-print";


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

            else if(argv[i] == s3){
                try{
                PTR(expr) e = parse(std::cin);
                cout<<e->interp()->to_expr()->to_string()<<endl;
                exit(0);}catch (runtime_error exn){
                    std::cerr << exn.what() << "\n";
                    return 1;
                }
            }

            else if(argv[i] == s4){
                try{
                PTR(expr) e = parse(std::cin);
                cout<<e->to_string()<<endl;
                exit(0);}catch (runtime_error exn){
                    std::cerr << exn.what() << "\n";
                    return 1;
                }
            }

            else if(argv[i] == s5){
                try{
                PTR(expr) e = parse(std::cin);
                cout<<e->to_pretty_string()<<endl;
                exit(0);}catch (runtime_error exn){
                    std::cerr << exn.what() << "\n";
                    return 1;}
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

TEST_CASE("equals"){
//test objects
    NumExpr n1(3);
    VarExpr v1("Jin");

    PTR(expr)f1 = NEW(NumExpr)(2);
    PTR(expr)f2 = NEW(NumExpr)(3);
    PTR(expr)f3 = NEW(NumExpr)(4);
    PTR(expr)f4 = NEW(NumExpr)(5);

    PTR(expr)e0 = NEW(NumExpr)(3);
    PTR(expr)e1 = NEW(NumExpr)(4);
    PTR(expr)e2 = NEW(AddExpr)(f1, e0);
    PTR(expr)e3 = NEW(AddExpr)(f3, f4);
    PTR(expr)e4 = NEW(MultExpr)(f3, f4);
    PTR(expr)e5 = NEW(MultExpr)(f3, f3);
    PTR(expr)e6 = NEW(VarExpr)("Jin");
    PTR(expr)e7 = NEW(VarExpr)("Jia");

    AddExpr a1(f1, f2);

    MultExpr m1(f3, f4);

//test expression NumExpr
    CHECK(n1.equals(e0));
    CHECK(!n1.equals(e1));
    CHECK(!n1.equals(e2));
    CHECK(!n1.equals(e4));
    CHECK(!n1.equals(e7));

//test expression AddExpr
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

//test expression VarExpr
    CHECK(v1.equals(e6));
    CHECK(!v1.equals(e7));
    CHECK(!v1.equals(e1));
    CHECK(!v1.equals(e2));
    CHECK(!v1.equals(e3));
    CHECK(!v1.equals(e4));
}

TEST_CASE("interp"){
    NumExpr n1(3);
    VarExpr v1("Jin");
    PTR(expr)f1 = NEW(NumExpr)(2);
    PTR(expr)f2 = NEW(NumExpr)(3);
    PTR(expr)f3 = NEW(NumExpr)(4);
    PTR(expr)f4 = NEW(NumExpr)(5);
    AddExpr a1(f1, f2);
    MultExpr m1(f3, f4);
    PTR(Val) res_Num = n1.interp();
    PTR(Val) res_Add = a1.interp();
    PTR(Val) res_Mult = m1.interp();
    PTR(expr)f5 = NEW(AddExpr)((NEW(VarExpr)("s")),
                           (NEW(NumExpr)(1)));
    PTR(expr)f6 = NEW(MultExpr)((NEW(VarExpr)("s")),
                            (NEW(NumExpr)(1)));
//test interp
    CHECK(res_Num->equals(NEW(NumVal)(3)));
    CHECK(res_Add->equals(NEW(NumVal)(5)));
    CHECK(res_Mult->equals(NEW(NumVal)(20)));

    CHECK_THROWS_WITH(f5->interp(),"VarExpr has no value" );
    CHECK_THROWS_WITH( v1.interp(), "VarExpr has no value" );
    CHECK_THROWS_WITH(f6->interp(),"VarExpr has no value" );
}


TEST_CASE("has_variable"){
//test has_variable
    CHECK((NEW(NumExpr)(5))->has_variable() == false);
    CHECK((NEW(AddExpr)((NEW(VarExpr)("s")), (NEW(NumExpr)(1))))->has_variable());
    CHECK((NEW(MultExpr)((NEW(VarExpr)("s")), (NEW(NumExpr)(1))))->has_variable());
    CHECK((NEW(AddExpr)((NEW(NumExpr)(3)), (NEW(NumExpr)(1))))->has_variable() == false);
    CHECK((NEW(MultExpr)((NEW(NumExpr)(3)), (NEW(NumExpr)(1))))->has_variable() == false);
    CHECK((NEW(VarExpr)("1"))->has_variable());

}

TEST_CASE("subst"){
//test subst

    PTR(expr)add = NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("xyz"), NEW(NumExpr)(10)), NEW(NumExpr)(11));
    PTR(expr)resAdd = NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(10)), NEW(NumExpr)(11));
    PTR(expr)mut = NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("xyz"), NEW(NumExpr)(10)), NEW(NumExpr)(11));
    PTR(expr)resMut = NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(10)), NEW(NumExpr)(11));

    CHECK(add->subst("xyz", NEW(NumVal)(20))->equals(resAdd));
    CHECK(mut->subst("xyz", NEW(NumVal)(20))->equals(resMut));


}

TEST_CASE("print && pretty_print"){

//-----test print
    stringstream out("");
    PTR(expr)f1 = NEW(NumExpr)(2);
    f1->print(out);
    CHECK( out.str() == "2");

    stringstream out1("");
    PTR(expr)f2 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)));
    f2->print(out1);
    CHECK(out1.str() == "(1+(2+3))");

    stringstream out2("");
    PTR(expr)f3 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)));
    f3->print(out2);
    CHECK(out2.str() == "(1+(2*3))");

//---test pretty_print
    PTR(expr) num1 = NEW(NumExpr)(1);
    PTR(expr) num2 = NEW(NumExpr)(2);
    PTR(expr) num3 = NEW(NumExpr)(3);
    PTR(expr) m0 = NEW(MultExpr)(num1, num2);
    PTR(expr) m2 = NEW(MultExpr)(m0, num3);
    PTR(expr) m4 = NEW(MultExpr)(num3, m0);
    PTR(expr) add = NEW(AddExpr)(num1, m0);
    PTR(expr) add2 = NEW(AddExpr)(m0, m2);
    PTR(expr) add1 = NEW(AddExpr)(num2, num3);
    PTR(expr) m3 = NEW(MultExpr)(num3, add1);
    PTR(VarExpr)v = NEW(VarExpr)("s");

    CHECK(num1->to_string()=="1");
    CHECK(m2->to_string() == "(1 * 2) * 3");
    CHECK(m4->to_string() == "3 * 1 * 2");
    CHECK(add->to_string() == "1 + 1 * 2");
//    CHECK(add2->to_string() =="(1 * 2) + (1 * 2) * 3");
    CHECK(m3->to_string() == "3 * (2 + 3)");
    CHECK(v->to_string()=="s" );

}

TEST_CASE("letExpr Subst"){
// test1 =
//          letExpr x = 4;
//          _in 7;          -> subst ("x", 1)
    PTR(expr)test1 = NEW(letExpr)(("x"), NEW(NumExpr)(4), NEW(NumExpr)(7));
    CHECK((test1 -> subst("x", NEW(NumVal)(1))) -> equals(test1));
// test2
//          letExpr x = x + 2;
//          _in x + 2;          -> subst ("x", 5)
    PTR(expr)test2 = NEW(letExpr)(("x"),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    CHECK(test2 -> subst("x", NEW(NumVal)(5)) -> equals(
            NEW(letExpr)(("x"),
                        NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)),
                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
// test3 =
//          letExpr x = x + 2;
//          _in x + 2;          -> subst ("y", 5)
    PTR(expr)test3 = NEW(letExpr)(("x"),
                              NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)),
                              NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
    PTR(expr) ans = NEW(letExpr)(("x"),
                            NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)),
                            NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)));
    PTR(expr) test4 = test3 -> subst("y", NEW(NumVal)(5));
//    cout << test3 ->to_pretty_string() << endl;
//    cout << test4 ->to_pretty_string() << endl;
//    cout << ans -> to_pretty_string() << endl;

    CHECK(test4 -> equals(ans));
}

TEST_CASE("letExpr Equals"){
// letExpr x = 6;
// _in x + 3;
    PTR(letExpr)test = NEW(letExpr)(("x"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)));
    CHECK(test-> equals(NEW(letExpr)(("x"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))) == true);
    CHECK(test-> equals(NEW(letExpr)(("y"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))) == false);
    CHECK(test-> equals(NEW(NumExpr)(6)) == false);
}

TEST_CASE("letExpr Has VarExpr"){
// test1 =
//          letExpr x = 2;
//          _in 4;
    PTR(expr)test1 = NEW(letExpr)(("x"), NEW(NumExpr)(2), NEW(NumExpr)(4));
    CHECK(test1 -> has_variable() == false);
// test2 =
//          letExpr x = x + 4;
//          _in 7;
    PTR(expr)test2 = NEW(letExpr)(("x"),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)),
                              NEW(NumExpr)(7));
    CHECK(test2 -> has_variable() == true);
// test3 =
//          letExpr x = 2;
//          _in x + 7;
    PTR(expr)test3 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(2),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)));
    CHECK(test3 -> has_variable() == true);
// test4 =
//          letExpr x = x + 6;
//          _in x + 5;
    PTR(expr)test4 = NEW(letExpr)(("x"),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6)),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)));
    CHECK(test4 -> has_variable() == true);
}

TEST_CASE("letExpr Print"){
// test1 =
//        letExpr x = 12;
//        _in x + 1;
    PTR(expr)test1 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(12),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    CHECK((test1 -> to_string()) == "(letExpr x=12 _in (x+1))");
// test2 =
//        letExpr x = 12;
//        _in letExpr x = 4;
//            _in x + 2;
    PTR(expr)test2 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(12),
                              NEW(letExpr)(("x"),
                                          NEW(NumExpr)(4),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))));
    CHECK((test2 -> to_string()) == "(letExpr x=12 _in (letExpr x=4 _in (x+2)))");
// test3 =
//        letExpr x = y + 2;
//        _in y + 3;
    PTR(expr)test3 = NEW(letExpr)(("x"),
                              NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)),
                              NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)));
    CHECK((test3 -> to_string()) == "(letExpr x=(y+2) _in (y+3))");
}

TEST_CASE("letExpr Interp"){
// test1 =
//        letExpr x = 4;
//        _in 7;
    PTR(expr)test1 = NEW(letExpr)(("x"), NEW(NumExpr)(4), NEW(NumExpr)(7));
    CHECK( test1 -> interp() ->equals(NEW(NumVal)(7)) );
// test2 =
//        letExpr x = 2;
//        _in x + 1;
    PTR(expr)test2 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(2),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    CHECK((test2 -> interp()) ->equals(NEW(NumVal)(3)) );
// test3 =
//        letExpr x = 7;
//        _in y + 2;
    PTR(expr)test3 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(7),
                              NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
    CHECK_THROWS_WITH( test3 -> interp(),
                       "VarExpr has no value" );
// test4 =
//        letExpr x = 4;
//        _in letExpr x = 7;
//            _in x + 1;
    PTR(expr)test4 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(4),
                              NEW(letExpr)(("x"),
                                          NEW(NumExpr)(7),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))));
    CHECK(test4 -> interp()->equals(NEW(NumVal)(8)) );

// test6 =
//        letExpr x = 9;
//        _in letExpr x = x + 2;
//            _in x + 2;
    PTR(expr)test6 = NEW(letExpr)(("x"), NEW(NumExpr)(9),
                              NEW(letExpr)(("x"),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))));
    CHECK((test6 -> interp()) ->equals(NEW(NumVal)(13)) );

}

TEST_CASE("letExpr Pretty Print"){
// test1 =
//        letExpr x = 4
//        _in x + 1
    PTR(expr)test1 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(4),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    CHECK((test1 -> to_pretty_string()) == "letExpr x = 4\n_in x + 1");
// test2 =
//        (letExpr x = 7
//        _in x) + 9
    PTR(expr)test2 = NEW(AddExpr)(NEW(letExpr)(("x"), NEW(NumExpr)(7), NEW(VarExpr)("x")),
                              NEW(NumExpr)(9));
    CHECK((test2 -> to_pretty_string()) == "(letExpr x = 7\n _in x) + 9");
// test3 =
//        7 * (letExpr x = 9
//             _in  x) + 3
    PTR(expr)test3 = NEW(AddExpr) (NEW(MultExpr)(NEW(NumExpr)(7),
                                            NEW(letExpr)(("x"), NEW(NumExpr)(9), NEW(VarExpr)("x"))),
                               NEW(NumExpr)(3));
    CHECK((test3 -> to_pretty_string()) == "7 * (letExpr x = 9\n     _in x) + 3");
// test4 =
//        6 * letExpr x = 9
//            _in  x + 1
    PTR(expr)test4 = NEW(MultExpr)(NEW(NumExpr)(6),
                               NEW(letExpr)(("x"),
                                           NEW(NumExpr)(9),
                                           NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))));
    CHECK((test4 -> to_pretty_string()) == "6 * letExpr x = 9\n    _in (x + 1)");
// test5 =
//        letExpr x = 7
//        _in letExpr x = 2
//            _in x + 2
    PTR(expr)test5 = NEW(letExpr)(("x"),
                              NEW(NumExpr)(7),
                              NEW(letExpr)(("x"), NEW(NumExpr)(2),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))));
    CHECK((test5 -> to_pretty_string()) == "letExpr x = 7\n_in letExpr x = 2\n    _in x + 2");
// test6 =
//        letExpr x = letExpr x = 7
//                 _in x + 2
//        _in x + 2
    PTR(expr)test6 = NEW(letExpr)(("x"),
                              NEW(letExpr)(("x"), NEW(NumExpr)(7),
                                          NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
    CHECK((test6 -> to_pretty_string()) == "letExpr x = letExpr x = 7\n            _in x + 2\n_in x + 2");
// test7 =
//       (7 + (letExpr x = letExpr x = 2
//                      _in x + 3
//             _in x + 3)) * 6
    PTR(expr)test7 = NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(7),
                                           NEW(letExpr)(("x"),
                                                       NEW(letExpr)(("x"),
                                                                   NEW(NumExpr)(2),
                                                                   NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))),
                                                       NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))),
                               NEW(NumExpr)(6));
    CHECK((test7 -> to_pretty_string()) == "(7 + (letExpr x = letExpr x = 2\n                  _in x + 3\n      _in x + 3)) * 6");

}

TEST_CASE("Parse"){
    //parse_Num
    CHECK(parse_str("19")->equals(NEW(NumExpr)(19)) );
    CHECK(parse_str("-19")->equals(NEW(NumExpr)(-19)) );
    CHECK(parse_str("(19)")->equals(NEW(NumExpr)(19)) );
    CHECK(parse_str("(-19)")->equals(NEW(NumExpr)(-19)) );

    //parse AddExpr
    CHECK(parse_str("3+2")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))) );
    CHECK(parse_str("(3+2)")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))) );
    CHECK(parse_str("3+-2")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(-2))) );
    CHECK(parse_str("(3)+2")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))) );
    CHECK(parse_str("(3)+(2)")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2))) );
    CHECK(parse_str("3+(-2)")->equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(-2))) );

    //parse mult
    CHECK( parse_str("2+2*3")
                   ->equals(NEW(AddExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))));
    CHECK( parse_str("2*2+3")
                   ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(NumExpr)(3))));
    CHECK( parse_str("6*4*3")
                   ->equals(NEW(MultExpr)(NEW(NumExpr)(6), NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)))));
    CHECK( parse_str("6+4+3")
                   ->equals(NEW(AddExpr)(NEW(NumExpr)(6), NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)))));
    CHECK( parse_str("6*(8+3)")
                   ->equals(NEW(MultExpr)(NEW(NumExpr)(6), NEW(AddExpr)(NEW(NumExpr)(8), NEW(NumExpr)(3)))));
    CHECK( parse_str("(4+3)*8")
                   ->equals(NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)), NEW(NumExpr)(8) )));


    //parse_var
    CHECK( parse_str("xyz")
                   ->equals(NEW(VarExpr)("xyz")));
    CHECK( parse_str("(xyz)+(1)")
                   ->equals(NEW(AddExpr)(NEW(VarExpr)("xyz"), NEW(NumExpr)(1))) );

    //parse space and parentheses
    CHECK( parse_str(" 13 ")
                   ->equals(NEW(NumExpr)(13)));
    CHECK( parse_str(" (  13     ) ")
                   ->equals(NEW(NumExpr)(13)));
    CHECK( parse_str("   11  +  1")
                   ->equals(NEW(AddExpr)(NEW(NumExpr)(11), NEW(NumExpr)(1))));
    CHECK( parse_str(" ( 11 + (1) ) ")
                   ->equals(NEW(AddExpr)(NEW(NumExpr)(11), NEW(NumExpr)(1))));
    CHECK( parse_str(" 11 * ( 11 + 1 ) ")
                   ->equals(NEW(MultExpr)(NEW(NumExpr)(11), NEW(AddExpr)(NEW(NumExpr)(11), NEW(NumExpr)(1)))));
    CHECK( parse_str(" ( 11 * 11 ) + 1 ")
                   ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(11), NEW(NumExpr)(11)), NEW(NumExpr)(1))));
    CHECK( parse_str(" 1 + (2 * 3)")
                   ->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))));

    //parse let
//    PTR(expr) l0 = NEW(letExpr)(NEW(VarExpr)("x"),
//                           NEW(NumExpr)(4),
//                           NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
//    CHECK( parse_str("let x = 4\n_in x + 1")->equals(l0));
//
//
//    PTR(expr) l1 = NEW(AddExpr)(NEW(letExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7), NEW(VarExpr)("x")),
//                           NEW(NumExpr)(9));
//    CHECK(parse_str("(let x = 7\n _in x) + 9")->equals(l1));
//
//    PTR(expr) l2 = NEW(AddExpr) (NEW(MultExpr)(NEW(NumExpr)(7),
//                                         NEW(letExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9), NEW(VarExpr)("x"))),
//                            NEW(NumExpr)(3));
//
//    CHECK(parse_str("7 * (let x = 9\n     _in x) + 3")->equals(l2));
//
//    PTR(expr) l3 = NEW(letExpr)(NEW(VarExpr)("x"),
//                           NEW(NumExpr)(7),
//                           NEW(letExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2),
//                                       NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))));
//    CHECK(parse_str("let x = 7\n_in let x = 2\n    _in x + 2")->equals(l3));
//    CHECK(parse_str("(7 + (let x = let x = 2 _in x + 3 _in x + 3)) * 6")->equals(NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(7),
//                                                                                                            NEW(letExpr)(NEW(VarExpr)("x"),
//                                                                                                                        NEW(letExpr)(NEW(VarExpr)("x"),
//                                                                                                                                    NEW(NumExpr)(2),
//                                                                                                                                    NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))),
//                                                                                                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))),
//                                                                                                NEW(NumExpr)(6))));
//
//    CHECK( parse_str("letExpr x = letExpr x = 7 _in x + 2 _in x + 2")->equals(NEW(letExpr)(NEW(VarExpr)("x"),
//                                                                                    NEW(letExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7),
//                                                                                                NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))),
//                                                                                    NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
}

TEST_CASE("BoolVal"){

    PTR(Val) b0 = NEW(BoolVal)(true);
    PTR(Val) b1 = NEW(BoolVal)(false);

    //test equals
    CHECK(b0->equals(b1) == false);
    CHECK(b0->equals(NEW(BoolVal)(true)) == true);

    //test is_true
    CHECK(b0->is_true() == true);
    CHECK(b1->is_true() == false);

    //test to_string

    CHECK(b0->to_string() == "true");
    CHECK(b1->to_string() == "false");
}

TEST_CASE("BoolExpr"){
    SECTION("equals") {
        CHECK((NEW (BoolExpr)(true))
                      ->equals(NEW (BoolExpr)(true)));
        CHECK(!(NEW(BoolExpr)(true))
                ->equals(NEW (BoolExpr)(false)));
        CHECK(!(NEW(BoolExpr)(true))
                ->equals(NULL));
    }

    SECTION("interp") {
        CHECK((NEW(BoolExpr)(true))->interp()
                      ->equals(NEW(BoolVal)(true)));
        CHECK((NEW(BoolExpr)(false))->interp()
                      ->equals(NEW(BoolVal)(false)));
    }

    SECTION("subst"){
        CHECK( (NEW(BoolExpr)(true))->subst("x", NEW(NumVal)(5))
                       ->equals(NEW(BoolExpr)(true)));
    }

    SECTION("has_variable"){
        CHECK( (NEW(BoolExpr)(true))->has_variable()
               == false );
    }

    SECTION("to_string"){
        CHECK( (NEW(BoolExpr)(true))->to_string()
               == "_true");
        CHECK( (NEW(BoolExpr)(false))->to_string()
               == "_false");
    }

}

TEST_CASE("EqualExpr"){

    SECTION("equals") {
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(7), NEW(NumExpr)(10)))
                       ->equals(NEW(EqualExpr)(NEW(NumExpr)(7), NEW(NumExpr)(10))) );
        CHECK( ! (NEW(EqualExpr)(NEW(NumExpr)(7), NEW(NumExpr)(10)))
                ->equals(NEW(EqualExpr)(NEW(NumExpr)(8), NEW(NumExpr)(10))) );
        CHECK( ! (NEW(EqualExpr)(NEW(NumExpr)(7), NEW(NumExpr)(9)))
                ->equals(NEW(EqualExpr)(NEW(NumExpr)(10), NEW(NumExpr)(9))) );
        CHECK( ! (NEW(EqualExpr)(NEW(NumExpr)(10), NEW(NumExpr)(9)))
                ->equals(NEW(NumExpr)(10)) );
    }

    SECTION("interp"){
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(4), NEW(NumExpr)(4)))->interp()
                       ->equals(NEW(BoolVal)(true)) );
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(5), NEW(NumExpr)(4)))->interp()
                       ->equals(NEW(BoolVal)(false)) );
    }

    SECTION("subst"){
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")))->subst("x", NEW(NumVal)(4))
                       ->equals(NEW(EqualExpr)(NEW(NumExpr)(2), NEW(NumExpr)(4))) );
    }

    SECTION("has_variable"){
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(2), NEW(NumExpr)(1)))->has_variable()
               == false );
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")))->has_variable()
               == true );
    }

    SECTION("to_string"){
        CHECK( (NEW(EqualExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)))->to_string()
               == "4 == 3");
    }
}

TEST_CASE("IfExpr"){

    SECTION("equals") {
        CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(5)))
                      ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(5))));
        CHECK(!(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(5)))
                ->equals(NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(3), NEW(NumExpr)(5))));
        CHECK(!(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(4)))
                ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(4))));
        CHECK(!(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(4)))
                ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(6))));
        CHECK(!(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(4)))
                ->equals(NULL));
    }

    SECTION("interp"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(2), NEW(NumExpr)(5)))->interp()
                       ->equals(NEW(NumVal)(2)));
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(2), NEW(NumExpr)(5)))->interp()
                       ->equals(NEW(NumVal)(5)));
        CHECK( (NEW(IfExpr)(NEW(EqualExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(NumExpr)(9), NEW(NumExpr)(10)))->interp()
                       ->equals(NEW(NumVal)(9)));
        CHECK( (NEW(IfExpr)(NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(8), NEW(NumExpr)(7)))->interp()
                       ->equals(NEW(NumVal)(7)));
    }

    SECTION("subst"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VarExpr)("x"), NEW(NumExpr)(2)))->subst("x", NEW(NumVal)(5))
                       ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(2))));
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(VarExpr)("x")))->subst("x", NEW(NumVal)(2))
                       ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(2))));
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5), NEW(VarExpr)("x")))->subst("x", NEW(NumVal)(2))
                       ->equals(NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5), NEW(NumExpr)(2))));
    }

    SECTION("has_variable"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(3)))->has_variable()
               == false );
        CHECK( (NEW(IfExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(NumExpr)(3)))->has_variable()
               == true );
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VarExpr)("x"), NEW(NumExpr)(6)))->has_variable()
               == true );
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(9), NEW(VarExpr)("x")))->has_variable()
               == true );
    }

    SECTION("to_string"){
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(2)))->to_string()
               == "(_if _true _then 3 _else 2)" );
    }

}

TEST_CASE("FunExpr"){

    SECTION("Equals"){
        CHECK( (NEW(FunExpr)("z", NEW(VarExpr)("x")))
                       ->equals(NEW(FunExpr)("z", NEW(VarExpr)("x"))));
        CHECK( (NEW(FunExpr)("z", NEW(NumExpr)(3)))
                       ->equals(NEW(FunExpr)("z", NEW(NumExpr)(3))));
        CHECK( ! (NEW(FunExpr)("x", NEW(NumExpr)(4)))
                ->equals(NEW(FunExpr)("y", NEW(NumExpr)(4))));
        CHECK( ! (NEW(FunExpr)("x", NEW(NumExpr)(31)))
                ->equals(NEW(FunExpr)("x", NEW(NumExpr)(41))));
        CHECK( ! (NEW(FunExpr)("x", NEW(NumExpr)(31)))
                ->equals(NULL));
    }

    SECTION("interp"){
        CHECK( (NEW(FunExpr)("x", NEW(NumExpr)(41)))->interp()
                       ->equals(NEW(FunVal)("x", NEW(NumExpr)(41))));
        CHECK( (NEW(FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(41), NEW(VarExpr)("x"))))->interp()
                       ->equals(NEW(FunVal)("x", NEW(AddExpr)(NEW(NumExpr)(41), NEW(VarExpr)("x")))));
    }

    SECTION("subst"){

        CHECK((NEW (FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4))))->subst("x", NEW(NumVal)(5))->equals(
                NEW (FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)))
                ));

        CHECK((NEW (FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(4))))->subst("z", NEW(NumVal)(5))->equals(
                NEW (FunExpr)("x", NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(4)))
        ));

    }

    SECTION("has_variable"){
        CHECK( (NEW(FunExpr)("x", NEW(NumExpr)(14)))->has_variable()
               == true);
    }

    SECTION("to_string"){
        CHECK( (NEW(FunExpr)("x", NEW(NumExpr)(15)))->to_string()
               == "(_fun (x) 15)");
    }
}

TEST_CASE("CallExpr"){

    SECTION("Equals"){
        CHECK( (NEW(CallExpr)(NEW(NumExpr)(13), NEW(NumExpr)(13)))
                       ->equals(NEW(CallExpr)(NEW(NumExpr)(13), NEW(NumExpr)(13))));
        CHECK( ! (NEW(CallExpr)(NEW(NumExpr)(31), NEW(NumExpr)(31)))
                ->equals(NEW(CallExpr)(NEW(NumExpr)(41), NEW(NumExpr)(31))));
        CHECK( ! (NEW(CallExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
                ->equals(NEW(CallExpr)(NEW(NumExpr)(3), NEW(VarExpr)("x"))));
        CHECK( ! (NEW(CallExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))
                ->equals(NULL));
    }

    SECTION("interp"){

    }

    SECTION("subst"){

    }

    SECTION("has_variable"){
        CHECK( (NEW(CallExpr)(NEW(NumExpr)(4), NEW(NumExpr)(4)))->has_variable()
               == true);
    }

    SECTION("to_string"){
        CHECK( (NEW(CallExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))->to_string()
               == ", (x(3))");
    }
}