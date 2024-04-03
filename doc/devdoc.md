<!--
   doc/devdoc.md
   Official Wopslang Documentation for Contributors

   2024, Wops Team
-->

# :hammer_and_wrench: Official Wopslang Documentation for Contributors

## Index
<details>
<summary>Index</summary>

- [Introduction]
    - [How Does Wopslang Interpreter Work?]
    - [Structure of Source Codes]
    - [What Should You Learn to Contribute?]
- [References]
    - [Source Codes]
      - [interpreter.cpp]
      - [import_/eexec_.cpp]
      - [parser/parse.cpp]
    - [Header Files]
      - [error/signal.h]
      - [import_/eexec_.h]
      - [parser/parse.h]
      - [runtime/AST.h]
      - [type/object.h]
      - [type/operator.h]
      - [type/variable.h]
    - [Classes]
      - [AST]
      - [ErrHandler] 
      - [Expr]
      - [Variable]
    - [Enums]
      - [Err]
      - [op_]
      - [StmtType]
      - [TYPE]
    - [Functions]
      - [EExecFunc]
      - [GetTokenTable]
      - [Parse]
      - [ParseExpr]
    - [Variables]
      - [funcs]
      - [operators]
      - [oprs]
      - [runes]
      - [type_strname]
</details>

## Introduction
### How Does Wopslang Interpreter Work?
Here is a basic diagram including the overall process of the interpreter.

```cpp
.wops file  →  interpreter.cpp  →  parse.h: GetTokenTable()
                                             ↓
                                        token_table
                                             ↓
                                      parse.h: Parse()
                                             ↓
                                            AST
                                             ↓
 signal.h: ErrHandler.CallErr()  ←  AST.h: AST.Execute()
```

After reading given `wops file`, `interpreter.cpp` makes `token_table` by calling `GetTokenTable()`. 
After that, it makes the abstract syntax tree of given code using `Parse()`.
At last, the `AST` is executed by `AST.Execute()`.
If there is any code error, it is handled by `ErrHandler.CallErr()`.

### Structure of Source Codes
Here is the structure of the *source codes, functions, enums, variables, and classes* consisting of the interpreter.

```tree
src
 ├── error
 │   └── signal.h : Err(E), ErrHandler(C)
 ├── import_
 │   ├── eexec_.cpp : EExecFunc(F)
 │   └── eexec_.h : EExecFunc(F)
 ├── interpreter.cpp : main(F)
 ├── parser
 │   ├── parse.cpp : funcs(V), GetTokenTable(F), operators(V), oprs(V), Parse(F), ParseExpr(F), runes(V)
 │   └── parse.h : GetTokenTable(F), Parse(F), ParseExpr(F)
 ├── runtime
 │   └── AST.h : AST(C), Expr(C), StmtType(E)
 └── type
     ├── object.h: Object(C)
     ├── operator.h: op_(E)
     └── variable.h: TYPE(E), type_strname(V), Variable(C)
```
> F: Function, E: Enum, V: Variable, C: Class

### What Should You Learn to Contribute?

## References

### Source Codes

#### interpreter.cpp

#### import_/eexec.cpp

#### parser/parse.cpp

### Header Files

#### error/signal.h

#### import_/eexec_.h

#### parser/parse.h

#### runtime/AST.h

#### type/object.h

#### type/operator.h

#### type/variable.h

### Classes

#### *class* AST

#### *class* ErrHandler

#### *class* Expr

#### *class* Variable

### Enums

#### *enum* Err

```cpp
enum Err { ... };
```
> - Declaration: [type/variable.h]

<details><summary>elements</summary>

- [0] : `OK`
- [1] : `ERROR`
- [2] : `INTERPRETER_CANNOT_OPEN_FILE`
- [3] : `BLANK_VARIABLE_NAME`
- [4] : `BLANK_OPERAND`
- [5] : `BLANK_PARAMETER`
- [6] : `NO_IDENTIFIER_AFTER_OPERATOR`
- [7] : `NO_OPERATION_MATCHING_TYPE`
- [8] : `NO_OPERATION_MATCHING_TYPE_UNARY`
- [9] : `NO_MATCHING_UNARY_OPERATION_FORM`
- [10] : `NO_MATCHING_SYNTAX_EXPRESSION`
- [11] : `NO_MATCHING_SYNTAX_FOR`
- [12] : `NO_MATCHING_SYNTAX_IF`
- [13] : `NO_MATCHING_SYNTAX_ELIF`
- [14] : `NO_MATCHING_STRUCTURE_IF`
- [15] : `NO_MATCHING_SYNTAX_BREAK`
- [16] : `NO_MATCHING_SYNTAX_CONTINUE`
- [17] : `NOT_MATCHING_TYPE_WITH_DEF`
- [18] : `NOT_MATCHING_TYPE_WHEN_COMP`
- [19] : `NOT_CLOSED_BLOCK`
- [20] : `VARIABLE_HAS_NOT_DECLARED`
- [21] : `VARIABLE_REDECLARE`
- [22] : `ERROR_OCCURED_WHILE_CALLING_FUNCTION`
- [23] : `BREAK_CONTINUE_ONLY_ALLOWED_FOR`
- [24] : `ASSIGN_ON_CONSTANT`
- [25] : `ASSIGN_ON_UNKNOWN`
- [26] : `IF_NO_BOOLEAN_CONDITION`
- [27] : `ELIF_NO_BOOLEAN_CONDITION`
- [28] : `FOR_NO_BOOLEAN_CONDITION`
- [29] : `CANNOT_LOAD_LIBRARY`
- [30] : `CANNOT_LOAD_SYMBOL`
- [31] : `OBJECT_OVERFLOW`
- [32] : `OBJECT_NOT_MATCHING_DATA`
- [33] : `OBJECT_WRONG_DIMENSION`
- [34] : `TOO_HIGH_DIMENSION`
- [35] : `UNMATCHED_PARENTHESIS`
- [36] : `OPERATION_ONLY_BINARY`
- [37] : `DIVIDING_WITH_ZERO`
</details>

