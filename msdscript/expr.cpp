//
// Created by 谷金 on 1/17/22.
//


#include "expr.hpp"
#include<iostream>
#include<stdexcept>
#include <sstream>
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


string expr::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}


void expr::pretty_print(std::ostream &out){
//    this -> pretty_print_at(os, false, false, false);
    this -> pretty_print_at(out, prec_none, 0, 0, 0);
}


string expr::to_string_p(){
    std::stringstream ss;
    this -> pretty_print(ss);
    return ss.str();
}

//override pure virtual method of interface
//Class Num--------------------------------------------------------------------------
Num::Num(int val){
    this->val = val;
}

bool Num::equals(expr *e) {
    Num *t = dynamic_cast<Num*>(e);
    if (t == NULL)
        return false;
    else
        return (this->val == t->val);
}

int Num::interp(){
    return this->val;
}

bool Num::has_variable(){
    return false;
}

expr* Num::subst(string s1, expr *e){
    return this;
}

void Num::print(ostream &out){
    out<<this->val;
}

void Num::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, false, 0,0);
}

void Num:: pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){
//    stringstream ss;
//    ss << this->val;
//    string str = ss.str();
    out<<this->val;
}

//Class Add--------------------------------------------------------------------------
Add::Add(expr *lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool Add::equals(expr *e) {
    Add *t = dynamic_cast<Add*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs));
}

int Add::interp() {
//Those methods should not use `dynamic_cast` (which should only be used in `equals`)
    // You should recur to `interp` for `lhs` and `rhs`, instead.
//    Num *l = dynamic_cast<Num*>(this->lhs);
//    Num *r = dynamic_cast<Num*>(this->rhs);
        return (this->rhs->interp() + this->lhs->interp());
//    else(throw std::runtime_error("Variable has no value"));
}

bool Add::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Add::subst(string s1, expr *e){
    return (new Add((this -> lhs) -> subst(s1,e),
                    (this -> rhs) ->subst(s1,e)));
}

void Add::print(ostream &out){
//    stringstream o("");
//    lhs->print(o);
//    rhs->print(o);
//    out<<"("+lhs->print(o)+ "+" +rhs->print(o)+")";
    out<<"(";
    lhs->print(out);
    out<<"+";
    rhs->print(out);
    out<<")";
//    return "("+lhs->print(o)+ "+" +rhs->print(o)+")";
}

void Add::pretty_print(ostream &out){
    pretty_print_at(out,prec_none, 0,0,0);
}

void Add::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){

    long begin = out.tellp();
    if(p<2||(p == 2&&!isLeftInside)){
        this->lhs->pretty_print_at(out,prec_add,true,true,occupy);
        out<<" + ";
        long end1 = out.tellp();
        this->rhs->pretty_print_at(out,prec_add, false,isNested,occupy+(end1-begin));
    }else{
        out << "(";
        long end2 = out.tellp();
        this->lhs->pretty_print_at(out,prec_add,true,true,occupy+(int)(end2-begin));
        out << " + ";
        long end3 = out.tellp();
        this->rhs->pretty_print_at(out,prec_add,false,isNested,occupy+(int)(end3-begin));
        out << ")";
    }
}

