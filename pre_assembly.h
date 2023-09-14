/*
 * Header: pre_assembly.h
 * ----------------------
 * This header file contains declarations and documentation for functions related to the pre-assembly process.
 * The pre-assembly process handles macro definitions and expansions, generating an intermediate file with macro-expanded code and macro definitions.
 *
 * Included Files:
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - string.h: C String Library. It provides functions for manipulating strings, such as string copying and comparison.
 *   - file_tool.h: Contains utility functions for file handling operations in the pre-assembly process.
 *   - macro_list.h: Contains data structures and functions for managing the linked list of macro definitions in the pre-assembly process.
 *   - text_tool.h: Contains utility functions for handling text and string operations in the pre-assembly process.
 *   - error_tool.h: Contains functions and error codes for handling errors in the pre-assembly process.
 *   - setting.h: Contains constant definitions and configurations used in the pre-assembly process.
 */

#ifndef PRE_ASSEMBLY_H
#define PRE_ASSEMBLY_H

#include <stdio.h>
#include <string.h>
#include "file_tool.h"
#include "macro_list.h"
#include "text_tool.h"
#include "error_tool.h"
#include "setting.h"

/*
 * Macro: START_MACRO
 * ------------------
 * The keyword used to indicate the start of a macro definition in the assembly file.
 *
 * Notes:
 *   - When the pre-assembly process encounters a line with the first word equal to "START_MACRO",
 *     it starts processing a new macro definition.
 *   - The corresponding macro name should follow the "START_MACRO" keyword on the same line.
 */
#define START_MACRO "mcro"

/*
 * Macro: END_MACRO
 * ----------------
 * The keyword used to indicate the end of a macro definition in the assembly file.
 *
 * Notes:
 *   - When the pre-assembly process encounters a line with the first word equal to "END_MACRO",
 *     it signifies the end of the current macro definition.
 *   - The pre-assembly process saves the text content of the macro and adds it to the list of macros.
 */
#define END_MACRO "endmcro"

/*
 * Enum: first_word_status
 * -----------------------
 * Represents the possible status of the first word in a line during the pre-assembly process.
 * This enum is used to determine the type of line being processed and helps in determining the appropriate action to take.
 *
 * Values:
 *   - STATUS_MACRO_NAME: Indicates that the first word of the line matches an existing macro name.
 *   - STATUS_MCRO: Indicates that the first word of the line is the "START_MACRO" keyword, starting a new macro definition.
 *   - STATUS_ENDMCRO: Indicates that the first word of the line is the "END_MACRO" keyword, ending a macro definition.
 *   - STATUS_TEXT_OF_MACRO: Indicates that the line contains text belonging to a macro being defined.
 *   - STATUS_TEXT_OF_CODE: Indicates that the line contains regular assembly code (not part of a macro definition).
 */
typedef enum {
    STATUS_MACRO_NAME,      /* Indicates that the first word of the line matches an existing macro name. */
    STATUS_MCRO,            /* Indicates that the first word of the line is the "START_MACRO" keyword, starting a new macro definition. */
    STATUS_ENDMCRO,         /* Indicates that the first word of the line is the "END_MACRO" keyword, ending a macro definition. */
    STATUS_TEXT_OF_MACRO,   /* Indicates that the line contains text belonging to a macro being defined. */
    STATUS_TEXT_OF_CODE     /* Indicates that the line contains regular assembly code (not part of a macro definition). */
} first_word_status;

/*
 * Function: start_pre_assembly
 * ----------------------------
 * Entry point for the pre-assembly process.
 *
 * This function initiates the pre-assembly process for the provided assembly source file represented
 * by the 'file_struct'. It sets up necessary initializations and then proceeds to process the file
 * line by line. The pre-assembly process handles macro definitions and expansions and generates the
 * intermediate file with macro-expanded code and macro definitions.
 *
 * Parameters:
 *   file_struct (ptr_file): A pointer to the 'file' struct representing the current assembly source file.
 *                           The 'file_struct' contains information about the source file, such as the file streams,
 *                           line structures, and the linked list of macros defined in the file.
 *
 * Notes:
 *   - The function updates the static global variable 'sfile' with the 'file_struct' pointer, allowing other functions
 *     within the 'pre_assembly.c' file to access and modify information related to the source file being processed.
 */
void start_pre_assembly(ptr_file file_struct);

#endif /* PRE_ASSEMBLY_H */