/*
 * Header: macro_list.h
 * --------------------
 * This header file defines the data structures and function prototypes related to the macro list used for storing macro information
 * in the assembly code. It includes other necessary headers and provides access to the functions and data structures needed for
 * macro management.
 *
 * Included Files:
 *   - stdlib.h: Standard Library. It provides functions for memory management and conversions.
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - string.h: C String Library. It provides functions for manipulating strings, such as string copying and comparison.
 *   - setting.h: Contains constant definitions and configurations used in the macro list and macro management.
 *   - error_tool.h: Contains functions and error codes for handling errors related to macro list operations.
 */

#ifndef MACRO_LIST_H
#define MACRO_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setting.h"
#include "error_tool.h"

/*
 * Struct: node_macro
 * ------------------
 * Represents a node in the macro list used for storing macro information.
 *
 * Fields:
 *  - name_macro: An array to store the name of the macro.
 *  - text_macro: An array to store the text associated with the macro.
 *  - next: A pointer to the next node in the macro list.
 */
typedef struct node_macro * ptr_macro;
typedef struct node_macro {
    char name_macro[MAX_ASSEMBLY_LINE_LENGTH];
    char text_macro[MAX_CHARACTERS_ASSEMBLY_FILE];
    ptr_macro next;
} item_macro;

/*
 * Function: add_to_list_macro
 * ---------------------------
 * Adds a new macro node to the linked list of macros, given the 'head' pointer
 * to the start of the list, 'name', and 'text' of the macro.
 *
 * Parameters:
 *   head (ptr_macro*): A pointer to the head of the linked list of macros.
 *   name (const char*): The name of the new macro to be added.
 *   text (const char*): The text or content associated with the macro.
 *
 * Returns:
 *   error_code: An error code indicating the status of the operation. Possible values are:
 *               - NO_ERROR: The macro was successfully added to the list.
 *               - MACRO_ALREADY_EXISTS: A macro with the same name already exists in the list.
 *
 * Notes:
 *   - The function first creates a new macro node using the provided 'name' and 'text' using
 *     the 'create_node_macro' function.
 *   - If the linked list is empty (head is NULL), the new node becomes the head of the list.
 *   - If a macro with the same name already exists in the list, the new node is not added, and
 *     the function returns an error code (MACRO_ALREADY_EXISTS).
 *   - If the list is not empty and the macro with the given 'name' doesn't exist, the new node
 *     is appended to the end of the list.
 *   - The function returns an error code to indicate the success or failure of the operation.
 */
error_code add_to_list_macro(ptr_macro *head, const char* name, const char* text);

/*
 * Function: search_in_list_macro
 * -----------------------------
 * Searches for a macro node with the given 'name_macro' in the linked list starting from 'head_macro'.
 * If a macro node with the matching name is found, a pointer to that node is returned; otherwise, NULL is returned.
 *
 * Parameters:
 *   head_macro (ptr_macro): A pointer to the head of the linked list (the first macro node).
 *   name_macro (const char*): The name of the macro to search for in the linked list.
 *
 * Returns:
 *   ptr_macro: A pointer to the found macro node if it exists, or NULL if the macro with the specified name is not found.
 *
 * Notes:
 *   - This function iterates through the linked list starting from 'head_macro' and compares the 'name_macro' of each node
 *     with the provided 'name_macro' using the 'strcmp' function.
 *   - If a macro node with the matching name is found, the function returns a pointer to that node.
 *   - If the macro with the given 'name_macro' is not found in the linked list, the function returns NULL.
 */
ptr_macro search_in_list_macro(ptr_macro head_macro, const char *name_macro);

/*
 * Function: free_list_macro
 * -------------------------
 * Frees the memory occupied by all nodes in the linked list starting from 'head'.
 * After calling this function, the linked list will be empty, and the 'head' pointer will be set to NULL.
 *
 * Parameters:
 *   head (ptr_macro*): A pointer to a pointer to the head of the linked list (the first macro node).
 *                      This allows the function to update the 'head' pointer to NULL after freeing all nodes.
 *
 * Notes:
 *   - This function iteratively traverses the linked list starting from 'head', freeing the memory allocated
 *     for each macro node.
 *   - For each node, the function saves the next node's address, updates the 'head' pointer to point to the next node,
 *     and then frees the memory occupied by the current node using the 'free' function.
 *   - The function continues this process until the end of the linked list is reached (when 'head' becomes NULL).
 *   - After freeing all nodes, the 'head' pointer is set to NULL to indicate that the linked list is empty.
 *   - If 'head' is initially NULL (an empty list), this function does nothing, and there is no memory leak.
 */
void free_list_macro(ptr_macro *head);

/*
 * Function: print_list_macro
 * --------------------------
 * (For Debugging) Prints the names and corresponding text of each macro node in the linked list starting from 'head'.
 *
 * Parameters:
 *   head (ptr_macro): The head of the linked list (the first macro node).
 *
 * Returns:
 *   void
 *
 * Notes:
 *   - This function is intended for debugging purposes.
 *   - It iteratively traverses the linked list starting from 'head'.
 *   - For each macro node, it prints the name and text of the macro using the 'printf' function.
 *   - If the linked list is empty (i.e., 'head' is NULL), nothing will be printed.
 *   - The '__attribute__((unused))' is a compiler directive that suppresses unused variable warnings
 *     for the 'print_list_macro' function, as the 'head' parameter might not be used in the function body.
 *     The attribute ensures that the function can still be compiled without warnings even if 'head' is not used.
 */
__attribute__((unused)) void print_list_macro(ptr_macro head);

#endif /* MACRO_LIST_H */