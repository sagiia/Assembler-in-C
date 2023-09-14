/* Header: text_tool.h
 * -------------------
 * This header file defines various tools and data structures used for processing assembly code lines.
 * It includes helper functions for parsing and manipulating text lines, as well as enums and structs
 * to represent different aspects of the assembly code.
 *
 * Included Files:
 *   - stdio.h: Provides standard input/output functions like printf and scanf.
 *   - stdlib.h: Contains general utility functions like memory allocation and conversion functions.
 *   - string.h: Contains string manipulation functions like strcpy and strcat.
 *   - ctype.h: Provides character handling functions like isdigit and isalpha.
 *   - error_tool.h: Contains functions and declarations for handling and reporting errors during the assembly process.
 *   - setting.h: Contains constant definitions and configurations used throughout the assembly process.
 */

#ifndef TEXT_TOOL_H
#define TEXT_TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "error_tool.h"
#include "setting.h"

/* Macro: GET_WORD
 * ---------------
 * A helper macro for parsing words from the input 'temp_line' and storing them in the 'new_line' struct.
 *
 * This macro is used to extract up to five words from the 'temp_line' and store them in the corresponding fields
 * of the 'new_line' struct. It advances the index 'i' through the 'temp_line', extracts words delimited by whitespace,
 * and copies them to 'new_line' fields 'word_number'. It also skips over any additional whitespace between words and
 * sets the 'count' field of 'new_line' to indicate if there are more words in the line beyond the fifth word.
 *
 * Parameters:
 *   - word_number: The identifier for the field in the 'new_line' struct where the word should be stored (e.g., word1, word2, etc.).
 *   - number: The current count of words found so far in the line.
 *
 * Notes:
 *   - The 'GET_WORD' macro is intended for use within the 'create_new_line_struct' function to simplify word extraction
 *     and assignment.
 */
#define GET_WORD(word_number, number)                                                                       \
j = 0;                                                                                                      \
while (temp_line[i] != ' ' && temp_line[i] != '\t' && temp_line[i] != '\n' && temp_line[i] != '\0') {       \
    temp_word[j] = temp_line[i];                                                                            \
    i++;                                                                                                    \
    j++;                                                                                                    \
}                                                                                                           \
temp_word[j] = '\0';                                                                                        \
strcpy(new_line ->word_number, temp_word);                                                                  \
memset(temp_word, 0, MAX_ASSEMBLY_LINE_LENGTH);                                                             \
while (temp_line[i] == ' ' || temp_line[i] == '\t') {                                                       \
i++;                                                                                                        \
}                                                                                                           \
if(temp_line[i] == '\n'||temp_line[i] == '\0'){                                                             \
    new_line ->count = number;                                                                              \
    return new_line;                                                                                        \
}                                                                                                           \

/* Constants: Assembly Directives */
#define DOT_DATA ".data" /* The assembly directive '.data'. */
#define DOT_STRING ".string" /* The assembly directive '.string'. */
#define DOT_ENTRY ".entry" /* The assembly directive '.entry'. */
#define DOT_EXTERN ".extern" /* The assembly directive '.extern'. */

/*
 * Enum: count_word_in_line
 * ------------------------
 * This enumeration represents the count of words in a line of assembly code.
 * It defines named constants to represent different word counts.
 *
 * Constants:
 *   - ZERO: Represents zero words in the line. (This state typically occurs in empty lines)
 *   - ONE: Represents one word in the line.
 *   - TWO: Represents two words in the line.
 *   - TREE: Represents three words in the line.
 *   - FOUR: Represents four words in the line.
 *   - FIVE: Represents five words in the line (maximum allowed).
 *   - TOO_MUCH: Represents more than five words in the line (exceeded maximum).
 *
 * Notes:
 *   - This enumeration is primarily used to indicate the number of words found in a line during assembly code parsing.
 *   - A line in assembly code can have up to five words (e.g., instruction mnemonic, operands, labels, directives).
 *   - If more than five words are found in a line, it is considered an error and flagged as 'TOO_MUCH'.
 */
