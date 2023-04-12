# Language-Engine-Test
This is a practice to implement lexical analysis, parsing, compilation and interpretation
#### Current state
I had implemted Parser and Interpreter using raw unions , pointers, cstring and custom allocators.
But I faced lots of hard-debuging bugs. 
So I'm migrating to safer methods like std::variant and smart pointets, etc.