string Add::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class Mult--------------------------------------------------------------------------
Mult::Mult(expr* lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

//override pure virtual method of interface
bool Mult::equals(expr *e) {
    Mult *t = dynamic_cast<Mult*>(e);
    if (t == NULL)
        return false;
    else
        return (this->lhs->equals(t->lhs)  && this->rhs->equals(t -> rhs) );
}

int Mult::interp() {
//    Num *l = dynamic_cast<Num*>(this->lhs);
//    Num *r = dynamic_cast<Num*>(this->rhs);
//    if(this->rhs!=NULL && this->lhs!=NULL && this->interp_())
        return (this->rhs->interp() * this->lhs->interp());
//    else(throw std::runtime_error("Variable has no value"));
}

bool Mult::has_variable(){
    if(this->lhs->has_variable()||this->rhs->has_variable()) return true;
    return false;
}

expr* Mult::subst(string s1, expr *e){
    return (new Mult((this->lhs) ->subst(s1,e),(this -> rhs) ->subst(s1,e)));
}

void Mult::print(ostream &out){
//    stringstream o("");
//    lhs->print(o);
//    rhs ->print(o);
    out<<"(";
    lhs->print(out);
    out<<"*";
    rhs->print(out);
    out<<")";
//    out<<"("+lhs->print(o)+ "*" +rhs->print(o)+")";
//    return "("+lhs->print(o)+ "*" +rhs->print(o)+")";
}

void Mult::pretty_print(ostream &out){
    pretty_print_at(out, prec_none, false,0,0);
}

void Mult::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){
//opposite to dfs we use true/false and accumulator to identify l/r side of the mult
//from top to bottom -> get the right answer
    long begin = out.tellp();
    if(isLeftInside == true&&p==3){
        out<<"(";
        long end1 = out.tellp();
        lhs->pretty_print_at(out,prec_mult,true,1,occupy+(int)(end1-begin));
        out<<" * ";
        long end2 = out.tellp();
        rhs->pretty_print_at(out, prec_mult, false,isNested,occupy+(int)(end2-begin));
        out<<")";
    }else{
        lhs->pretty_print_at(out,prec_mult,true,1,occupy);
        out<<" * ";
        long end3 = out.tellp();
        rhs->pretty_print_at(out, prec_mult, false,isNested,occupy+(int)(end3-begin));
    }
}

string Mult::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class Variable--------------------------------------------------------------------------
Variable::Variable(string input) {
    this -> s = input;
}

bool Variable::equals(expr *e) {
    Variable *t = dynamic_cast<Variable*>(e);
    if (t == NULL)
        return false;
    else
        return (this->s == t->s);
}

int Variable::interp(){
    throw std::runtime_error("Variable has no value");
}

bool Variable::has_variable(){
    if(this->s != "") return true;
    return false;
}

expr* Variable::subst(string s1, expr *e){
    if(this->s == s1){
        return e;
    }else {
        return this;
    }
}

void Variable::print(ostream &out){
    out<<this->s;
}

void Variable::pretty_print(ostream &out){

    pretty_print_at(out,prec_none, false,0,0);
}

void Variable::pretty_print_at(ostream &out,precedence_t p,bool isInside,bool isNested,int occupy){
    out << (this->s);
}

string Variable::to_string(){
    stringstream out("");
    pretty_print(out);
    return out.str();
}

//Class _Let--------------------------------------------------------------------------

