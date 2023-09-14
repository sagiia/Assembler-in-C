/* Header File: file_tool.h
 * ------------------------
 * This header file defines data structures and function declarations related to file handling
 * and file I/O operations in the assembler.
 *
 * Included Files:
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - stdlib.h: Standard Library. It provides functions for memory allocation, conversion, and other utility functions.
 *   - string.h: C String Library. It provides functions for manipulating strings, such as string copying and comparison.
 *   - unistd.h: POSIX Standard Library. It provides various symbolic constants and types and declares various functions that are useful for interacting with the operating system.
 *   - macro_list.h: Contains data structures and functions for managing the linked list of macro definitions in the pre-assembly process.
 *   - file_tool.h: Contains utility functions for file handling operations in the pre-assembly process.
 *   - label_list.h: Contains data structures and functions for managing the linked list of label definitions in the pre-assembly process.
 *   - text_tool.h: Contains utility functions for handling text and string operations in the pre-assembly process.
 *   - setting.h: Contains constant definitions and configurations used in the pre-assembly process.
 */

#ifndef FILE_TOOL_H
#define FILE_TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "macro_list.h"
#include "file_tool.h"
#include "label_list.h"
#include "text_tool.h"
#include "setting.h"

/*
 * Enum: file_exists_status
 * ------------------------
 * This enumeration represents the status of a file existence check.
 * It defines named constants to represent different outcomes of the file check.
 *
 * Constants:
 *   - NO_EXISTS: Indicates that the file does not exist.
 *   - EXISTS: Indicates that the file exists.
 *   - TOO_LONG: Indicates that the file name is too long.
 *
 * Notes:
 *   - This enumeration is used to indicate the status of a file existence check in the assembly code.
 */
typedef enum {
    NO_EXISTS,      /* File does not exist. */
    EXISTS,         /* File exists. */
    TOO_LONG        /* File name is too long. */
} file_exists_status;

/*
 * Enum: file_ext
 * --------------
 * This enumeration represents different file extensions used in the assembly code.
 * It defines named constants to represent different types of files.
 *
 * Constants:
 *   - EXT_INPUT: Represents an input file.
 *   - EXT_MACRO: Represents a macro file.
 *   - EXT_OBJECT: Represents an object file.
 *   - EXT_EXTERN: Represents an external references file.
 *   - EXT_ENTRY: Represents an entry labels file.
 *
 * Notes:
 *   - This enumeration is used to categorize and identify different types of files used during the assembly process.
 */
typedef enum {
    EXT_INPUT,      /* Input file. */
    EXT_MACRO,      /* Macro file. */
    EXT_OBJECT,     /* Object file. */
    EXT_EXTERN,     /* External references file. */
    EXT_ENTRY       /* Entry labels file. */
} file_ext;

/*
 * Struct: file_struct
 * -------------------
 * This struct represents a file and its associated data in the assembly code.
 * It holds information about the file's name, current line of text, counters, flags, data arrays, and file pointers.
 *
 * Fields:
 *   - name_file: A character array to store the name of the file.
 *   - line_text: A character array to store the current line of text from the file.
 *   - current_line: An integer representing the current line number in the file.
 *   - pos_in_line: An integer representing the current position within the current line.
 *   - count_macro: An integer representing the number of macros in the file.
 *   - count_error: An integer representing the number of errors encountered in the file.
 *   - count_line: An integer representing the total number of lines in the file.
 *   - IC: An integer representing the Instruction Counter.
 *   - DC: An integer representing the Data Counter.
 *   - error_flag: A boolean flag indicating if an error occurred while processing the file.
 *   - extern_flag: A boolean flag indicating if there are external references in the file.
 *   - entry_flag: A boolean flag indicating if there are entry labels in the file.
 *   - data_array: An array of unsigned integers to store data values.
 *   - instruction_array: An array of unsigned integers to store instruction values.
 *   - line_struct: A pointer to the current line's structure (item_line).
 *   - head_macro_list: A pointer to the head of the macro list (ptr_macro).
 *   - head_label_list: A pointer to the head of the label list (ptr_label).
 *   - file_as: A file pointer for the assembly file.
 *   - file_am: A file pointer for the machine code (object) file.
 *   - file_ob: A file pointer for the object file.
 *   - file_ent: A file pointer for the entry labels file.
 *   - file_ext: A file pointer for the external references file.
 *
 * Notes:
 *   - This struct is used to store relevant data and settings related to a specific assembly file.
 *   - It is designed to facilitate the assembly code processing, error detection, and file handling.
 */
