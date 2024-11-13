<!--
   doc/grammar-rough.md
   Wopslang Language Rough Reference for v1

   2024, Wops Team
-->

# Wops Rough Grammar Note

### Based on Category Theory
- Haskell 
- https://github.com/sudopark/category-theory-for-programmers-study
- We're to make a **useful** Haskell!
- Lazy Evaluation -> Infinity
- **Restarting our programming language?**

### Types
- Element
- Object (**Set**(Set), **Ord**(Ordered Set), **Grp**(Group), **Vect**(Vector Space), **Grph**(Graph))
    - Each object must include its operations
    - Morphism(functions), Functor?
    - 3rd party Object

### Basic Grammar

#### I/O
```js
ask a, b, c
say "I", "love", "Wopslang"
```
#### Variable Definition & Assignment

Very important **`as`** keyword! Use `{}` to express the construction.
```py
a := {1, 2, 3} as Set
b := 1 as N
b := -3 as N # Assignement
c := 1 as R
d := {1->3, 2->3} as Grph
e := {1,2} as Ord
f := {1,2} as CCInterval # For uniformity. Consider giving exception on interval to use [1,2], or giving more freedom to developers
f := (1,2) as OCInterval 
```

Definition of object: must indicate how it'll interpret the structure.

#### Operators (included in object definition)
    - For example, N : `+`, `-`, `*`, `/`(?: not closed), `^`(exponential), `xor`, ...

#### Loops
```py
for (h:= {2,n+1,1} as Range) => (

)

for (i in {1,2} as CCInterval) => (

)

if (i >= 3) => (

)
```

#### Functions
```js
f:{<param>}->{<return>} => (

)
```

### Block vs Expression
- `{}`: for Expressions
- `()`: for Blocks


### INF

Let's make new Types to introduce the concept of infinity. Creating an infinitely big number is impossible, so we define a new type INF.

Infinity
: bigger than any numbers
```js
x := Infty as INF
```

Infinitesimal
bigger than zero, smaller than any positive numbers (= 1/inf)
```js
x := Inftysmall as INF
```


