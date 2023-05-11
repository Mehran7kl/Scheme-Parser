# Aim
This is a practice to implement lexical analysis, parsing, compilation and interpretation
## Targets:
Implementing
+ Lexer
+ Parser
+ Interpreter
+ Compiler
+ Optimiser
### The Language
A LISP dialect. Minimalist Scheme as close as possible.
#### Current state
I have implemted Parser and Interpreter using raw unions , pointers, cstring and custom allocators.
But I faced lots of hard-debuging bugs. 
Now I adopted safer methods like std::variant and smart pointets, etc.
Finally it's on alpha state; means it's working tho not reliable.