typedef struct file_struct *ptr_file;
typedef struct file_struct {
    char name_file[MAX_FILE_NAME_LENGTH];    /* Name of the file. */
    char line_text[MAX_ASSEMBLY_LINE_LENGTH];/* Current line of text from the file. */

    int current_line;       /* Current line number in the file. */
    int pos_in_line;        /* Current position within the current line. */
    int count_macro;        /* Number of macros in the file. */
    int count_error;        /* Number of errors encountered in the file. */
    int count_line;         /* Total number of lines in the file. */
    int IC;                 /* Instruction Counter. */
    int DC;                 /* Data Counter. */

    bool error_flag;        /* Flag indicating if an error occurred while processing the file. */
    bool extern_flag;       /* Flag indicating if there are external references in the file. */
    bool entry_flag;        /* Flag indicating if there are entry labels in the file. */

    unsigned int data_array[MAX_ARRAY_SIZE];           /* Array to store data values. */
    unsigned int instruction_array[MAX_ARRAY_SIZE];    /* Array to store instruction values. */

    ptr_line line_struct;       /* Pointer to the current line's structure. */
    ptr_macro head_macro_list;  /* Pointer to the head of the macro list. */
    ptr_label head_label_list;  /* Pointer to the head of the label list. */

    FILE *file_as;      /* File pointer for the assembly file. */
    FILE *file_am;      /* File pointer for the machine code (object) file. */
    FILE *file_ob;      /* File pointer for the object file. */
    FILE *file_ent;     /* File pointer for the entry labels file. */
    FILE *file_ext;     /* File pointer for the external references file. */

} item_file;

/* Function: create_new_file_struct
 * --------------------------------
 * Creates and initializes a new file struct with the given file name.
 *
 * This function dynamically allocates memory for a new file struct and initializes its members with default values.
 *
 * Parameters:
 *   - name_file: A pointer to a string containing the name of the file to be associated with the new file struct.
 *
 * Returns:
 *   - A pointer to the newly created and initialized file struct.
 *
 * Notes:
 *   - If dynamic memory allocation fails, the function prints an error message to stderr and exits the program.
 */
ptr_file create_new_file_struct(char *name_file);

/* Function: free_file
 * -------------------
 * Frees the memory allocated for a file struct.
 *
 * This function releases the memory occupied by the file struct pointed to by 'file_struct'.
 * If 'file_struct' is a valid pointer (not NULL), the memory is deallocated and becomes available
 * for reuse. If 'file_struct' is NULL, the function does nothing and returns immediately.
 *
 * Parameters:
 *   - file_struct: A pointer to the file struct that needs to be freed.
 *
 * Returns:
 *   - None.
 *
 * Notes:
 *   - It is essential to call this function for each file struct created using 'create_new_file_struct' to avoid memory leaks.
 */
void free_file(ptr_file file_struct);

/*
 * Function: file_exists
 * ---------------------
 * The 'file_exists' function checks if a file with the given name exists in the current directory. It also
 * verifies that the file name is not too long. The function uses the 'access' function to check for file
 * existence and returns the appropriate status.
 *
 * Parameters:
 *   name_file: A pointer to a string representing the name of the file to check for existence.
 *
 * Returns:
 *   file_exists_status: An enumerated type representing the status of the file existence check.
 *
 * Notes:
 *   - The 'file_exists' function calls the 'valid_file_name' function to ensure that the file name is not too long.
 *   - If the file name is too long, the function returns 'TOO_LONG'.
 *   - The 'access' function is used to check if the file exists. It returns -1 if the file does not exist and 0 otherwise.
 *   - If the file exists, the function returns 'EXISTS'.
 *   - If the file does not exist, the function returns 'NO_EXISTS'.
 *   - The 'get_file_with_extension' function is used to construct the full file name with the appropriate extension
 *     before checking for file existence.
 */
