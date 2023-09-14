/*
 * Header: error_tool.h
 * --------------------
 * This header file contains declarations for error handling and reporting functions in the assembly code.
 * It includes an enum 'error_code' to represent different error types and defines corresponding error messages.
 * The header also provides the declaration for the 'print_error' function that displays error messages to the user.
 *
 * Included Files:
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - text_tool.h: Contains utility functions for handling text and string operations in the pre-assembly process.
 */

#ifndef ERROR_TOOL_H
#define ERROR_TOOL_H

#include <stdio.h>
#include "text_tool.h"

/* Enum: error_code
 * ----------------
 * Represents the possible error codes that can occur during assembly code parsing and processing.
 * Each error code corresponds to a specific error condition encountered while processing the code.
 *
 * List of Error Codes:
 *   - NO_ERROR: No error occurred; the code is valid.
 *   - LABEL_ALREADY_EXISTS: The label with the same name already exists in the symbol table.
 *   - MACRO_ALREADY_EXISTS: The macro with the same name already exists in the macro table.
 *   - COMMA_REQUIRED_BETWEEN_VALUES: A comma is required between consecutive values.
 *   - DATA_NEED_NUM_VALUE: The .data directive requires numeric values as parameters.
 *   - CANT_DEFINE_LABEL_BEFORE_ENTRY: Labels cannot be defined before the .entry directive.
 *   - CANT_DEFINE_LABEL_BEFORE_EXTERN: Labels cannot be defined before the .extern directive.
 *   - STRING_STRUCTURE_NOT_VALID: The structure of a string is not valid; it should be enclosed in quotes.
 *   - STRING_MUST_END_IN_QUOTES: Strings must end with a closing quote.
 *   - STRING_DIRECTIVE_ACCEPTS_ONE_PARAMETER: The .string directive accepts only one parameter (a string).
 *   - TOO_MUCH_WORDS_FOR_INSTRUCTION: Too many words (operands) provided for the instruction.
 *   - CANT_FIND_LABEL_TO_ENTRY: The label specified in the .entry directive was not found in the symbol table.
 *   - INVALID_LABEL_NAME: The label name is invalid; it contains unsupported characters or exceeds the maximum length.
 *   - INSTRUCTION_NAME_NOT_EXIST: The instruction name provided does not correspond to any valid instruction.
 *   - INSTRUCTION_SHOULD_RECEIVE_TWO_OPERANDS: The instruction should receive exactly two operands.
 *   - COMMA_REQUIRED_BETWEEN_OPERANDS: A comma is required between two operands of an instruction.
 *   - INSTRUCTION_SHOULD_RECEIVE_ONE_OPERAND: The instruction should receive exactly one operand.
 *   - INSTRUCTION_SHOULD_NOT_RECEIVE_OPERANDS: The instruction should not receive any operands.
 *   - INVALID_ADDRESS_METHOD_FOR_INSTRUCTION: The address method used for the instruction is invalid or not supported.
 *   - MUST_PROVIDE_LABELS_TO_EXTERN: The .extern directive must be followed by one or more labels.
 *   - MUST_PROVIDE_LABELS_TO_ENTRY: The .entry directive must be followed by one or more labels.
 *   - MUST_PROVIDE_VALUES_TO_DATA: The .data directive must be followed by one or more numeric values.
 *   - INVALID_COMMA_POSITION: The comma is placed at an invalid position in the assembly code.
 *   - LABEL_NOT_FOUND: The label specified in the code was not found in the symbol table.
 *   - NESTED_MACRO_DEFINITION: Nested macro definitions are not allowed; only top-level macros are supported.
 *   - MACRO_NAME_IS_INSTRUCTION_OR_DIRECTIVE: The macro name provided conflicts with a valid instruction or directive name.
 *
 * Notes:
 *   - The error codes are used to identify and report specific issues encountered during the parsing and processing
 *     of assembly code. They provide useful information for debugging and improving the error handling of the assembler.
 *   - Each error code corresponds to a unique error condition, which can be identified and handled separately.
 *   - When an error occurs, the appropriate error code is set, and an error message with details about the error is
 *     displayed to assist the user in understanding and resolving the issue in the assembly code.
 */
typedef enum {
    NO_ERROR,
    LABEL_ALREADY_EXISTS,
    MACRO_ALREADY_EXISTS,
    COMMA_REQUIRED_BETWEEN_VALUES,
    DATA_NEED_NUM_VALUE,
    CANT_DEFINE_LABEL_BEFORE_ENTRY,
    CANT_DEFINE_LABEL_BEFORE_EXTERN,
    STRING_STRUCTURE_NOT_VALID,
    STRING_MUST_END_IN_QUOTES,
    STRING_DIRECTIVE_ACCEPTS_ONE_PARAMETER,
    TOO_MUCH_WORDS_FOR_INSTRUCTION,
    CANT_FIND_LABEL_TO_ENTRY,
    INVALID_LABEL_NAME,
    INSTRUCTION_NAME_NOT_EXIST,
    INSTRUCTION_SHOULD_RECEIVE_TWO_OPERANDS,
    COMMA_REQUIRED_BETWEEN_OPERANDS,
    INSTRUCTION_SHOULD_RECEIVE_ONE_OPERAND,
    INSTRUCTION_SHOULD_NOT_RECEIVE_OPERANDS,
    INVALID_ADDRESS_METHOD_FOR_INSTRUCTION,
    MUST_PROVIDE_LABELS_TO_EXTERN,
    MUST_PROVIDE_LABELS_TO_ENTRY,
    MUST_PROVIDE_VALUES_TO_DATA,
    INVALID_COMMA_POSITION,
    LABEL_NOT_FOUND,
    NESTED_MACRO_DEFINITION,
    MACRO_NAME_IS_INSTRUCTION_OR_DIRECTIVE
} error_code;

/*
 * Function: print_error
 * ---------------------
 * This function is responsible for displaying error messages to the user during the assembly process.
 * It takes an 'error_code' as input to identify the specific error situation and prints an appropriate error message.
 * The function uses the 'error_code' enum to categorize different error types.
 *
 * Parameters:
 *   - code: The 'error_code' representing the specific error situation.
 *   - line: The line number in the assembly code where the error occurred.
 *
 * Notes:
 *   - The 'print_error' function is used whenever an error is detected in the assembly code.
 *   - It takes the error code and line number as input and prints the corresponding error message.
 *   - The function uses ANSI escape sequences to apply red color to the error message for better visibility.
 */
void print_error(error_code code, int line);

#endif /* ERROR_TOOL_H */