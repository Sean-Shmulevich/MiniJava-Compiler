# MiniJava Compiler

A compiler for a subset of Java (MiniJava) that compiles source code down to LLVM IR, which can then be compiled to native machine code.

Built as a series of projects for CS 1622 (Compilers) at the University of Pittsburgh, Fall 2024.

## The Language

MiniJava supports:
- Classes with methods
- Integer and string types
- Arithmetic and boolean expressions
- `if`/`else` and `while` control flow
- `system.println` for output
- Arrays (1D and 2D)
- Method parameters and return values

Example (`helloworld.mjava`):
```java
program helloworld;
class Helloworld
{
    method int main()
    {
        system.println('Hello World !!!');
        return 0;
    }
}
```

## Compiler Phases

### 1. Lexical Analysis (Scanner)
- **Tool:** Flex
- **File:** `lex.l`
- Tokenizes MiniJava source into a stream of tokens (identifiers, keywords, literals, operators)
- Builds a string table for identifiers and string constants

### 2. Syntax Analysis (Parser)
- **Tool:** Yacc (Berkeley Yacc)
- **File:** `grammar.y`
- Parses the token stream according to the MiniJava grammar
- Constructs an Abstract Syntax Tree (AST) using a custom tree data structure (`proj2.h`)

### 3. Semantic Analysis
- **Files:** `semantic.cpp`, `proj3.cpp`, `table.cpp`
- Traverses the AST to build a symbol table
- Performs type checking and scope resolution
- Annotates the symbol table with type attributes, nesting levels, and initialization info

### 4. Code Generation
- **File:** `codegen.cpp`
- **Target:** LLVM IR
- Walks the annotated AST and symbol table to emit LLVM IR
- Uses the LLVM C++ API (`IRBuilder`, `Module`, `Function`, `BasicBlock`, etc.)
- Supports function definitions, arithmetic, string output via `printf`, and control flow

## Tools & Dependencies

| Tool | Purpose |
|------|---------|
| **Flex** | Lexer generator |
| **Yacc** (Berkeley Yacc) | Parser generator |
| **LLVM** | IR generation and optimization framework |
| **Clang** | Compiling generated LLVM IR to native binaries |
| **C++17** | Implementation language |

## Building

### macOS (Homebrew)

```bash
brew install llvm bison byacc
make clean && make build
```

### Linux

Requires `clang`, `llvm-config`, `flex`, and `yacc` (version 10+ of LLVM).

```bash
make clean && make build
```

## Usage

**Compile MiniJava to LLVM IR:**
```bash
./codegen -o output.ll tests/helloworld.mjava
```

**Verbose mode (shows symbol table and parse tree):**
```bash
./codegen -v -o output.ll tests/helloworld.mjava
```

**Run the generated IR as a native binary:**
```bash
clang -Wno-override-module -o program output.ll
./program
```

## Project Structure

```
├── lex.l              # Lexer specification
├── grammar.y          # Parser grammar
├── proj2.cpp/h        # AST node construction and traversal
├── proj3.cpp/h        # Symbol table and semantic analysis
├── semantic.cpp       # Semantic analysis pass
├── table.cpp          # Symbol table printing
├── codegen.cpp        # LLVM IR code generation
├── llvm/              # LLVM global declarations
│   └── LLVMGlobals.cpp/h
├── main.cpp           # Driver program
├── tests/             # MiniJava test programs
├── examples/          # LLVM IR example programs
└── Makefile
```

## Test Programs

| Test | Description |
|------|-------------|
| `helloworld.mjava` | Hello world with string output |
| `functions.mjava` | Function definitions and calls |
| `variables.mjava` | Variable declarations and assignments |
| `if-statements.mjava` | Conditional branching |
| `if-else-statements.mjava` | If-else control flow |
| `if-else-while-statements.mjava` | Loops and conditionals |
| `fib.mjava` | Fibonacci sequence |
| `array-1d.mjava` | One-dimensional arrays |
| `array-2d.mjava` | Two-dimensional arrays |
| `objects.mjava` | Object instantiation |
| `multi-class.mjava` | Multiple class definitions |
| `parameters.mjava` | Method parameters |
| `structs.mjava` | Struct-like class usage |

## Acknowledgments

Course framework and project structure by Professor Wonsun Ahn, CS 1622 Fall 2024, University of Pittsburgh.
