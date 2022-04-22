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
                expr * e = parse(std::cin);
                cout<<e->interp()->to_expr()->to_string()<<endl;
                exit(0);}catch (runtime_error exn){
                    std::cerr << exn.what() << "\n";
                    return 1;
                }
            }

            else if(argv[i] == s4){
                try{
                expr * e = parse(std::cin);
                cout<<e->to_string()<<endl;
                exit(0);}catch (runtime_error exn){
                    std::cerr << exn.what() << "\n";
                    return 1;
                }
            }

            else if(argv[i] == s5){
                try{
                expr * e = parse(std::cin);
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

    expr *f1 = new NumExpr(2);
    expr *f2 = new NumExpr(3);
    expr *f3 = new NumExpr(4);
    expr *f4 = new NumExpr(5);

    expr *e0 = new NumExpr(3);
    expr *e1 = new NumExpr(4);
    expr *e2 = new AddExpr(f1, e0);
    expr *e3 = new AddExpr(f3, f4);
    expr *e4 = new MultExpr(f3, f4);
    expr *e5 = new MultExpr(f3, f3);
    expr *e6 = new VarExpr("Jin");
    expr *e7 = new VarExpr("Jia");

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
    expr *f1 = new NumExpr(2);
    expr *f2 = new NumExpr(3);
    expr *f3 = new NumExpr(4);
    expr *f4 = new NumExpr(5);
    AddExpr a1(f1, f2);
    MultExpr m1(f3, f4);
    Val* res_Num = n1.interp();
    Val* res_Add = a1.interp();
    Val* res_Mult = m1.interp();
    expr *f5 = new AddExpr((new VarExpr("s")),
                           (new NumExpr(1)));
    expr *f6 = new MultExpr((new VarExpr("s")),
                            (new NumExpr(1)));
//test interp
    CHECK(res_Num->equals(new NumVal(3)));
    CHECK(res_Add->equals(new NumVal(5)));
    CHECK(res_Mult->equals(new NumVal(20)));
    CHECK_THROWS_WITH(f5->interp(),"VarExpr has no value" );
    CHECK_THROWS_WITH( v1.interp(), "VarExpr has no value" );
    CHECK_THROWS_WITH(f6->interp(),"VarExpr has no value" );
}


TEST_CASE("has_variable"){
//test has_variable
    CHECK((new NumExpr(5))->has_variable() == false);
    CHECK((new AddExpr((new VarExpr("s")), (new NumExpr(1))))->has_variable());
    CHECK((new MultExpr((new VarExpr("s")), (new NumExpr(1))))->has_variable());
    CHECK((new AddExpr((new NumExpr(3)), (new NumExpr(1))))->has_variable() == false);
    CHECK((new MultExpr((new NumExpr(3)), (new NumExpr(1))))->has_variable() == false);
    CHECK((new VarExpr("1"))->has_variable());

}

TEST_CASE("subst"){
//test subst

    expr *add = new AddExpr(new AddExpr(new VarExpr("xyz"), new NumExpr(10)), new NumExpr(11));
    expr *resAdd = new AddExpr(new AddExpr(new NumExpr(20), new NumExpr(10)), new NumExpr(11));
    expr *mut = new MultExpr(new AddExpr(new VarExpr("xyz"), new NumExpr(10)), new NumExpr(11));
    expr *resMut = new MultExpr(new AddExpr(new NumExpr(20), new NumExpr(10)), new NumExpr(11));
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", reinterpret_cast<Val *>(new VarExpr("y")))
                   ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );

    CHECK(add->subst("xyz", new NumVal(20))->equals(resAdd));
    CHECK(mut->subst("xyz", new NumVal(20))->equals(resMut));

    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", reinterpret_cast<Val *>(new VarExpr("y")))
                   ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) );


}

