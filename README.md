## MSDScript Description

The MSDScript is a programming language interpreter implemented by C++. It has some basic features like JavaScript and we can execute it through command line. Below are some features:

Addition
Multiplication
Comparison
Boolean
Human language parsing
Variable bind
If/else conditions
Function execution

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
Variables' input rule like other languages as C++, variables are valid for any combinations of character but invalid for starting of digit or _.<br>
Examples:<br>
Valid: `x1` or `abc` or `string_`.<br>
Invalid: `1x` or `_x`.<br>

#### Booleans
Booleans is represented as  `_true` or `false`.

#### Addition and Multiplication
Addition and Multiplication are represented as  `+` and `*`.

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
The left part as `()`should be variables and follwing will be the function body.<br>
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

## MSDScript Expressions and Grammer 
The Expressions mean all the features that MSDScript have.<br>
Grammer means parsing with conditions and comparisons.<br>
Expressions:<br>
```
<expr> = <number>
       | <boolean>
       | <expr> == <expr>
       | <expr> + <expr>
       | <expr> * <expr>
       | <variable>
       | _let <variable> = <expr> _in <expr>
       | _if <expr> _then <expr> _else <expr>
       
<Val> = <number>
      | <boolean>
      | <function>     
```
Grammer:<br>
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








