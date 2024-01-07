<!--
   doc/devdoc.md
   Official Wopslang Documentation for Contributors

   2024, Wops Team
-->

# :hammer_and_wrench: Official Wopslang Documentation for Contributors

## Index
<details>
<summary>Click this to unfold the index</summary>

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
 │   ├── parse.cpp : GetTokenTable(F), Parse(F), ParseExpr(F)
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

#### *enum* op_

#### *enum* StmtType

#### *enum* TYPE

### Functions

#### *Object* EExecFunc(std::string func, Object argv)

#### *std::pair<int, int>* Parse(AST& head, std::vector<std::vector<String>> Token_table, int parse_start, int arg_idx, std::vector<String>& token_storage)

#### *Expr* ParseExpr(std::vector<std::string> tokens, int parsing_line)

#### *std::vector<String>* GetTokenTable(String code)

### Variables

#### `*const std::string* type_strname[]`


[Introduction]: #introduction
[How Does Wopslang Interpreter Work?]: #how-does-wopslang-interpreter-work
[Structure of Source Codes]: #structure-of-source-codes
[What Should You Learn to Contribute?]: #what-should-you-learn-to-contribute