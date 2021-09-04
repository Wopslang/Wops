# :page_with_curl: Contribute Guideline

## :wave: Welcome, new Wopers  

First off, thank you for considering contributing to Wopslang. Wopslang could be existed because of many devoted Wopers like you.

## :hammer_and_wrench: How to contribute

### :heavy_exclamation_mark: Using Issues

Report performance errors on code (memory leak, infinity loop, etc).  
There isn't any issue template, but your issue must have these three parts.

- How to reproduce
- Error Message
- Your code(if necessary)

If you already have a solution for it, please make a PR.

### :fork_and_knife: Using PR

If you already have a solution for it, PR suits you.  
Code style should satisfy these checklist.

- [ ] [Google C++ Coding Style Guide #General Naming Rules](https://google.github.io/styleguide/cppguide.html#General_Naming_Rules)  
- [ ] K&R Indentation style  
- [ ] Use Boilerplate Code :  

```c++
/*
 * <file path> 
 * <file description><Headerfile/Sourcefile>
 *
 * <year>, Wops Team
 * */

// if your code is header
#ifndef WOPS_[filename_CAPITAL]_H
#define WOPS_[filename_CAPITAL]_H

/* your code */

#endif
```

Also, your PR can be rejected by these reasons:

- Too much change
- No bug check(or performance check)
- Doesn't match with our plan

Finally, your PR branch's name should be in one of these forms:

- `patch-[username]-[number]`
- *`dev-[username]-[number]`: only for inside Wopers*
- `docs-[username]-[number]`

### :speech_balloon: Being the Wopslang Manager

Wopslang managers mediate the overheated community, review the issues and pull requests, and lead the development of Wopslang. Of course, this job requires not only deep knowledge of Wopslang, but also conversation skill. Also, Wopslang managers should be in Wops team; Wopslang managers should be inside Wopers.  

If you want to contribute as the Wopslang manager, join the [Wops Community Slack](https://join.slack.com/t/wopslangcommunity/shared_invite/zt-nkcy12cy-n8YlAPnOT~ErPODF6k3jOw) and send DM to Wopslang managers. We'll prepare the interview for you.

#### Wopslang manager list

|Github Username|Slack Username|
|---|---|
|@RedoC-github|@RedoC-github|
|@codenstory|@codenstory|
|@pokmui|@pokmui|

## :white_check_mark: TODO

**Contributes about things in TODO list will have higher priority than normal one.**  

- Wopslang Runtime System Test

## :computer: Wopslang Runtime System Test

> Note: this section will be deleted after developing parser

Follow these instruction to execute Wopslang Runtime System.

1. open src/runtime/AST-test.cpp
2. clear every code inside main function

3. declare ASTs (there must be main AST)

4. declare every tokens (in this code, some token expression nodes were reused, but we suggest you not to reuse tokens)

     - See the defination of Expr for more detail
5. connect them

     - Connection Order: expression-expression, connected expression-AST, AST-AST

6. declare empty variable storage and execute main AST

If you wonder how to connect or declare AST or ExprAST, see the defination(src/runtime/AST.h). You can also test Runtime System Test by sending us your Wopslang code. Please make new discussion(category: Q&A) and attach your wopslang code. We'll code the runtime system testing C++ code and send it to you. Copy and paste the testing code into AST-test.cpp and build it. **If your wopslang code catched some bugs, we won't send you the testing code.**

2021, Wops Team