typedef enum {
    ZERO,       /* 0 words in the line */
    ONE,        /* 1 word in the line */
    TWO,        /* 2 words in the line */
    TREE,       /* 3 words in the line */
    FOUR,       /* 4 words in the line */
    FIVE,       /* 5 words in the line (maximum allowed) */
    TOO_MUCH    /* More than 5 words in the line (exceeded maximum) */
} count_word_in_line;

/*
 * Enum: instruction_type
 * -----------------------
 * This enumeration represents the type of assembly instruction.
 * It defines named constants to represent different types of assembly instructions.
 *
 * Notes:
 *   - The 'NOT_INSTRUCTION' constant is useful for identifying lines that are not valid assembly instructions.
 */
typedef enum {
    MOV,        /* Move instruction */
    CMP,        /* Compare instruction */
    ADD,        /* Add instruction */
    SUB,        /* Subtract instruction */
    NOT,        /* Not instruction */
    CLR,        /* Clear instruction */
    LEA,        /* Load Effective Address instruction */
    INC,        /* Increment instruction */
    DEC,        /* Decrement instruction */
    JMP,        /* Jump instruction */
    BNE,        /* Branch if Not Equal instruction */
    RED,        /* Read instruction */
    PRN,        /* Print instruction */
    JSR,        /* Jump to Subroutine instruction */
    RTS,        /* Return from Subroutine instruction */
    STOP,       /* Stop instruction (halt the program) */
    NOT_INSTRUCTION /* Line that is not an instruction (e.g., labels, directives) */
} instruction_type;

/*
 * Enum: line_status
 * -----------------
 * This enumeration represents the status of a line in the assembly code.
 * It defines named constants to represent different line statuses.
 *
 * Constants:
 *   - STATUS_DATA: Indicates that the line contains a data definition (e.g., .data directive).
 *   - STATUS_STRING: Indicates that the line contains a string definition (e.g., .string directive).
 *   - STATUS_CODE: Indicates that the line contains an assembly instruction (instruction mnemonic).
 *   - STATUS_EXTERN: Indicates an external variable declaration (e.g., .extern directive).
 *   - STATUS_ENTRY: Indicates an entry point declaration (e.g., .entry directive).
 *
 * Notes:
 *   - This enumeration is used to categorize the purpose of each line in the assembly code.
 *   - It helps to distinguish between different types of lines during parsing and processing.
 *   - Each line status corresponds to a specific assembly directive or instruction mnemonic.
 */
typedef enum {
    STATUS_DATA,    /* Line contains data definition */
    STATUS_STRING,  /* Line contains string definition */
    STATUS_CODE,    /* Line contains an assembly instruction */
    STATUS_EXTERN,  /* Line is an external variable declaration */
    STATUS_ENTRY    /* Line is an entry point declaration */
} line_status;

/*
 * Enum: addressing_method
 * -----------------------
 * This enumeration represents the addressing methods used in the operands of an assembly instruction.
 * It defines named constants to represent different addressing methods.
 *
 * Constants:
 *   - NOT_EXIST: Indicates that the addressing method does not exist (not used in the instruction).
 *   - IMMEDIATE: Represents immediate addressing (the operand value is directly used as an operand).
 *   - DIRECT: Represents direct addressing (the operand is a memory address of the data).
 *   - REGISTER: Represents register addressing (the operand is a CPU register).
 */
typedef enum {
    NOT_EXIST,      /* Addressing method does not exist (not used in the instruction) */
    IMMEDIATE = 1,  /* Immediate addressing (operand value is directly used as an operand) */
    DIRECT = 3,     /* Direct addressing (operand is a memory address) */
    REGISTER = 5    /* Register addressing (operand is a CPU register) */
} addressing_method;

