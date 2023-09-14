/*
 * Header: second_pass.h
 * ---------------------
 * This header file contains declarations and documentation for functions related to the second pass of the assembly process.
 * The second pass performs various tasks necessary to complete the assembly process, such as generating the object file,
 * handling entry labels, and updating addressing methods of instructions.
 *
 * Included Files:
 *   - file_tool.h: Contains utility functions for file handling operations and line processing in the second pass of the assembly process.
 *   - text_tool.h: Contains utility functions for string manipulation and parsing in the second pass.
 *   - setting.h: Contains constant definitions and configurations used in the second pass of the assembly process.
 */

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "file_tool.h"
#include "text_tool.h"
#include "setting.h"

/*
 * Function: start_second_pass
 * ---------------------------
 * Entry point for the second pass of the assembly process.
 *
 * This function initiates the second pass for the provided assembly source file represented by the 'file_struct'.
 * It sets up necessary initializations and then proceeds to process the file line by line. The second pass handles
 * instructions and data sections, generates the final machine code, and creates the output file with the assembled
 * machine code and the list of external labels encountered during the pass.
 *
 * Parameters:
 *   original_file_struct (ptr_file): A pointer to the 'file' struct representing the current assembly source file.
 *                                    The 'file_struct' contains information about the source file, such as the file streams,
 *                                    line structures, data arrays, and instruction arrays.
 */
void start_second_pass(ptr_file original_file_struct);

#endif /* SECOND_PASS_H */