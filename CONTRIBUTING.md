# :page_with_curl: Contribute Guideline

## :wave: Welcome, new Wopers  

> :bulb: Tip: We call contributers as Woper

First off, thank you for considering contributing to Wopslang. Wopslang could be existed because of many devoted Wopers like you.

## :hammer_and_wrench: How to contribute

### :heavy_exclamation_mark: Using Issues

Report performance errors on code (memory leak, infinity loop, etc).  
There isn't any issue template, but please include these two parts.

- Where  
- Describe error  

If you already have a solution for it, please make a PR.

### :fork_and_knife: Using PR

If you already have a solution for it, PR suits you.  
Code style should satisfy these checklist.

- [ ] [Google C++ Coding Style Guide #General Naming Rules](https://google.github.io/styleguide/cppguide.html#General_Naming_Rules)  
- [ ] K&R Indentation style  
- [ ] Boilerplate Code  

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

Also, your branch's name should be like
`patch-[username]-[number]`.

## :white_check_mark: TODO

**Contributes about things in TODO list will have higher priority than normal one.**  

- Parser  

2021, Wops Team