/*
 * Enum: encoding_type
 * -------------------
 * This enumeration represents the encoding type of a variable in the assembly code.
 * It defines named constants to represent different encoding types.
 *
 * Constants:
 *   - ABSOLUTE: Represents absolute encoding (the variable is assigned a fixed memory address).
 *   - EXTERNAL: Represents external encoding (the variable is defined in another file or module).
 *   - RELOCATABLE: Represents relocatable encoding (the variable's address is determined at linking time).
 *
 * Notes:
 *   - This enumeration is used to indicate the type of encoding used for a variable in the assembly code.
 *   - The encoding type affects how the assembler generates machine code for the variable.
 */
typedef enum {
    ABSOLUTE,       /* Absolute encoding (variable has a fixed memory address) */
    EXTERNAL,       /* External encoding (variable is defined in another file or module) */
    RELOCATABLE     /* Relocatable encoding (variable's address is determined at linking time) */
} encoding_type;

/*
 * Struct: line_struct
 * -------------------
 * This struct represents a line of assembly code, storing its components and relevant information.
 *
 * Members:
 *   - word1: A character array to store the first word in the assembly line.
 *   - word2: A character array to store the second word in the assembly line.
 *   - word3: A character array to store the third word in the assembly line.
 *   - word4: A character array to store the fourth word in the assembly line.
 *   - word5: A character array to store the fifth word in the assembly line.
 *   - source: An enumeration indicating the addressing method of the source operand in an instruction.
 *   - destination: An enumeration indicating the addressing method of the destination operand in an instruction.
 *   - count: An enumeration representing the count of words in the line (e.g., one, two, three, etc.).
 *
 * Notes:
 *   - The struct is used to store the components of an assembly line after parsing.
 *   - The 'source' and 'destination' members specify the addressing method of the instruction operands.
 *   - The 'count' member indicates the number of words found in the line during assembly code parsing.
 *   - This struct is typically used to process and analyze individual lines of the assembly code.
 */
typedef struct line_struct * ptr_line;
typedef struct line_struct {
    char word1[MAX_ASSEMBLY_LINE_LENGTH];  /* First word in the assembly line */
    char word2[MAX_ASSEMBLY_LINE_LENGTH];  /* Second word in the assembly line */
    char word3[MAX_ASSEMBLY_LINE_LENGTH];  /* Third word in the assembly line */
    char word4[MAX_ASSEMBLY_LINE_LENGTH];  /* Fourth word in the assembly line */
    char word5[MAX_ASSEMBLY_LINE_LENGTH];  /* Fifth word in the assembly line */
    addressing_method source;              /* Addressing method of the source operand in an instruction */
    addressing_method destination;         /* Addressing method of the destination operand in an instruction */
    count_word_in_line count;              /* Count of words in the line */
} item_line;


/* Function: create_new_line_struct
 * --------------------------------
 * Creates and initializes a new line struct based on the provided text line.
 *
 * This function takes a string 'text_line' as input and creates a new line struct, dynamically
 * allocating memory for it. The function then parses the input 'text_line' to extract up to five
 * words (delimited by whitespace) and stores them in the corresponding fields of the newly created
 * line struct.
 *
 * Parameters:
 *   - text_line: A pointer to a string containing the text of the line to be processed.
 *
 * Returns:
 *   - ptr_line: A pointer to the newly created and initialized line struct.
 *
 * Notes:
 *   - If dynamic memory allocation fails, the function prints an error message to stderr and exits the program.
 *   - The 'initialize_variables' function is called to reset all fields of the new line struct to zero .
 *   - The 'GET_WORD' macro is used to extract individual words from the 'text_line' and store them in the line struct.
 *   - The line struct is designed to store up to five words, but the function does not check if there are more than five words
 *     in the input 'text_line'. It simply stores the first five words found, and sets the 'count' field to indicate if there
 *     were more words in the line.
 */
ptr_line create_new_line_struct(const char *text);

