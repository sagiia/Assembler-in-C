/*
 * Header: main.h
 * --------------
 * The 'main.h' header file contains function declarations and includes other header files required for the assembly process.
 * It serves as the primary interface for the assembly program, providing access to the main functions that drive the
 * assembly process for multiple assembly files.
 *
 * Included Files:
 *   - file_tool.h: Contains functions for file-related operations, such as opening and closing files.
 *   - pre_assembly.h: Contains functions and declarations for the pre-assembly process, which handles macros and expansions.
 *   - first_pass.h: Contains functions and declarations for the first pass of the assembly process, which collects labels and calculates memory addresses.
 *   - second_pass.h: Contains functions and declarations for the second pass of the assembly process, which generates the final machine code.
 *   - setting.h: Contains constant definitions and configurations used throughout the assembly process.
 */

#ifndef MAIN_H
#define MAIN_H

#include "file_tool.h"
#include "pre_assembly.h"
#include "first_pass.h"
#include "second_pass.h"
#include "setting.h"

/*
 * Function: start_assembly
 * ------------------------
 * The 'start_assembly' function serves as the main driver for the assembly process. It is responsible for
 * initiating the assembly process for each assembly file provided as command-line arguments. The function loops
 * through all the files, checks their existence, and begins the assembly process for each valid file.
 *
 * Parameters:
 *   countFiles: An integer representing the number of command-line arguments (number of files to process).
 *   arrayFiles: An array of pointers to strings, where each element represents a file name.
 *
 * Notes:
 *   - This function is called from the 'main' function, which is the entry point of the program.
 *   - If no files are supplied as command-line arguments, the function displays an error message and exits the program.
 */
void start_assembly(int countFiles, char **arrayFiles);

/*
 * Function: start_assembly_process_on_file
 * ---------------------------------------
 * The 'start_assembly_process_on_file' function initiates the assembly process for an individual assembly file.
 * It performs the necessary steps of the assembly process, including pre-assembly, first pass, second pass, and
 * printing the result at the end of the file processing. The function is responsible for managing the file-specific
 * data structure and ensuring the proper sequence of operations during assembly.
 *
 * Parameters:
 *   name_file: A pointer to a string representing the name of the assembly file to be processed.
 *
 * Notes:
 *   - This function is called by the 'start_assembly' function for each valid assembly file provided as a command-line
 *     argument.
 */
void start_assembly_process_on_file(char* name_file);

/*
 * Function: print_end_of_file
 * ---------------------------
 * The 'print_end_of_file' function prints the final result of the assembly process for a specific assembly file.
 * It displays whether the compilation was successful or encountered errors, along with additional information
 * related to the number of lines parsed and any error messages.
 *
 * Parameters:
 *   file_struct: A pointer to the file structure (ptr_file) representing the processed assembly file.
 *
 * Notes:
 *   - This function is called at the end of the assembly process for each assembly file to display the final result.
 *   - The function takes the 'file_struct' as a parameter to access the file-specific data, such as the number of errors,
 *     whether the compilation was successful (indicated by 'error_flag'), and the values of the instruction counter (IC)
 *     and data counter (DC).
 *   - If no errors were encountered during the assembly process (error_flag == FALSE), the function prints a success message
 *     indicating that the compilation was completed successfully.
 */
void print_end_of_file(ptr_file file_struct);

#endif /* MAIN_H */