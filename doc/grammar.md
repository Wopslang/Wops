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
  - [Boolean Literal]
- [Types]
- [Variables]
- [Expressions]
  - [Blocks]
  - [Declarations]
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
identifiers = letter { letter | uni_digit } .
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

> In version v0.1, only decimal is allowed to use.

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

### Boolean Literal

boolean literal is a bit representing boolean constant: *true, and false*.

```ebnf
bool_lit = "0" |  "1".
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

## Types

A data type or simply type is an attribute which notate a particular kind of data item.  
There are four kind of type: *Integer, Floating-Point, String, and Boolean*. And it can be notated with these [predeclared identifiers][Predeclared Identifiers]:

|Identifier|Matching Type|Description|
|---|---|--|
|int|Integer|signed 32-bit integers (-2147483648 ~ 2147483647)|
|double|Floating-Point|IEEE-754 64-bit floating-point numbers|
|string|String|[same with string literal][String Literal]|
|bool|Boolean|[same with boolean literal][Boolean Literal]|

```ebnf
Type = "int" | "double" | "string" | "bool" .
```

## Variables

A variable is a storage for holding a value. The set of available values is determined by the variable's [type][Types].
There are two types of variable which can be used in Wopslang v0.1: *constant variable, modifiable variable*. You can declare constant variable as adding const keyword in variable declare expression. Also, you should add initial value to declare constant variable and you won't be able to reassign its value. Interpreter can emit the error if value's representing type doesn't match with variable's type.

```go
int woo = toint(in())
int ooo = -1

string poo = "result: " + tostring(woo + ooo)
string soo = poo
```

## Expressions

### Blocks

A block is an empty sequence of declarations and statements within matching brace({,})s.

```ebnf
Block = "{" Statements "}" .
Statements = { Statement } .
```

if and for expression is considered to be in its own block.

### Declarations

A declaration bind *identifiers* and *value* to a constant or [variable][Variables].
Every identifier in a program must be declared, and no identifier may be declared twice in the same block. See [Variables] to get more information.

```ebnf
Declaration = ConstDel | VarDel .
ConstDel = "const" Type identifiers "=" Expression .
VarDel = Type identifiers [ "=" Expression ] .
```

To find syntax defination of `Expression`, see [Operators] for more.

### If Expression

### For Expression

## Statesments

## Builtin Functions

## Error Handling

<!-- Link -->

[Introduction]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#introduction
[Notation]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#notation
[Source Code Representation]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#source-code-representation
[Lexical Elements]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#lexical-elements
[Types]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#types
[Variables]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#variables
[Expressions]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#expressions
[Statesments]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#statesments
[if]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#if
[for]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#for
[Builtin functions]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#builtin-functions
[Error Handling]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#error-handling
[Tokens]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#tokens
[Predeclared Identifiers]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#predeclared-identifiers
[UTF-8]: https://en.wikipedia.org/wiki/UTF-8
[Characters]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#characters
[Letter and Digits]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#letter-and-digits
[Comments]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#comments
[Semicolons]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#semicolons
[Identifiers]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#identifiers
[Keywords]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#keywords
[Operators and Punctuation]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#operators-and-punctuation
[Integer Literal]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#integer-literal
[Floating-point Literal]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#floating-point-literal
[Rune Literal]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#rune-literal
[String Literal]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#string-literal
[Boolean Literal]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#boolean-literal
[Blocks]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#blocks
[Declarations]: https://github.com/Wopslang/Wops/blob/main/doc/grammar.md#declarations
