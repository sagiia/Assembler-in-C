# Assembler Project - C Course 20465

This project is **_two-pass assembler_** model with the implementation of macros.  

## Getting Started

The project was coded and compiled using Ubuntu, but it should run on all Linux versions.

### Usage
To build the program you can use makefile by make command:
```
>   make
```
The assembly files need to be with an **`.as` extension**.
After compile, you can send the file names to the program as arguments (without the file extensions).

To compile the files first.as, second.as, x.as you need to run:
```
>   assembler first second x
```
The output files with the same filenames and the following extensions:  
- `.ob` - Object file
- `.ent` - Entries file
- `.ext` - Externals file

An example of input and output files can be found under `examples` folder.

## Macros

macros are sections of code that include statements. In the program you can define a macro and use it in different places in the program. The use of a macro from a certain place in the program will cause the macro to be allocated to that place.
Before the two passes, the assembler goes through all the code and interprets the macros.
The program after the macros are retired is the program that the assembler translates.

Setting up a macro is done as follows (in the example the name of the macro is m1):
```
mcro m1
   inc r2
   mov A, r1
endmcro
```
A macro is used by simply mentioning its name.
For example, if somewhere in the program it is written:
```
.
.
m1
.
m1
.
.
```
The program after retiring the macro will look like this:
```
.
.
   inc r2
   mov A, r1
.
   inc r2
   mov A, r1
.
.
```

### Instruction Structure

Every software *instruction* is encoded into a few *words* in memory, The first word is of the following structure:

| 11  10   9| 8 7 6 5 | 4 3 2 | 1 0 
|------------------|---------|-------|----
| Addressing source | Opcode | Addressing destination | A,R,E 


## Commands

| Instruction name | Opcode (decimal) | 
|------------------|------------------ 
| mov              | 0                | 
| cmp              | 1                | 
| add              | 2                | 
| sub              | 3                | 
| not              | 4                | 
| clr              | 5                | 
| lea              | 6                | 
| inc              | 7                | 
| dec              | 8                | 
| jmp              | 9                | 
| bne              | 10               | 
| red              | 11               | 
| prn              | 12               | 
| jsr              | 13               | 
| rts              | 14               | 
| stop             | 15               | 



## Directives
A **_directive_** line of the following structure:

1. An **optional** preceding *label*. e.g. `HELLO: `.
2. A _directive_: `.data`, `.string`, `.struct`, `.entry` or `.extern`.
   3. Operands according to the type of the *directive*.

      ### `.data`
      Allocates memory in the data, storing received integers later in memory.
      The parameters of `.data` are valid integers (separated by a comma).  
      E.g: `.data 4, –31, +42, 99`.

      ### `.string`
      Receives a string as an operand and stores it in the data image. (stores char encoded ny their *ASCII* values). 
      E.g.: `STR: .string “abcdefghi”`
   
      ### `.entry`
      Received name of a *label* to the *symbol table*, for recognize later in other assembly files (and they would be able to use it).
      E.g.: `.entry HELLO`

      ### `.extern`
      Receives a name of a *label* as a parameter and declares the *label* as being external (defined in another file).
      E.g.: `.extern HELLO`
   
 
## Registers
There are 8 CPU registers: r0, r1, r2, r3, r4, r5, r6, r7.<br />
The size of each register is 12 bits.  