TEST_CASE("print && pretty_print"){

//-----test print
    stringstream out("");
    expr *f1 = new NumExpr(2);
    f1->print(out);
    CHECK( out.str() == "2");

    stringstream out1("");
    expr *f2 = new AddExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)));
    f2->print(out1);
    CHECK(out1.str() == "(1+(2+3))");

    stringstream out2("");
    expr *f3 = new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3)));
    f3->print(out2);
    CHECK(out2.str() == "(1+(2*3))");

//---test pretty_print
    NumExpr* num1 = new NumExpr(1);
    NumExpr* num2 = new NumExpr(2);
    NumExpr* num3 = new NumExpr(3);
    MultExpr* m0 = new MultExpr(num1, num2);
    MultExpr* m2 = new MultExpr(m0, num3);
    MultExpr* m4 = new MultExpr(num3, m0);
    AddExpr* add = new AddExpr(num1, m0);
    AddExpr* add2 = new AddExpr(m0, m2);
    AddExpr* add1 = new AddExpr(num2, num3);
    MultExpr* m3 = new MultExpr(num3, add1);
    VarExpr *v = new VarExpr("s");

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
    expr *test1 = new letExpr(new VarExpr("x"), new NumExpr(4), new NumExpr(7));
    CHECK((test1 -> subst("x", new NumVal(1))) -> equals(test1));
// test2 =
//          letExpr x = x + 2;
//          _in x + 2;          -> subst ("x", 5)
    expr *test2 = new letExpr(new VarExpr("x"),
                              new AddExpr(new VarExpr("x"), new NumExpr(2)),
                              new AddExpr(new VarExpr("x"), new NumExpr(2)));
    CHECK(test2 -> subst("x", new NumVal(5)) -> equals(
            new letExpr(new VarExpr("x"),
                        new AddExpr(new NumExpr(5), new NumExpr(2)),
                        new AddExpr(new VarExpr("x"), new NumExpr(2)))));
// test3 =
//          letExpr x = x + 2;
//          _in x + 2;          -> subst ("y", 5)
    expr *test3 = new letExpr(new VarExpr("x"),
                              new AddExpr(new VarExpr("x"), new NumExpr(2)),
                              new AddExpr(new VarExpr("x"), new NumExpr(2)));
    CHECK(test3 -> subst("y", new NumVal(5)) -> equals(test3));
}

TEST_CASE("letExpr Equals"){
// letExpr x = 6;
// _in x + 3;
    letExpr *test = new letExpr(new VarExpr("x"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(3)));
    CHECK(test-> equals(new letExpr(new VarExpr("x"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(3)))) == true);
    CHECK(test-> equals(new letExpr(new VarExpr("y"), new NumExpr(6), new AddExpr(new VarExpr("y"), new NumExpr(3)))) == false);
    CHECK(test-> equals(new NumExpr(6)) == false);
}

TEST_CASE("letExpr Has VarExpr"){
// test1 =
//          letExpr x = 2;
//          _in 4;
    expr *test1 = new letExpr(new VarExpr("x"), new NumExpr(2), new NumExpr(4));
    CHECK(test1 -> has_variable() == false);
// test2 =
//          letExpr x = x + 4;
//          _in 7;
    expr *test2 = new letExpr(new VarExpr("x"),
                              new AddExpr(new VarExpr("x"), new NumExpr(4)),
                              new NumExpr(7));
    CHECK(test2 -> has_variable() == true);
// test3 =
//          letExpr x = 2;
//          _in x + 7;
    expr *test3 = new letExpr(new VarExpr("x"),
                              new NumExpr(2),
                              new AddExpr(new VarExpr("x"), new NumExpr(7)));
    CHECK(test3 -> has_variable() == true);
// test4 =
//          letExpr x = x + 6;
//          _in x + 5;
    expr *test4 = new letExpr(new VarExpr("x"),
                              new AddExpr(new VarExpr("x"), new NumExpr(6)),
                              new AddExpr(new VarExpr("x"), new NumExpr(5)));
    CHECK(test4 -> has_variable() == true);
}