/* Function: delete_label_from_line_struct
 * ---------------------------------------
 * Deletes the first word from a line struct and shifts the remaining words.
 *
 * Parameters:
 *   - line_struct: A pointer to the line struct from which the first word is to be deleted.
 *
 * Notes:
 *   - The 'delete_label_from_line_struct' function is designed for use during the first pass of the assembly process
 *     to remove the label (if any) from the input line after it has been processed and the relevant information has
 *     been extracted. This operation prepares the line for further processing during the assembly.
 *   - The function uses the 'strcpy' function to copy the content of one word to another and the 'memset' function to
 *     set the fifth word to an empty string.
 *   - The function does not check whether the line struct has a label (first word) before attempting to delete it.
 *     If the line struct's 'count' field is already zero or one (indicating that there is no label), the function will
 *     still perform the word shifting and clearing without any errors.
 */
void delete_label_from_line_struct(ptr_line line_struct);

/* Function: free_line
 * -------------------
 * Frees the memory allocated for a line struct.
 *
 * Parameters:
 *   - line_struct: A pointer to the line struct that needs to be freed.
 *
 * Notes:
 *   - The 'free_line' function is designed to be used when the line struct is no longer needed and can be
 *     safely deallocated. After calling this function, the 'line_struct' pointer should not be used again
 *     to access the freed memory.
 */
void free_line(ptr_line line_struct);

/* Function: skip_white_character
 * ------------------------------
 * Skips over white spaces (spaces and tabs) in the input text starting from the specified position.
 *
 * Parameters:
 *   - curr_position: An integer representing the starting position from which to begin scanning in 'text'.
 *   - text: A pointer to a null-terminated string containing the text to be scanned.
 *
 * Returns:
 *   - int: The updated position after skipping over white space characters.
 *
 * Notes:
 *   - The 'skip_white_character' function is used to move past any leading white space characters in a text
 *     line during parsing or processing. It is often utilized in assembly code analysis to skip over spaces
 *     and tabs before extracting relevant information from the text.
 *   - The function scans through 'text' character by character, starting from 'curr_position', and increments
 *     the position until a non-white space character (i.e., not a space or tab) is encountered or the end of
 *     the string is reached.
 *   - The function returns the updated position after skipping over white space characters. This position can
 *     be used as a starting point for further parsing or processing of the text.
 *   - If 'curr_position' is already pointing to a non-white space character or the null terminator at the end
 *     of the string, the function returns 'curr_position' unchanged.
 *   - The function does not modify the content of 'text'; it only provides an updated position after skipping
 *     over white space characters.
 */
int skip_white_character(int curr_position, const char *text);

/* Function: skip_one_word_in_line
 * -------------------------------
 * Skips over one word in the input line text starting from the specified position.
 *
 * Parameters:
 *   - curr_position: An integer representing the starting position from which to begin scanning in 'line_text'.
 *   - line_text: A pointer to a null-terminated string containing the line of text to be scanned.
 *
 * Returns:
 *   - int: The updated position after skipping over the current word.
 *
 * Notes:
 *   - The 'skip_one_word_in_line' function is used to move past one complete word in a line of text during parsing
 *     or processing. It is often utilized in assembly code analysis to extract individual words from the text for
 *     further examination or processing.
 */
int skip_one_word_in_line(int curr_position, const char *text);

/* Function: is_end_line
 * ---------------------
 * Checks if the given position in the line text corresponds to the end of the line.
 *
 * Parameters:
 *   - curr_position: An integer representing the starting position from which to begin scanning in 'line_text'.
 *   - line_text: A pointer to a null-terminated string containing the line of text to be examined.
 *
 * Returns:
 *   - bool: 'TRUE' if the given position corresponds to the end of the line (i.e., an empty line), 'FALSE' otherwise.
 *
 * Notes:
 *   - The function scans through 'line_text' character by character, starting from the specified 'curr_position',
 *     and checks for white space characters (spaces and tabs). If it encounters any non-white space character
 *     before reaching the end of the line, it returns 'FALSE' to indicate that the line is not empty.
 *   - The 'is_end_line' function does not modify the content of 'line_text' and only performs read-only operations
 *     on the text to check for the presence of white space characters or the end of the line.
 */