_let::_let(Variable* variable, expr* rhs, expr* body){
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool _let::equals(expr *e){
    _let *t = dynamic_cast<_let*>(e);
    if (t == NULL)
        return false;
    else
        return (this->variable->equals(t->variable)
        && this->rhs->equals(t->rhs)
        && this->body->equals(t->body));
}

int _let::interp(){
    if(rhs->has_variable()) {
        return ((this->body)->subst(((this->variable)->to_string()),
                                    (this->rhs)))->interp();
    }
    else{
        expr *newRhs = new Num(rhs -> interp());
        return ((this->body)
                -> subst ((this->variable)->to_string(), newRhs))
                -> interp();
    }
}

bool _let::has_variable(){
    return this->rhs->has_variable()||this->body->has_variable();
}

expr* _let::subst(string s1, expr *e){

    if (s1 == this->variable->to_string()){
        return new _let(this->variable,
                        this->rhs->subst(s1,e),
                        this->body);
    }

    else{
        return new _let(this->variable,
                        this->rhs->subst(s1, e),
                        this->body->subst(s1, e));
    }
}
void _let::print(ostream &out){
    out << "(_let ";
    this->variable->print(out);
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}
void _let::pretty_print(ostream &out){
    pretty_print_at(out,prec_none,0,0,0);
}

string _let::to_string(){
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

void _let::pretty_print_at(ostream &out,precedence_t p,bool isLeftInside,bool isNested,int occupy){

    long begin_ = out.tellp();

    if ((p > 1 && isLeftInside) || isNested){
        out << "(_let ";
        this->variable->pretty_print_at(out, p, false, false, occupy+1);
        out << " = ";
        long end_p1 = out.tellp();
        this->rhs->pretty_print_at(out, p, false, false,
                                   occupy+(int)(end_p1-begin_));
        out << "\n";
        // spaces + 1: the '1' stands for the '(' in '(_let'
        for(int i=0; i<occupy+1; i++){
            out << " ";
        }
        out << "_in ";
        // spaces + 5: the '5' stands for the '(' + '_in '
        this->body->pretty_print_at(out, p, false, false, occupy+5);
        out << ")";
    }else{
        out << "_let ";
        this->variable->pretty_print_at(out, p, false, false, occupy);
        out << " = ";
        long end_p2 = out.tellp();
        this->rhs->pretty_print_at(out, p, false, false,
                                   occupy+(int)(end_p2-begin_));
        out << "\n";
        for(int i=0; i<occupy; i++){
            out << " ";
        }
        out << "_in ";
        // spaces + 4: the '4' stands for the '_in '
        this->body->pretty_print_at(out, p, false, false, occupy+4);
    }

}
//string _let::to_string();

/*
 * Test
 */
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
    //          _let x = 5;
    //          _in 6;          -> subst ("x", 1)
    expr *test1 = new _let(new Variable("x"), new Num(5), new Num(6));
    CHECK((test1 -> subst("x", new Num(1))) -> equals(test1));
    // test2 =
    //          _let x = x + 1;
    //          _in x + 1;          -> subst ("x", 5)
    expr *test2 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(1)),
                           new Add(new Variable("x"), new Num(1)));
    CHECK(test2 -> subst("x", new Num(5)) -> equals(
            new _let(new Variable("x"),
                     new Add(new Num(5), new Num(1)),
                     new Add(new Variable("x"), new Num(1)))));
    // test3 =
    //          _let x = x + 1;
    //          _in x + 1;          -> subst ("y", 5)
    expr *test3 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(1)),
                           new Add(new Variable("x"), new Num(1)));
    CHECK(test3 -> subst("y", new Num(5)) -> equals(test3));
}

TEST_CASE("_let Equals"){
    // _let x = 5;
    // _in x + 1;
    _let *test = new _let(new Variable("x"), new Num(5), new Add(new Variable("x"), new Num(1)));
    CHECK(test-> equals(new _let(new Variable("x"), new Num(5), new Add(new Variable("x"), new Num(1)))) == true);
    CHECK(test-> equals(new _let(new Variable("y"), new Num(5), new Add(new Variable("y"), new Num(1)))) == false);
    CHECK(test-> equals(new Num(6)) == false);
}

TEST_CASE("_let Has Variable"){
    // test1 =
    //          _let x = 5;
    //          _in 6;
    expr *test1 = new _let(new Variable("x"), new Num(5), new Num(6));
    CHECK(test1 -> has_variable() == false);
    // test2 =
    //          _let x = x + 1;
    //          _in 6;
    expr *test2 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(1)),
                           new Num(6));
    CHECK(test2 -> has_variable() == true);
    // test3 =
    //          _let x = 5;
    //          _in x + 1;
    expr *test3 = new _let(new Variable("x"),
                           new Num(5),
                           new Add(new Variable("x"), new Num(1)));
    CHECK(test3 -> has_variable() == true);
    // test4 =
    //          _let x = x + 1;
    //          _in x + 1;
    expr *test4 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(1)),
                           new Add(new Variable("x"), new Num(1)));
    CHECK(test4 -> has_variable() == true);
}

TEST_CASE("_let Print"){
    // test1 =
    //        _let x = 5;
    //        _in x + 1;
    expr *test1 = new _let(new Variable("x"),
                           new Num(5),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test1 -> to_string()) == "(_let x=5 _in (x+1))");
    // test2 =
    //        _let x = 5;
    //        _in _let x = 3;
    //            _in x + 1;
    expr *test2 = new _let(new Variable("x"),
                           new Num(5),
                           new _let(new Variable("x"),
                                    new Num(3),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK((test2 -> to_string()) == "(_let x=5 _in (_let x=3 _in (x+1)))");
    // test3 =
    //        _let x = x + 1;
    //        _in x + 1;
    expr *test3 = new _let(new Variable("x"),
                           new Add(new Variable("x"), new Num(1)),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test3 -> to_string()) == "(_let x=(x+1) _in (x+1))");
}