TEST_CASE("letExpr Print"){
// test1 =
//        letExpr x = 12;
//        _in x + 1;
    expr *test1 = new letExpr(new VarExpr("x"),
                              new NumExpr(12),
                              new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CHECK((test1 -> to_string()) == "(letExpr x=12 _in (x+1))");
// test2 =
//        letExpr x = 12;
//        _in letExpr x = 4;
//            _in x + 2;
    expr *test2 = new letExpr(new VarExpr("x"),
                              new NumExpr(12),
                              new letExpr(new VarExpr("x"),
                                          new NumExpr(4),
                                          new AddExpr(new VarExpr("x"), new NumExpr(2))));
    CHECK((test2 -> to_string()) == "(letExpr x=12 _in (letExpr x=4 _in (x+2)))");
// test3 =
//        letExpr x = y + 2;
//        _in y + 3;
    expr *test3 = new letExpr(new VarExpr("x"),
                              new AddExpr(new VarExpr("y"), new NumExpr(2)),
                              new AddExpr(new VarExpr("y"), new NumExpr(3)));
    CHECK((test3 -> to_string()) == "(letExpr x=(y+2) _in (y+3))");
}

TEST_CASE("letExpr Interp"){
// test1 =
//        letExpr x = 4;
//        _in 7;
    expr *test1 = new letExpr(new VarExpr("x"), new NumExpr(4), new NumExpr(7));
    CHECK( test1 -> interp() ->equals(new NumVal(7)) );
// test2 =
//        letExpr x = 2;
//        _in x + 1;
    expr *test2 = new letExpr(new VarExpr("x"),
                              new NumExpr(2),
                              new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CHECK((test2 -> interp()) ->equals(new NumVal(3)) );
// test3 =
//        letExpr x = 7;
//        _in y + 2;
    expr *test3 = new letExpr(new VarExpr("x"),
                              new NumExpr(7),
                              new AddExpr(new VarExpr("y"), new NumExpr(2)));
    CHECK_THROWS_WITH( test3 -> interp(),
                       "VarExpr has no value" );
// test4 =
//        letExpr x = 4;
//        _in letExpr x = 7;
//            _in x + 1;
    expr *test4 = new letExpr(new VarExpr("x"),
                              new NumExpr(4),
                              new letExpr(new VarExpr("x"),
                                          new NumExpr(7),
                                          new AddExpr(new VarExpr("x"), new NumExpr(1))));
    CHECK(test4 -> interp()->equals(new NumVal(8)) );

// test6 =
//        letExpr x = 9;
//        _in letExpr x = x + 2;
//            _in x + 2;
    expr *test6 = new letExpr(new VarExpr("x"), new NumExpr(9),
                              new letExpr(new VarExpr("x"),
                                          new AddExpr(new VarExpr("x"), new NumExpr(2)),
                                          new AddExpr(new VarExpr("x"), new NumExpr(2))));
    CHECK((test6 -> interp()) ->equals(new NumVal(13)) );

}

TEST_CASE("letExpr Pretty Print"){
// test1 =
//        letExpr x = 4
//        _in x + 1
    expr *test1 = new letExpr(new VarExpr("x"),
                              new NumExpr(4),
                              new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CHECK((test1 -> to_pretty_string()) == "letExpr x = 4\n_in x + 1");
// test2 =
//        (letExpr x = 7
//        _in x) + 9
    expr *test2 = new AddExpr(new letExpr(new VarExpr("x"), new NumExpr(7), new VarExpr("x")),
                              new NumExpr(9));
    CHECK((test2 -> to_pretty_string()) == "(letExpr x = 7\n _in x) + 9");
// test3 =
//        7 * (letExpr x = 9
//             _in  x) + 3
    expr *test3 = new AddExpr (new MultExpr(new NumExpr(7),
                                            new letExpr(new VarExpr("x"), new NumExpr(9), new VarExpr("x"))),
                               new NumExpr(3));
    CHECK((test3 -> to_pretty_string()) == "7 * (letExpr x = 9\n     _in x) + 3");
// test4 =
//        6 * letExpr x = 9
//            _in  x + 1
    expr *test4 = new MultExpr(new NumExpr(6),
                               new letExpr(new VarExpr("x"),
                                           new NumExpr(9),
                                           new AddExpr(new VarExpr("x"), new NumExpr(1))));
    CHECK((test4 -> to_pretty_string()) == "6 * letExpr x = 9\n    _in (x + 1)");
// test5 =
//        letExpr x = 7
//        _in letExpr x = 2
//            _in x + 2
    expr *test5 = new letExpr(new VarExpr("x"),
                              new NumExpr(7),
                              new letExpr(new VarExpr("x"), new NumExpr(2),
                                          new AddExpr(new VarExpr("x"), new NumExpr(2))));
    CHECK((test5 -> to_pretty_string()) == "letExpr x = 7\n_in letExpr x = 2\n    _in x + 2");
// test6 =
//        letExpr x = letExpr x = 7
//                 _in x + 2
//        _in x + 2
    expr *test6 = new letExpr(new VarExpr("x"),
                              new letExpr(new VarExpr("x"), new NumExpr(7),
                                          new AddExpr(new VarExpr("x"), new NumExpr(2))),
                              new AddExpr(new VarExpr("x"), new NumExpr(2)));
    CHECK((test6 -> to_pretty_string()) == "letExpr x = letExpr x = 7\n         _in x + 2\n_in x + 2");
// test7 =
//       (7 + (letExpr x = letExpr x = 2
//                      _in x + 3
//             _in x + 3)) * 6
    expr *test7 = new MultExpr(new AddExpr(new NumExpr(7),
                                           new letExpr(new VarExpr("x"),
                                                       new letExpr(new VarExpr("x"),
                                                                   new NumExpr(2),
                                                                   new AddExpr(new VarExpr("x"), new NumExpr(3))),
                                                       new AddExpr(new VarExpr("x"), new NumExpr(3)))),
                               new NumExpr(6));
    CHECK((test7 -> to_pretty_string()) == "(7 + (letExpr x = letExpr x = 2\n               _in x + 3\n      _in x + 3)) * 6");

}

TEST_CASE("Parse"){
    //parse_Num
    CHECK(parse_str("19")->equals(new NumExpr(19)) );
    CHECK(parse_str("-19")->equals(new NumExpr(-19)) );
    CHECK(parse_str("(19)")->equals(new NumExpr(19)) );
    CHECK(parse_str("(-19)")->equals(new NumExpr(-19)) );

    //parse AddExpr
    CHECK(parse_str("3+2")->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) );
    CHECK(parse_str("(3+2)")->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) );
    CHECK(parse_str("3+-2")->equals(new AddExpr(new NumExpr(3), new NumExpr(-2))) );
    CHECK(parse_str("(3)+2")->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) );
    CHECK(parse_str("(3)+(2)")->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) );
    CHECK(parse_str("3+(-2)")->equals(new AddExpr(new NumExpr(3), new NumExpr(-2))) );

    //parse mult
    CHECK( parse_str("2+2*3")
                   ->equals(new AddExpr(new NumExpr(2), new MultExpr(new NumExpr(2), new NumExpr(3)))));
    CHECK( parse_str("2*2+3")
                   ->equals(new AddExpr(new MultExpr(new NumExpr(2), new NumExpr(2)), new NumExpr(3))));
    CHECK( parse_str("6*4*3")
                   ->equals(new MultExpr(new NumExpr(6), new MultExpr(new NumExpr(4), new NumExpr(3)))));
    CHECK( parse_str("6+4+3")
                   ->equals(new AddExpr(new NumExpr(6), new AddExpr(new NumExpr(4), new NumExpr(3)))));
    CHECK( parse_str("6*(8+3)")
                   ->equals(new MultExpr(new NumExpr(6), new AddExpr(new NumExpr(8), new NumExpr(3)))));
    CHECK( parse_str("(4+3)*8")
                   ->equals(new MultExpr(new AddExpr(new NumExpr(4), new NumExpr(3)), new NumExpr(8) )));


    //parse_var
    CHECK( parse_str("xyz")
                   ->equals(new VarExpr("xyz")));
    CHECK( parse_str("(xyz)+(1)")
                   ->equals(new AddExpr(new VarExpr("xyz"), new NumExpr(1))) );

    //parse space and parentheses
    CHECK( parse_str(" 13 ")
                   ->equals(new NumExpr(13)));
    CHECK( parse_str(" (  13     ) ")
                   ->equals(new NumExpr(13)));
    CHECK( parse_str("   11  +  1")
                   ->equals(new AddExpr(new NumExpr(11), new NumExpr(1))));
    CHECK( parse_str(" ( 11 + (1) ) ")
                   ->equals(new AddExpr(new NumExpr(11), new NumExpr(1))));
    CHECK( parse_str(" 11 * ( 11 + 1 ) ")
                   ->equals(new MultExpr(new NumExpr(11), new AddExpr(new NumExpr(11), new NumExpr(1)))));
    CHECK( parse_str(" ( 11 * 11 ) + 1 ")
                   ->equals(new AddExpr(new MultExpr(new NumExpr(11), new NumExpr(11)), new NumExpr(1))));
    CHECK( parse_str(" 1 + (2 * 3)")
                   ->equals(new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3)))));

    //parse let
    expr* l0 = new letExpr(new VarExpr("x"),
                           new NumExpr(4),
                           new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CHECK( parse_str("letExpr x = 4\n_in x + 1")->equals(l0));

    expr* l1 = new AddExpr(new letExpr(new VarExpr("x"), new NumExpr(7), new VarExpr("x")),
                           new NumExpr(9));
    CHECK(parse_str("(letExpr x = 7\n _in x) + 9")->equals(l1));

    expr* l2 = new AddExpr (new MultExpr(new NumExpr(7),
                                         new letExpr(new VarExpr("x"), new NumExpr(9), new VarExpr("x"))),
                            new NumExpr(3));

    CHECK(parse_str("7 * (letExpr x = 9\n     _in x) + 3")->equals(l2));

    expr* l3 = new letExpr(new VarExpr("x"),
                           new NumExpr(7),
                           new letExpr(new VarExpr("x"), new NumExpr(2),
                                       new AddExpr(new VarExpr("x"), new NumExpr(2))));
    CHECK(parse_str("letExpr x = 7\n_in letExpr x = 2\n    _in x + 2")->equals(l3));
    CHECK(parse_str("(7 + (letExpr x = letExpr x = 2 _in x + 3 _in x + 3)) * 6")->equals(new MultExpr(new AddExpr(new NumExpr(7),
                                                                                                            new letExpr(new VarExpr("x"),
                                                                                                                        new letExpr(new VarExpr("x"),
                                                                                                                                    new NumExpr(2),
                                                                                                                                    new AddExpr(new VarExpr("x"), new NumExpr(3))),
                                                                                                                        new AddExpr(new VarExpr("x"), new NumExpr(3)))),
                                                                                                new NumExpr(6))));

    CHECK( parse_str("letExpr x = letExpr x = 7 _in x + 2 _in x + 2")->equals(new letExpr(new VarExpr("x"),
                                                                                    new letExpr(new VarExpr("x"), new NumExpr(7),
                                                                                                new AddExpr(new VarExpr("x"), new NumExpr(2))),
                                                                                    new AddExpr(new VarExpr("x"), new NumExpr(2)))));
}

TEST_CASE("BoolVal"){

    BoolVal* b0 = new BoolVal(true);
    BoolVal* b1 = new BoolVal(false);

    //test equals
    CHECK(b0->equals(b1) == false);
    CHECK(b0->equals(new BoolVal(true)) == true);

    //test is_true
    CHECK(b0->is_true() == true);
    CHECK(b1->is_true() == false);

    //test to_string

    CHECK(b0->to_string() == "true");
    CHECK(b1->to_string() == "false");
}