bool is_end_line(int curr_position, const char *line_text);

/* Function: is_label
 * ------------------
 * Checks if the first word in the line struct is a label.
 *
 * Parameters:
 *   - line_struct: A pointer to the 'ptr_line' struct containing the line of text to be examined.
 *
 * Returns:
 *   - bool: 'TRUE' if the first word in 'line_struct' is a label, 'FALSE' otherwise.
 *
 * Notes:
 *   - The 'is_label' function helps determine if a line of text starts with a label. Labels are used to mark memory
 *     locations or specific positions in the assembly code and end with a colon (':').
 *   - The 'is_label' function does not modify any other part of 'line_struct' except 'line_struct->word1' when it
 *     processes a line containing a label.
 */
bool is_label(ptr_line line_struct);

/* Function: is_number
 * -------------------
 * Checks if the given string represents a numeric value.
 *
 * This function takes a pointer to a character array 'text' as input and checks whether the content of the array
 * represents a numeric value. A numeric value can be either a positive or negative integer, containing only digits
 * (0-9) and an optional leading sign ('+' or '-').
 *
 * Parameters:
 *   - text: A pointer to a character array (string) to be checked for representing a numeric value.
 *
 * Returns:
 *   - bool: 'TRUE' if 'text' represents a numeric value, 'FALSE' otherwise.
 *
 * Notes:
 *   - The function does not consider floating-point numbers or numbers with exponent notation. It only checks for
 *     positive or negative integers consisting of digits (0-9) and an optional leading sign.

 *   - The function is useful for validating user input or parsing numeric values from strings in applications such as
 *     assemblers, interpreters, or compilers.
 */
bool is_number(char * text);

/* Function: is_line_empty
 * -----------------------
 * Checks if a given line of text is empty or contains only whitespace characters and comments.
 *
 * This function takes a pointer to a character array 'line_text' representing a line of text in an assembly code.
 * It checks whether the line is considered empty or contains only whitespace characters and comments. An empty line
 * is a line that either contains no characters (except the newline '\n' character) or only consists of whitespace
 * characters (' ' and '\t') and optional leading comments (denoted by a semicolon ';').
 *
 * Parameters:
 *   - line_text: A pointer to a character array (string) representing a line of text in the assembly code.
 *
 * Returns:
 *   - bool: 'TRUE' if the line is empty or contains only whitespace characters and optional leading comments (';'),
 *           'FALSE' otherwise.
 *
 * Notes:
 *   - The function treats a line starting with a semicolon ';' as empty, considering it a comment line.
 *   - The function is useful for detecting empty lines or lines with only comments in assembly code processing.
 */
bool is_line_empty(const char *line_text);

/* Function: is_label_name_valid
 * -----------------------------
 * Checks if the given name_label is a valid label name in assembly code.
 *
 * This function takes a pointer to a character array 'name_label', which represents the label name in assembly code.
 * It performs several checks to determine whether the 'name_label' is a valid label name:
 *   - It first checks if 'name_label' matches any of the reserved words using the 'is_name_a_reserved_word' function.
 *     If a match is found, the function immediately returns 'FALSE' to indicate that it is not a valid label name.
 *   - Next, it verifies that the first character of 'name_label' is an alphabet character (a-z or A-Z).
 *     If the first character is not an alphabet character, the function returns 'FALSE'.
 *   - Then, it iterates through the 'name_label' to ensure that it contains only alphabets (a-z or A-Z) and digits (0-9).
 *     If it encounters any non-alphanumeric characters, the function returns 'FALSE'.
 *   - The function also keeps track of the number of characters in 'name_label', ensuring it does not exceed the maximum
 *     allowable label name length (MAX_NAME_LABEL_LENGTH).
 *     If the length of 'name_label' exceeds the maximum length, the function returns 'FALSE'.
 *
 * Parameters:
 *   - name_label: A pointer to a character array (string) representing the label name to be checked for validity.
 *
 * Returns:
 *   - bool: 'TRUE' if 'name_label' is a valid label name, 'FALSE' otherwise.
 *
 * Notes:
 *   - It uses the 'is_name_a_reserved_word' function to check if 'name_label' matches any reserved words.
 */
