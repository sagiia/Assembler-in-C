/*
 * Header: first_pass.h
 * --------------------
 * This header file contains declarations and documentation for functions related to the first pass of the assembly process.
 * The first pass performs various tasks essential for the assembly process, such as handling labels, updating addresses of data labels, and generating the instruction array.
 *
 * Included Files:
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - stdlib.h: Standard Library. It provides functions for memory management and conversions.
 *   - file_tool.h: Contains utility functions for file handling operations and line processing in the first pass of the assembly process.
 *   - text_tool.h: Contains utility functions for string manipulation and parsing in the first pass.
 *   - error_tool.h: Contains functions and error codes for handling errors in the first pass.
 *   - label_list.h: Contains data structures and functions for managing the linked list of labels in the first pass.
 *   - setting.h: Contains constant definitions and configurations used in the first pass of the assembly process.
 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include "file_tool.h"
#include "text_tool.h"
#include "error_tool.h"
#include "label_list.h"
#include "setting.h"

/*
 * Function: start_first_pass
 * --------------------------
 * Initiates the first pass of the assembly process for the provided assembly source file.
 *
 * This function serves as the entry point for the first pass of the assembly process. It sets up necessary
 * initializations and then proceeds to process the file line by line.
 *
 * Parameters:
 *   original_file_struct (ptr_file): A pointer to the 'file' struct representing the original assembly source file.
 *                                    The 'original_file_struct' contains information about the source file,
 *                                    such as the file streams and line structures.
 *
 * Notes:
 *   - The function updates the static global variable 'sfile' with the 'original_file_struct' pointer,
 *     allowing other functions within the 'first_pass.c' file to access and modify information related
 *     to the source file being processed.
 */
void start_first_pass(ptr_file original_file_struct);

#endif /* FIRST_PASS_H */