An enumeration of every handled error type.

#### *enum* op_

```cpp
enum op_ { ... };
```
> - Declaration: [type/operator.h]

<details><summary>Elements</summary>

- [0] : `PLU`
- [1] : `MIN`
- [2] : `MUL`
- [3] : `DIV`
- [4] : `MOD`
- [5] : `ASSIGN`
- [6] : `EQ`
- [7] : `NEQ`
- [8] : `LG`
- [9] : `SM`
- [10] : `LGE`
- [11] : `SME`
- [12] : `LNOT`
- [13] : `LAND`
- [14] : `LOR`

</details>
An enumeration of every operator in Wopslang.

#### *enum* StmtType

```cpp
enum StmtType { ... }
```
> - Declaration: [runtime/AST.h]

<details><summary>Elements</summary>

- [0] : `Main`
- [1] : `ConstDel`
- [2] : `VarDel`
- [3] : `Expression`
- [4] : `Assignment`
- [5] : `BreakStmt`
- [6] : `ContinueStmt`
- [7] : `IfStmt`
- [8] : `ElifStmt`
- [9] : `ElseStmt`
- [10] : `ForStmt`
- [11] : `BracketBlock`
</details>
An enumeration of every statement types in Wopslang.

#### *enum* TYPE

```cpp
enum TYPE { ... };
```
> - Declaration: [type/variable.h]

<details><summary>Elements</summary>

- [0] : `INT`
- [1] : `DOUBLE`
- [2] : `STRING`
- [3] : `BOOL`
- [4] : `OPERATOR`
</details>
An array which includes every type's name in string form.

### Functions

#### *Object* EExecFunc(func, argv)

#### *std::pair<int, int>* Parse(head, Token_table, parse_start, arg_idx, token_storage)

#### *Expr* ParseExpr(tokens, parsing_line)

#### *std::vector<String>* GetTokenTable(code)

### Variables

#### *const std::string* type_strname[]

```cpp
const std::string type_strname[] = {"INT", "DOUBLE", "STRING", "BOOL", "OPERATOR"};
```
> - Declaration: [type/variable.h]
> - Type: `const std::string`

An array which includes every type's name in string form.


[Introduction]: #introduction
[How Does Wopslang Interpreter Work?]: #how-does-wopslang-interpreter-work
[Structure of Source Codes]: #structure-of-source-codes
[What Should You Learn to Contribute?]: #what-should-you-learn-to-contribute
[References]: #references
[Source Codes]: https://github.com/Wopslang/Wops/blob/doc/doc/devdoc.md#source-codes
[interpreter.cpp]: https://github.com/Wopslang/Wops/blob/doc/doc/devdoc.md#interpretercpp
[import_/eexec_.cpp]: #import_eexeccpp
[parser/parse.cpp]: #parserparsecpp
[Header Files]: #header-files
[error/signal.h]: #errorsignalh
[import_/eexec_.h]: https://github.com/Wopslang/Wops/blob/doc/doc/devdoc.md#import_eexec_h
[parser/parse.h]: #parserparseh
[runtime/AST.h]: #runtimeasth
[type/object.h]: #typeobjecth
[type/operator.h]: #typeoperatorh
[type/variable.h]: #typevariableh
[Classes]: #classes
[AST]: #class-ast
[ErrHandler]: #class-errhandler
[Expr]: #class-expr
[Variable]: #class-variable
[Enums]: #enums
[Err]: #enum-err
[op_]: #enum-op_
[StmtType]: #enum-stmttype
[TYPE]: #enum-type
[Functions]: #functions
[EExecFunc]: #object-eexecfuncfunc-argv
[GetTokenTable]: #stdvector-gettokentablecode
[Parse]: #stdpairint-int-parsehead-token_table-parse_start-arg_idx-token_storage
[ParseExpr]: #expr-parseexprtokens-parsing_line
[Variables]: #variables
[type_strname]: #const-stdstring-type_strname