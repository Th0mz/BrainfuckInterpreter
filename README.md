### BrainFuck
---

- Brainfuck is a programming language designed to test the boundaries of computer programming language design, so called *esoteric programming language*.

<br>

- This programing language only has eight commands that are used to control a memory pointer. This pointer can change the content of the memory that he refers to.

<br>

- This commands are:

| Command | Meaning                                                                 |
|:-------:|-------------------------------------------------------------------------|
|    >    | increment the data pointer (to point to the    next cell to the right). |
|    <    |  decrement the data pointer (to point to the next cell to the left).    |
|    +    | increment (increase by one) the byte at the data pointer.               |
|    -    | decrement (decrease by one) the byte at the data pointer.               |
|    .    | output the byte at the data pointer.                                    |
|    ,    | accept one byte of input, storing its value in the byte at the data pointer.|
|    [    | if the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command                                                                             |
|   ]     | if the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.                                                                 |