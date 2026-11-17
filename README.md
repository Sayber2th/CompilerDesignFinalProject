# Mini Compiler: Compiler and Interpreter for a Custom Language

A compiler and interpreter for a small C++-style programming language, written entirely in C++. It takes a source file, runs it through every classic compiler phase, and executes the result.

This project is also the subject of a published paper: V. Singh and O. Kulkarni, "Design and Implementation of a Mini Compiler with Three Address Code Generation, Optimization, and Interpreter-Based Execution," *International Journal of Innovative Research in Technology (IJIRT)*, vol. 13, no. 1, pp. 7394-7396, 2026. [Read the paper](https://ijirt.org/article?manuscript=205683)

## Pipeline

```
Source file -> Lexer -> Parser (AST) -> Semantic Analyzer -> TAC Generator -> TAC Optimizer -> TAC Interpreter -> Output
```

| Phase | Files | What it does |
|---|---|---|
| Lexical analysis | `src/lexer.h`, `src/lexer.cpp` | Converts source text into tokens |
| Syntax analysis | `src/parser.h`, `src/parser.cpp` | Builds an abstract syntax tree (AST) from the tokens |
| Semantic analysis | `src/semanticAnalyzer.h`, `src/semanticAnalyzer.cpp` | Builds the symbol table and checks declarations and types |
| Intermediate code generation | `src/tacGenerator.h`, `src/tacGenerator.cpp` | Generates Three-Address Code (TAC) from the AST |
| Optimization | `src/tacOptimizer.h`, `src/tacOptimizer.cpp` | Applies constant propagation, constant folding, and dead code elimination to the TAC |
| Execution | `src/tacInterpreter.h`, `src/tacInterpreter.cpp` | Runs the optimized TAC and produces the program's output |
| Entry point | `src/driver.h`, `src/driver.cpp` | Reads the source file and runs each phase in order |

The grammar of the language is documented in `docs/ContextFreeGrammar.txt`.

## The Language

The language looks like C++, with a `print` statement for output instead of `std::cout`.

- Data types: `int` and `string`
- Declarations, with or without an initial value: `int a = 10 + 5;`, `int c;`, `string d = "Hello";`
- Assignments: `c = b + 0;`
- Arithmetic expressions (`+`, `-`, `*`, `/`) and unary minus: `int y = -x;`
- Printing a variable: `print(d);`

## Build and Run

**Requirements:** Windows, and either Visual Studio or JetBrains Rider with the MSVC Build Tools ("Desktop development with C++" workload). The project uses the Windows API to set the console to UTF-8, so it is Windows-only.

1. Open `CompilerDesignFinalProject.sln` in Visual Studio or Rider.
2. Build the `Mini Compiler` project (Build > Build Solution). The executable is created at `x64\Debug\Mini Compiler.exe` in the repository root.
3. Run the executable from the repository root, passing the path of a source file.

PowerShell:

```
& "x64\Debug\Mini Compiler.exe" "tests\test.txt"
```

Command Prompt:

```
"x64\Debug\Mini Compiler.exe" "tests\test.txt"
```

The program takes exactly one argument: the path to the source file.

## Example

**Input program:**

```
int a = 10 + 5;
int b = a * 2;
int c;
int e = 19;
int f = e * 6;
string d = "Hello";
c = b + 0;

int x = 5 + 6;
int y = -x;

print(d);
print(f);
print(a);
print(b);
print(y);
```

The compiler prints the result of every phase before running the program. The token list and the abstract syntax tree are collapsed below; click to expand them.

<details>
<summary>Show the token list (81 tokens)</summary>

```
Number of tokens: 81

List of tokens:
1)Token: int | Type: keyword
2)Token: a | Type: identifier
3)Token: = | Type: equals
4)Token: 10 | Type: integer literal
5)Token: + | Type: plus
6)Token: 5 | Type: integer literal
7)Token: ; | Type: semicolon
8)Token: int | Type: keyword
9)Token: b | Type: identifier
10)Token: = | Type: equals
11)Token: a | Type: identifier
12)Token: * | Type: asterisk
13)Token: 2 | Type: integer literal
14)Token: ; | Type: semicolon
15)Token: int | Type: keyword
16)Token: c | Type: identifier
17)Token: ; | Type: semicolon
18)Token: int | Type: keyword
19)Token: e | Type: identifier
20)Token: = | Type: equals
21)Token: 19 | Type: integer literal
22)Token: ; | Type: semicolon
23)Token: int | Type: keyword
24)Token: f | Type: identifier
25)Token: = | Type: equals
26)Token: e | Type: identifier
27)Token: * | Type: asterisk
28)Token: 6 | Type: integer literal
29)Token: ; | Type: semicolon
30)Token: string | Type: keyword
31)Token: d | Type: identifier
32)Token: = | Type: equals
33)Token: " | Type: double quotes
34)Token: Hello | Type: string literal
35)Token: " | Type: double quotes
36)Token: ; | Type: semicolon
37)Token: c | Type: identifier
38)Token: = | Type: equals
39)Token: b | Type: identifier
40)Token: + | Type: plus
41)Token: 0 | Type: integer literal
42)Token: ; | Type: semicolon
43)Token: int | Type: keyword
44)Token: x | Type: identifier
45)Token: = | Type: equals
46)Token: 5 | Type: integer literal
47)Token: + | Type: plus
48)Token: 6 | Type: integer literal
49)Token: ; | Type: semicolon
50)Token: int | Type: keyword
51)Token: y | Type: identifier
52)Token: = | Type: equals
53)Token: - | Type: minus
54)Token: x | Type: identifier
55)Token: ; | Type: semicolon
56)Token: print | Type: keyword
57)Token: ( | Type: left parenthesis
58)Token: d | Type: identifier
59)Token: ) | Type: right parenthesis
60)Token: ; | Type: semicolon
61)Token: print | Type: keyword
62)Token: ( | Type: left parenthesis
63)Token: f | Type: identifier
64)Token: ) | Type: right parenthesis
65)Token: ; | Type: semicolon
66)Token: print | Type: keyword
67)Token: ( | Type: left parenthesis
68)Token: a | Type: identifier
69)Token: ) | Type: right parenthesis
70)Token: ; | Type: semicolon
71)Token: print | Type: keyword
72)Token: ( | Type: left parenthesis
73)Token: b | Type: identifier
74)Token: ) | Type: right parenthesis
75)Token: ; | Type: semicolon
76)Token: print | Type: keyword
77)Token: ( | Type: left parenthesis
78)Token: y | Type: identifier
79)Token: ) | Type: right parenthesis
80)Token: ; | Type: semicolon
81)Token:  | Type: end of file token
```

</details>

<details>
<summary>Show the abstract syntax tree and symbol table</summary>

```
Root program node
├── Declaration statement | type: int
│   ├── Identifier: a
│   └── Add
│       ├── Integer literal: 10
│       └── Integer literal: 5
├── Declaration statement | type: int
│   ├── Identifier: b
│   └── Multiply
│       ├── Identifier: a
│       └── Integer literal: 2
├── Declaration statement | type: int
│   └── Identifier: c
├── Declaration statement | type: int
│   ├── Identifier: e
│   └── Integer literal: 19
├── Declaration statement | type: int
│   ├── Identifier: f
│   └── Multiply
│       ├── Identifier: e
│       └── Integer literal: 6
├── Declaration statement | type: string
│   ├── Identifier: d
│   └── String literal: Hello
├── Assignment statement
│   ├── Identifier: c
│   └── Add
│       ├── Identifier: b
│       └── Integer literal: 0
├── Declaration statement | type: int
│   ├── Identifier: x
│   └── Add
│       ├── Integer literal: 5
│       └── Integer literal: 6
├── Declaration statement | type: int
│   ├── Identifier: y
│   └── Unary minus
│       └── Identifier: x
├── Print statement
│   └── Identifier: d
├── Print statement
│   └── Identifier: f
├── Print statement
│   └── Identifier: a
├── Print statement
│   └── Identifier: b
└── Print statement
    └── Identifier: y

Symbol Table:
1) Symbol: y | Type: int | Initialized: yes
2) Symbol: a | Type: int | Initialized: yes
3) Symbol: b | Type: int | Initialized: yes
4) Symbol: c | Type: int | Initialized: yes
5) Symbol: e | Type: int | Initialized: yes
6) Symbol: f | Type: int | Initialized: yes
7) Symbol: d | Type: string | Initialized: yes
8) Symbol: x | Type: int | Initialized: yes
```

</details>

**Unoptimized Three-Address Code:**

```
1] _t1 = 10 + 5
2] a = _t1
3] _t2 = a * 2
4] b = _t2
5] e = 19
6] _t3 = e * 6
7] f = _t3
8] d = "Hello"
9] _t4 = b + 0
10] c = _t4
11] _t5 = 5 + 6
12] x = _t5
13] _t6 = uminus x
14] y = _t6
15] print d
16] print f
17] print a
18] print b
19] print y
```

**Optimized Three-Address Code:**

```
1] a = 15
2] b = 30
3] f = 114
4] d = "Hello"
5] _t6 = uminus 11
6] y = _t6
7] print d
8] print f
9] print a
10] print b
11] print y
```

The optimizer reduced 19 instructions to 11:

- **Constant folding:** `10 + 5` became `15`.
- **Constant propagation:** Known values flow into later expressions, even through temporaries. `e = 19` is propagated into `e * 6`, which folds to `114`. `a = 15` is propagated into `a * 2`, which folds to `30`.
- **Dead code elimination:** Assignments whose results are never used are removed. This includes the temporaries, `c` (which is never read), `e`, and `x`.

**Output of the program:**

```
Hello
114
15
30
-11
```

## Error Handling

The compiler stops at the first error it finds and reports it.

| Test file | What it contains | Result |
|---|---|---|
| `tests/typechecktest.txt` | A string assigned to an `int` variable | `Data stored in the variable must match its declared type!` followed by the variable name, its declared type, and the expression type |
| `tests/usedbeforedeclarationtest.txt` | `print(d)` where `d` was never declared | `Variable must be declared before usage: d` |
| `tests/runtimeerror.txt` | `int c = a / b;` where `b` is `0` | `Runtime error: division by zero`, detected during constant folding before the program runs |
| `tests/emptyfiletest.txt` | A file with only spaces and newlines | `The source file is empty. Please provide a file containing a program.` |

The lexer also reports errors with line and character positions, for example for an unterminated string literal, or for a symbol that is not part of the language (here, the first byte of a stray `é`):

```
[!] TOKENIZATION ERROR : Unterminated string literal at Line: 6 Character: 13
[!] TOKENIZATION ERROR : Unidentified symbol: (byte value 195) at Line: 4 Character: 12
```

Example of a semantic error (output of `typechecktest.txt`, after the AST):

```
Data stored in the variable must match its declared type!
Variable: typechecktest
Declared type: int
Expression type: string
```

## Test Files

The `tests` folder includes these input files:

- `test.txt`: a sample program that runs successfully
- `typechecktest.txt`: type mismatch error
- `usedbeforedeclarationtest.txt`: undeclared variable error
- `runtimeerror.txt`: division by zero
- `emptyfiletest.txt`: an empty source file (spaces and newlines only)

## Project Structure

```
CompilerDesignFinalProject/
├── Mini Compiler/   Visual C++ project (the compiler source is in src/)
├── tests/           Sample and error-case input programs
└── docs/            Language grammar
```

## Known Limitations

- Only `int` and `string` types are supported. There are no loops or conditional statements; these were kept out of scope for this project.
- Programs are straight-line code with no user input. The optimizer relies on this: it assumes every statement runs once, in order.
- Negation of a constant (`uminus 11`) is not folded into a single value.
- The compiler stops at the first error instead of reporting all of them.
- Non-ASCII characters are rejected outside string literals.
- Windows only, because the driver uses the Windows API.

## Author

Omkar Kulkarni | [LinkedIn](https://www.linkedin.com/in/omkarkulkarni891/)