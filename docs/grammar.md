<style>
  body {
    font-family:-apple-system, BlinkMacSystemFont, Segoe UI, Helvetica, Arial, sans-serif, Apple Color Emoji, Segoe UI Emoji;
    font-weight: 400;
  }

  .main-content {
    font-weight: 600;
  }

  h1 {
    font-weight: 600;
  }
  h2 {
    font-weight: 600;
  }
  h3 {
    font-weight: 600;
  }
</style>

# Official Wopslang Language Reference `v0.1`

## Index

- [Introduction]
- [Notation]
- [Source Code Representation]
  - [Characters]
  - [Letter and Digits]
- [Lexical Elements]
  - [Comments]
  - [Tokens]
  - [Semicolons]
  - [Identifiers]
  - [Keywords]
  - [Operators and Punctuation]
  - [Integer Literal]
  - [Floating-point Literal]
  - [Rune Literal]
  - [String Literal]
- [Types]
- [Variables]
- [Expressions]
- [Statesments]
  - [if]
  - [for]
- [Builtin functions]
- [Error Handling]

## Introduction

It is the official Wopslang language reference for v0.1 alpha.
Because this version is a test version, so lots of stuff in here can be changed.
Of course, lots of new grammar can be added to Wopslang.
So, make sure to check this page when you updated Wopslang to the newest version.

> Our language is based on an idea: simple, but powerful.
> And because our grammar was also designed with this idea, we developed only the most important expressions.
> However, we know that our standard of *importance* can be different from your standard.
> So, please tell us your idea is good to put into Wopslang.
> We love to hear your voice.

## Notation

The syntax is specified using Extended Backus-Naur Form (EBNF).

## Source Code Representation

 Source code is Unicode text encoded in [UTF-8].

### Characters

```ebnf
newline    = /* U+000A */ .
uni_char   = /* an arbitrary Unicode code point except newline */ .
uni_letter = /* a Unicode code point classified as Letter */ .
uni_digit  = /* a Unicode code point classified as Number, decimal digit /* .
```

### Letter and digits

```ebnf
type      = "int" | "double" | "bool" | "string" .
letter    = uni_letter | "_" .
digit     = "0" ... "9" .
```

## Lexical Elements

### Comments

Comments can be represented with a form:  

- Full-line comments start with the character sequence // and stop at the end of the line.

For example, this form will be allowed:

```go
// Good :D
out(":D")
```

And these forms won't be allowed:

```go
/*
Bad :(
*/
out(":(")  // Bad :(
```

### Tokens

Token is the word which can be used in Wopslang. There are four types: *Identifiers, Keywords, Operators, Punctuation, and Literals*. White space, formed from spaces (U+0020), horizontal tabs (U+0009), carriage returns (U+000D), and newlines (U+000A), is ignored except as it separates tokens that would otherwise combine into a single token. Also, the semicolon can split tokens as triggering newline or EOF(End of File).

### Semicolons

A newline or end of file may be trigger by the insertion of a semicolon(`;`). See [Tokens] to get information about breaking tokens.

### Identifiers

Identifiers name program entities such as variables and types. An identifier is a sequence of one or more letters and digits. The first character in an identifier must be a letter.

```ebnf
identifiers ::= letter { letter | uni_digit } .
```

Example:

```go
abc
_WopsV01
옵스랭
αβ
```

Some identifiers cannot be used. See [Predeclared Identifiers] for more detail.

### Keywords

The following keywords are reserved and may not be used as identifiers.

> We only put keywords which work on v0.1 alpha, so it can be updated.

```text
break        const        continue
elif         else         for
if           range        return
```

### Operators and Punctuation

The following character sequences represent operators (including assignment operators) and punctuation.

> We only put operators and punctuation which work on v0.1 alpha, so it can be updated.

```text
+    &&    ==    !=    (    )
-    ||    <     <=    [    ]
*    >     >=    /     {    }
     =     ;     %     !            
```

### Integer Literal

An integer literal is a sequence of digits representing an integer constant.

> In version v0.1, only decimal can be allowed.

```ebnf
integer_lit    = "0" | ( "1" … "9" ) [ decimal_digits ] .
decimal_digits = { digit } .
```