bool is_label_name_valid(char *name_label);

/* Function: is_name_a_reserved_word
 * --------------------------------
 * Checks if the given name is a reserved word in the assembly language.
 *
 * This function takes a pointer to a character array 'name', which represents a word or identifier
 * in the assembly language. It compares 'name' against a list of known reserved words to determine
 * if it matches any of them.
 *
 * Parameters:
 *   - name: A pointer to a character array (string) representing the word or identifier to be checked.
 *
 * Returns:
 *   - bool: 'TRUE' if 'name' is a reserved word, 'FALSE' otherwise.
 *
 * Notes:
 *   - The function is case-sensitive; reserved words must match exactly, including letter case.
 *   - The reserved words checked by this function include assembler directives (e.g., ".data", ".string"),
 *     processor registers (e.g., "@r0", "@r2"), and assembly instructions (e.g., "mov", "cmp").
 */
bool is_name_a_reserved_word(char *name);

/* Function: get_instruction_type
 * ------------------------------
 * Retrieves the instruction type for the given assembly instruction word.
 *
 * This function takes a pointer to a character array 'word', representing an assembly instruction
 * mnemonic. It compares 'word' against a list of known assembly instructions and returns the
 * corresponding instruction type enumeration.
 *
 * Parameters:
 *   - word: A pointer to a character array (string) representing the assembly instruction mnemonic.
 *
 * Returns:
 *   - instruction_type: An enumeration representing the type of the assembly instruction.
 *     - MOV: Move instruction.
 *     - CMP: Compare instruction.
 *     - ADD: Add instruction.
 *     - SUB: Subtract instruction.
 *     - NOT: Not instruction.
 *     - CLR: Clear instruction.
 *     - LEA: Load Effective Address instruction.
 *     - INC: Increment instruction.
 *     - DEC: Decrement instruction.
 *     - JMP: Jump instruction.
 *     - BNE: Branch if Not Equal instruction.
 *     - RED: Read instruction.
 *     - PRN: Print instruction.
 *     - JSR: Jump to Subroutine instruction.
 *     - RTS: Return from Subroutine instruction.
 *     - STOP: Stop instruction.
 *     - NOT_INSTRUCTION: Indicates that 'word' does not match any known assembly instruction.
 *
 * Notes:
 *   - The function is case-sensitive; instruction mnemonics must match exactly, including letter case.
 */
instruction_type get_instruction_type(const char *word);

/* Function: get_addressing_method_type
 * -------------------------------------
 * Retrieves the addressing method type for the given assembly operand.
 *
 * This function takes a pointer to a character array 'word', representing an assembly operand.
 * It checks the content of 'word' to determine its addressing method type and returns the corresponding
 * addressing_method enumeration.
 *
 * Parameters:
 *   - word: A pointer to a character array (string) representing the assembly operand.
 *
 * Returns:
 *   - addressing_method: An enumeration representing the addressing method type of the assembly operand.
 *     - NOT_EXIST: Indicates that 'word' is empty (null-terminated) and does not contain any valid operand.
 *     - IMMEDIATE: Indicates that 'word' contains an immediate value, such as a numeric constant.
 *     - REGISTER: Indicates that 'word' represents a register operand, such as "@r1" or "@r2".
 *     - DIRECT: Indicates that 'word' represents a direct addressing method.
 *
 * Notes:
 *   - The function is case-sensitive for the "REGISTER" addressing method; register operands must match exactly,
 *     including letter case ("@r1" and "@R1" are considered different).
 *   - The function relies on the 'is_number' and 'is_register' helper functions to identify the addressing method type.
 */
addressing_method get_addressing_method_type(char *word);

