## MSDScript Description

The MSDScript is a programming language interpreter implemented by C++. It has some basic features like JavaScript and we can execute it through command line. 

### Features:

Addition
Multiplication
Comparison
Boolean
Human language parsing
Variable bind
If/else conditions
Function execution

### Grammer:
```
<expr> = <comparg>
		| <comparg> == <expr>

<comparg> = <addend>
		| <addend> + <comparg>

<addend> = <multicand>
		| <multicand> * <addend>

<multicand> = <inner>
		| <multicand> ( <expr> )

<inner> = <number>
		| ( <expr> )
		| <variable>
		| _let <variable> = <expr> _in <expr>
		| _true
		| _false
		| _if <expr> _then <expr> _else <expr>
		| _fun ( <variable> ) <expr>
```
The precedence of the operators is `==`, `+`, `*`, `_bool`,`if/else`,`function`. (from low precedence to high).

## Build and Installation Instructions

By the command line:
Drect to the msdscript file which contains the makefile, then run `make` to compile the file.<br>
After, we can get a out file -> `msdscript` we can use command like `./msdscript --operation` to run our msdsrcipt coomand.

Operations list:<br>
`./msdscript --help` : get the manual of MSDScript<br>
`./msdscript --test` : execute the test part for the debug pattern<br>
`./msdscript --interp` : excute the MSDScript function<br>
`./msdscript --print` : parse human language and print out the valid MSDScript experssion<br>


## User Manuals

MSDScript can be run via the command line. 
How to execute: <br>
First use the `./msdscript --interp` command to start interpreting. <br>
Then We need to use `Ctrl+D` to begin the execute. <br>

### Keywords
#### Numbers
Numbers experssion should not contain commas, decimals or spaces between characters and numbers.<br>
Examples:<br>
Valid: `1+2` or `1 + 2` or `1    +      2`.<br>
Invalid: `2,000 + 7.5` or `4 000 + 10`.<br>

#### Variables
Variables' input rule like other languages as C++, variables are valid for any combinations of character between 'a - z' and 'A - Z' but invalid for starting of digit or _.<br>
Examples:<br>
Valid: `x1` or `abc` or `string`.<br>
Invalid: `1x` or `_x`.<br>

#### Booleans
Booleans is represented as  `_true` or `_false`.

#### Addition and Multiplication
Addition and Multiplication are represented as  `+` and `*`.<br>
****Note**** : the subexpressions of `+` and `*` must produce numbers like `1 + 2` or `1 * 2` instead of `x + 1` or `x * 1`.<br>
While the Comparison as `==` can compare variable and number and return true or false.
In the MSDScript we use `EqualExpr` expressions to represent `==`. We will talk about in the API part.

#### Comparison
Comparison is represented as  `==`.

#### Variable definition
The `_let` keyword assign value to a variable.<br>
This keyword should be followed by an variable at the left and the assigned value at the right.<br>
Examples:<br>
`_let x = 5
_in  (_let y = 3
      _in  y + 2) + x`<br>
`(_let x = 5
 _in  x) + 1`<br>
 **Note** Parentheses should be put around the smallest expression needed to resolve an ambiguity.
 
 #### If/else conditions
 This expersstion is similar to ternary operator in Java. It has 3 parts:<br>
 1. Evaluation Section: if true, execute step 2, or step 3. <br>
 2. then true branch. <br>
 3. else false branch.<br>
 They are comprised of the `_if`, `_then`, and `_else` keywords<br>
 Format:<br>
```
  _if //this 
 _then //do this 
 _else //do that
```
#### Functions and calls 
Functions are represented by the `_fun` keyword.<br>
Like JavaScript, everything is derived from the prototypes, the MSDScript expressions are derived from one class, so the function is same type as value and number.<br>
The left part as `()`should be variables and follwing will be the function body.<br>
The first subexpression of a function-call must have a function value so the definiltion like `_fun() x + 1` is invalid.<br>
Examples:<br>
`_fun (x) x + 1` creates function that add 1 to value x.<br>

To execute a function, you should pass the value into the function after the close parentheses of `()`.<br>
Examples:<br>
`(_fun (x) x + 1) (2)` will execute the answer 3.

## MSDScript API

### MSD files

#### Core
The core files support the running of msdscript: <br> 

`env.cpp and env.hpp`: Allow for quicker replacing of variable values.<br>
`expr.cpp and expr.hpp`: Expression files.<br>
`value.cpp and value.hpp`: Values used for function call.<br>

#### Helpers
The helper files help configure the msdscript and build user interface.<br>
`pointer.h`: MSDScript without shared pointers may result in memory leak so we use this file to configure the shared pointer.<br>
`main.cpp`: This file used to execute msd command like `./msdscript --interp`.<br>
`parse.cpp` and `parse.hpp`: Used to parse human language to msdscript readable experssion.

#### Testing

`Catch.cpp`: This framework will execute when doing testing. 

## MSDScript Expressions
The Expressions mean all the features that MSDScript have.<br>
Grammer means parsing with conditions and comparisons.<br>
Expressions:<br>
```
<expr> = <NumExpr>
       | <BoolExpr>
       | <expr> == <expr> (EqualExpr)
       | <expr> + <expr> (AddExpr)
       | <expr> * <expr> (MultExpr)
       | <VarExpr>
       | _let <variable> = <expr> _in <expr> (LetExpr)
       | _if <expr> _then <expr> _else <expr> (IfExpr)
       | _fun(<VarExpr>)<expr> (FunExpr)
       | <expr>(<expr>)
       
<Val> = <number>
      | <boolean>
      | _fun(〈variable〉)〈expr〉    
```
### expr
expr is the basic class of MSDScipt, all the expressions are derived(or inherited)from this class.
That is why the function, value, and number expressions can be treated as the same type of 'variable'. Not like C++, we can operate functions like numbers!<br>
The types or the subclass of expr is shown before.

### Parse
When you run MSDScript in the interp pattern, the MSDScript will implement the `parse` class to formate input and translate it to MSDScript expressions which can be executed by the Script.<br>
Below are some key method of wrapping user's input:<br>
`PTR(expr)parse(std::istream &in) ` Extract the input stream, parses it, and returns the entire input as an expr. <br>
Like the grammer shown before, MSDScipt will parse the input like 'recursion', so the method 'linkedlist' is: <br>
```
`PTR(expr)parse_expr(std::istream &in)`  // parse '==' 
		|	
`PTR(expr)parse_comparg(std::istream &in)` // parse '+'
		|
`PTR(expr)parse_addend(std::istream &in)` // parse '*'
		|
`PTR(expr)parse_multicand(std::istream &in)` // parse 'inner function' like _let, _boolean and _fun
		|
`PTR(expr)parse_inner(std::istream &in)`
```
For the inner part, MSDSCript implement the help method like `PTR(expr)parse_if(std::istream &in)` and `PTR(expr)parse_fun(std::istream &in)` to help parse the expression.<br>
The return type of parse is always an expr.

#### Interpreting Expressions
`interp` is to find the value of an expression and this method return a new class Val which can be convert to a string so the MSDScript will response after calling function or expersions.<br>
The implement of parse and interp is 
```
PTR(expr) e = parse(std::cin);
cout<<e->interp(NEW(EmptyEnv)())->to_string()<<endl;
```
Below is example: <br>
<img width="346" alt="image" src="https://user-images.githubusercontent.com/89375316/165440058-06f52d10-6745-455d-9a6a-7b4a3ce5732a.png">