Example:

```go
42
197
0
32589
1

01 // (x)
34-5 // (x)
a56bc // (x)
```

### Floating-point Literal

A floating-point literal is a decimal representation of a floating-point constant. A decimal floating-point literal consists of an integer part (decimal digits), a decimal point, a fractional part (decimal digits).

```ebnf
float_lit = integer_lit | integer_lit "." decimal_digits
```

Example:

```go
0.7
3.141592653589793238
0.0
0.08

.3 // (x)
11. // (x)
```

### Rune Literal

A rune literal represents a rune constant, an integer value identifying a Unicode code point. A rune literal is expressed as one or more characters such as `\n`, `\"`.

After a backslash, certain single-character escapes represent special values:

|Literal|Unicode|Description|
|---|---|--|
|`\a`|U+0007|alert or bell|
|`\b`|U+0008|backspace|
|`\f`|U+000C|form feed|
|`\n`|U+000A|line feed or newline|
|`\r`|U+0007|carriage return|
|`\t`|U+0007|horizontal tab|
|`\v`|U+0007|vertical tab|
|`\\`|U+0007|backslash|
|`\'`|U+0007|single quote|
|`\"`|U+0007|double quote|

```ebnf
rune_lit      = "'" uni_value "'" .
uni_value     = uni_char | escaped_char .
escaped_char  = `\` ( "a" | "b" | "f" | "n" | "r" | "t" | "v" | `\` | "'" | `"` ) .
```

### String Literal

A string literal represents a string constant obtained from concatenating a sequence of characters.

```ebnf
string_lit = `"` { unicode_value } `"` .
```

Example:

```go
"abc"
"Hello, World!\n"
"Wopslang is 옵스랭"
"🤖"
"\"" // Same with '"'
```

> 👷‍♂️ We're on working now. Stay tuned.

## Types

## Variables

## Expressions

### If Expression

### For Expression

## Statesments

## Builtin Functions

## Error Handling

<!-- Link -->

[Introduction]: https://wopslang.github.io/Wops/grammar.html#introduction
[Notation]: https://wopslang.github.io/Wops/grammar.html#notation
[Source Code Representation]: https://wopslang.github.io/Wops/grammar.html#source-code-representation
[Lexical Elements]: https://wopslang.github.io/Wops/grammar.html#lexical-elements
[Types]: https://wopslang.github.io/Wops/grammar.html#types
[Variables]: https://wopslang.github.io/Wops/grammar.html#variables
[Expressions]: https://wopslang.github.io/Wops/grammar.html#expressions
[Statesments]: https://wopslang.github.io/Wops/grammar.html#statesments
[if]: https://wopslang.github.io/Wops/grammar.html#if
[for]: https://wopslang.github.io/Wops/grammar.html#for
[Builtin functions]: https://wopslang.github.io/Wops/grammar.html#builtin-functions
[Error Handling]: https://wopslang.github.io/Wops/grammar.html#error-handling
[Tokens]: https://wopslang.github.io/Wops/grammar.html#tokens
[Predeclared Identifiers]: https://wopslang.github.io/Wops/grammar.html#predeclared-identifiers
[UTF-8]: https://en.wikipedia.org/wiki/UTF-8
[Characters]: https://wopslang.github.io/Wops/grammar.html#characters
[Letter and Digits]: https://wopslang.github.io/Wops/grammar.html#letter-and-digits
[Comments]: https://wopslang.github.io/Wops/grammar.html#comments
[Semicolons]: https://wopslang.github.io/Wops/grammar.html#semicolons
[Identifiers]: https://wopslang.github.io/Wops/grammar.html#identifiers
[Keywords]: https://wopslang.github.io/Wops/grammar.html#keywords
[Operators and Punctuation]: https://wopslang.github.io/Wops/grammar.html#operators-and-punctuation
[Integer Literal]: https://wopslang.github.io/Wops/grammar.html#integer-literal
[Floating-point Literal]: https://wopslang.github.io/Wops/grammar.html#floating-point-literal
[Rune Literal]: https://wopslang.github.io/Wops/grammar.html#rune-lineral
[String Literal]: https://wopslang.github.io/Wops/grammar.html#string-lineral