/* Function: get_number_of_register
 * --------------------------------
 * Extracts the register number from a given register name.
 *
 * This function takes a pointer to a character array 'register_name', representing a register name in the format "@rX",
 * where 'X' is a digit from 0 to 7 (e.g., "@r0", "@r1", ..., "@r7"). The function extracts the numeric value of 'X'
 * and returns it as an integer.
 *
 * Parameters:
 *   - register_name: A pointer to a character array (string) representing the register name in the format "@rX".
 *
 * Returns:
 *   - int: An integer representing the numeric value of the register extracted from 'register_name'.
 *
 * Notes:
 *   - The function assumes that 'register_name' is a valid register name in the "@rX" format.
 *   - It is the caller's responsibility to ensure that 'register_name' indeed represents a valid register name.
 *   - If 'register_name' is not a valid register name (e.g., does not match the "@rX" format), the function behavior
 *     is undefined, and the result will be unpredictable.
 *   - The function extracts the numeric value by subtracting the character '0' (ASCII value 48) from the third character
 *     of 'register_name', which corresponds to the digit 'X' in the "@rX" format.
 */
int get_number_of_register(char *register_name);

/* Function: convert_binary_to_64base
 * ----------------------------------
 * Converts a 12-bit binary word to a 64-base representation.
 *
 * This function takes an unsigned integer 'word' representing a 12-bit binary word and converts it into a 64-base
 * (base64) representation.
 *
 * binary word is used as an index into the lookup table to obtain the corresponding character in the base64 encoding.
 * The resulting base64 representation is a three-character string (plus a null terminator) that can be used to represent
 * the original 12-bit binary word.
 *
 * Parameters:
 *   - word: An unsigned integer representing the 12-bit binary word to be converted.
 *
 * Returns:
 *   - char*: A dynamically allocated pointer to a string containing the base64 representation of the input 'word'.
 *            The caller is responsible for freeing the memory allocated by this function when it is no longer needed.
 *
 * Notes:
 *   - The function dynamically allocates memory to store the base64 representation of the input 'word'.
 *   - If the memory allocation fails, the function prints an error message to stderr and exits the program.
 *   - The function uses the 'get_specific_bits' helper function to extract specific bits from the input 'word'.
 */
char * convert_binary_to_64base(unsigned int word);

/* Function: print_red
 * -------------------
 * Sets the text color in the terminal to red.
 *
 * This function uses ANSI escape codes to change the text color in the terminal to red.
 * When called, any subsequent text printed to the terminal will appear in red until the color is reset.
 *
 */
void print_red();


/* Function: print_reset
 * ---------------------
 * Resets the text color in the terminal to the default color.
 *
 * This function uses ANSI escape codes to reset the text color in the terminal to the default color.
 * When called, any subsequent text printed to the terminal will use the default color.
 *
 */
void print_reset();

/* Function: print_line
 * --------------------
 * (For Debugging) Prints the content of a 'ptr_line' structure to the console.
 *
 * This function takes a pointer to a 'ptr_line' structure and prints the content of its 'word1', 'word2', 'word3',
 * 'word4', and 'word5' members, separated by spaces. Additionally, it prints the 'count' member at the end of the line.
 * The function is intended for debugging purposes to display the content of the 'ptr_line' structure and its associated
 * information.
 *
 * Parameters:
 *   - line_struct: A pointer to the 'ptr_line' structure to be printed.
 *
 * Notes:
 *   - The function is declared with '__attribute__((unused))' to suppress unused function warnings. It is intended for
 *     debugging purposes and may not be used in all cases, but it provides valuable information when debugging the code.
 *   - The function does not modify the content of the 'ptr_line' structure or any associated data.
 *   - The printed output format is as follows:
 *       "word1 word2 word3 word4 word5 count: X\n"
 *     where 'word1', 'word2', 'word3', 'word4', and 'word5' represent the content of the corresponding members of
 *     the 'ptr_line' structure, and 'X' is the value of the 'count' member.
 */
__attribute__((unused)) void print_line(ptr_line line_struct);

#endif /* TEXT_TOOL_H */