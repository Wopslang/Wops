<!--
   CONTRIBUTING.md
   Wopslang Contribute guide

   2023, Wops Team

-->

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
- [ ] K&R Indentation Style
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

All commit messages should satisfy these checklist.

- [ ] Description Starts with Lowercase Character
- [ ] No Emoji
- [ ] Use Boilerplate Code :

```text
<file name/path>: <description>
```

Also, your PR can be rejected by these reasons:

- Too much change
- No bug check(or performance check)
- Doesn't match with our plan

Finally, your PR branch's name should be in one of these forms:

- `patch-[username]-[number]`
- _`dev-[username]-[number]`: only for inside Wopers_
- `docs-[username]-[number]`

### :speech_balloon: Being the Wopslang Manager

Wopslang managers mediate the overheated community, review the issues and pull requests, and lead the development of Wopslang. Of course, this job requires not only deep knowledge of Wopslang, but also conversation skill. Also, Wopslang managers should be in Wops team; Wopslang managers should be inside Wopers.

We're not recruiting the Wopslang manager for now.

#### Wopslang manager list

| Github Username | Slack Username |
| --------------- | -------------- |
| @RedoC-github   | @RedoC-github  |
| @rdxxer         | @codenstory    |
| @pokmui         | @pokmui        |

2023, Wops Team