TEST_CASE("_let Interp"){
    // test1 =
    //        _let x = 5;
    //        _in 6;
    expr *test1 = new _let(new Variable("x"), new Num(5), new Num(6));
    CHECK( test1 -> interp() == 6);
    // test2 =
    //        _let x = 5;
    //        _in x + 1;
    expr *test2 = new _let(new Variable("x"),
                           new Num(5),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test2 -> interp()) == 6);
    // test3 =
    //        _let x = 5;
    //        _in y + 1;
    expr *test3 = new _let(new Variable("x"),
                           new Num(5),
                           new Add(new Variable("y"), new Num(1)));
    CHECK_THROWS_WITH( test3 -> interp(),
                       "Variable has no value" );
    // test4 =
    //        _let x = 5;
    //        _in _let x = 3;
    //            _in x + 1;
    expr *test4 = new _let(new Variable("x"),
                           new Num(5),
                           new _let(new Variable("x"),
                                    new Num(3),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK(test4 -> interp() == 4);

    // test6 =
    //        _let x = 5;
    //        _in _let x = x + 1;
    //            _in x + 1;
    expr *test6 = new _let(new Variable("x"), new Num(5),
                           new _let(new Variable("x"),
                                    new Add(new Variable("x"), new Num(1)),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK((test6 -> interp()) == 7);

}

TEST_CASE("_let Pretty Print"){
    // test1 =
    //        _let x = 5
    //        _in x + 1
    expr *test1 = new _let(new Variable("x"),
                           new Num(5),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test1 -> to_string_p()) == "_let x = 5\n_in x + 1");
    // test2 =
    //        (_let x = 5
    //        _in x) + 1
    expr *test2 = new Add(new _let(new Variable("x"), new Num(5), new Variable("x")),
                          new Num(1));
    CHECK((test2 -> to_string_p()) == "(_let x = 5\n _in x) + 1");
    // test3 =
    //        5 * (_let x = 5
    //             _in  x) + 1
    expr *test3 = new Add (new Mult(new Num(5),
                                    new _let(new Variable("x"), new Num(5), new Variable("x"))),
                           new Num(1));
    CHECK((test3 -> to_string_p()) == "5 * (_let x = 5\n     _in x) + 1");
    // test4 =
    //        5 * _let x = 5
    //            _in  x + 1
    expr *test4 = new Mult(new Num(5),
                           new _let(new Variable("x"),
                                    new Num(5),
                                    new Add(new Variable("x"), new Num(1))));
//    CHECK((test4 -> to_string_p()) == "5 * _let x = 5\n    _in x + 1");
//    // test5 =
//    //        _let x = 5
//    //        _in _let x = 3
//    //            _in x + 1
    expr *test5 = new _let(new Variable("x"),
                           new Num(5),
                           new _let(new Variable("x"), new Num(3),
                                    new Add(new Variable("x"), new Num(1))));
    CHECK((test5 -> to_string_p()) == "_let x = 5\n_in _let x = 3\n    _in x + 1");
    // test6 =
    //        _let x = _let x = 3
    //                 _in x + 1
    //        _in x + 1
    expr *test6 = new _let(new Variable("x"),
                           new _let(new Variable("x"), new Num(3),
                                    new Add(new Variable("x"), new Num(1))),
                           new Add(new Variable("x"), new Num(1)));
    CHECK((test6 -> to_string_p()) == "_let x = _let x = 3\n         _in x + 1\n_in x + 1");
    // test7 =
    //       (5 + (_let x = _let x = 1
    //                      _in x + 2
    //             _in x + 3)) * 4
    expr *test7 = new Mult(new Add(new Num(5),
                                   new _let(new Variable("x"),
                                            new _let(new Variable("x"),
                                                     new Num(1),
                                                     new Add(new Variable("x"), new Num(2))),
                                            new Add(new Variable("x"), new Num(3)))),
                           new Num(4));
    CHECK((test7 -> to_string_p()) == "(5 + (_let x = _let x = 1\n               _in x + 2\n      _in x + 3)) * 4");
}