file_exists_status file_exists(char *name_file);

/* Function: valid_file_name
 * ----------------------
 * Checks if the provided file name is valid.
 *
 * This function verifies whether the length of the file name specified in 'name_file'
 * is within the acceptable limits. If the file name exceeds the maximum allowable file name length
 * (MAX_FILE_NAME_LENGTH), the function returns FALSE to indicate that the file name is too long.
 * Otherwise, it returns TRUE, meaning the name is within the allowed length.
 *
 * Parameters:
 *   - name_file: A pointer to a string containing the name of the file to be checked.
 *
 * Returns:
 *   - bool: 'TRUE' if the file name is within the acceptable limits, 'FALSE' otherwise.
 */
bool valid_file_name(char *name_file);

/* Function: get_file_with_extension
 * ----------------------
 * Adds a file extension to the provided file name.
 *
 * This function appends a file extension to the given 'name_file' based on the 'ext' value.
 * The resulting full file name, including the extension, is stored in a new dynamically allocated
 * buffer, and a pointer to this buffer is returned.
 *
 * Parameters:
 *   - name_file: A pointer to a string containing the base name of the file.
 *   - ext: An enum 'file_ext' specifying the type of file extension to be added.
 *
 * Returns:
 *   - char*: A dynamically allocated pointer to a string containing the full file name with the extension.
 *            The caller is responsible for freeing the memory allocated by this function when it is no
 *            longer needed.
 *
 * Notes:
 *   - If memory allocation fails, an error message is printed to the standard error stream, and the program exits.
 *   - The 'file_ext' enum values determine the type of extension to be added to the file name:
 *     - EXT_MACRO: Appends ".am" (e.g., "example.am") for pre-assembler output.
 *     - EXT_OBJECT: Appends ".ob" (e.g., "example.ob") for the result after the pass operation.
 *     - EXT_EXTERN: Appends ".ext" (e.g., "example.ext") for external symbols.
 *     - EXT_ENTRY: Appends ".ent" (e.g., "example.ent") for entry symbols.
 *     - Default: Appends ".as" (e.g., "example.as") for the original file extension.
 */
char *get_file_with_extension(char *name_file, file_ext ext);

/* Function: open_file
 * ----------------------
 * Opens a file with the specified file extension and permission mode.
 *
 * This function constructs the full file name by appending the appropriate file extension to the
 * 'name' parameter based on the 'ext' value. It then attempts to open the file using the provided
 * 'permission' mode. If the file cannot be opened, an error message is printed to the standard
 * error stream, and the program exits.
 *
 * Parameters:
 *   - name: A pointer to a string containing the base name of the file.
 *   - ext: An enum 'file_ext' specifying the type of file extension to be added.
 *   - permission: A string containing the permission mode for opening the file (e.g., "r", "w", "a", etc.).
 *
 * Returns:
 *   - FILE*: A pointer to the opened file.
 *
 * Notes:
 *   - The function uses the 'get_file_with_extension' function to obtain the full file name with the extension.
 *   - If the file cannot be opened, an error message is printed to the standard error stream, and the program exits.
 */
FILE *open_file(char *name, file_ext ext, char permission[]);

/* Function: print_file
 * ----------------------
 * (For Debugging) Prints the contents of a file structure and associated lists.
 *
 * Parameters:
 *   - head: A pointer to the file structure to be printed.
 *
 * Notes:
 *   - The function is declared with '__attribute__((unused))' to suppress unused function warnings.
 *     It is intended to be used for debugging purposes to print the internal state of the file structure.
 *   - The function does not modify the content of the file structure or its associated lists.
 */
__attribute__((unused)) void print_file(ptr_file head);

#endif /* FILE_TOOL_H */