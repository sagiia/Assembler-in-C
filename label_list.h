/*
 * Header: label_list.h
 * --------------------
 * This is the header file for managing a linked list of labels in an assembly code.
 *
 * The "label_list.h" header file provides function prototypes and data structures to manage a linked list of labels. Labels are
 * essential components of assembly code, representing specific memory addresses or symbols within the program. The header file defines
 * the 'type_of_label' enumeration, which categorizes the types of labels (DATA, CODE, EXTERN, or ENTRY). It also defines the 'item_label'
 * structure, representing a single label node in the linked list, and the 'ptr_label' type, which is a pointer to a label node.
 *
 * Included Files:
 *   - stdlib.h: Standard Library. It provides functions for memory allocation, conversion, and other utility functions.
 *   - stdio.h: Standard Input/Output library. It provides functions for input and output operations, such as file handling.
 *   - string.h: C String Library. It provides functions for manipulating strings, such as string copying and comparison.
 *   - error_tool.h: Contains functions for error handling and printing error messages during the pre-assembly process.
 *   - setting.h: Contains constant definitions and configurations used in the pre-assembly process.
 */

#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error_tool.h"
#include "setting.h"

/*
 * Enum: type_of_label
 * -------------------
 * An enumeration representing the types of labels in the assembly code.
 *
 * Enum Values:
 *   - DATA: Represents a label that defines a data segment in the assembly code. Data labels are used to declare and initialize data
 *           values and variables.
 *   - CODE: Represents a label that defines a code segment in the assembly code. Code labels are used to mark specific locations in
 *           the program's instructions.
 *   - EXTERN: Represents a label that refers to a symbol defined in a different source file. External labels are used to reference
 *             symbols from other modules or files.
 *   - ENTRY: Represents a label that is marked as an entry point in the assembly code. Entry labels are used to indicate the starting
 *            point of the program execution.
 */
typedef enum {DATA, CODE, EXTERN, ENTRY} type_of_label;

/*
 * Struct: item_label
 * ------------------
 * A structure representing a label node in the label list.
 *
 * This structure defines a node in the label list, containing information about a specific label, including its 'name_label', 'address_label',
 * 'type', and a pointer 'next' to the next label node in the list. Each node represents a label and serves as a building block of the linked list.
 * The linked list of 'item_label' nodes forms the label list, which organizes and manages the labels used in the assembly code.
 *
 * Fields:
 *   - name_label: An array of characters (string) representing the name of the label. The 'name_label' field should adhere to the specific
 *                 naming rules for assembly language labels and should be null-terminated.
 *   - address_label: An integer representing the memory address associated with the label.
 *   - type: A 'type_of_label' enum value representing the type of the label (DATA, CODE, EXTERN, or ENTRY). The 'type' field categorizes
 *           the label and provides information about its role in the assembly code.
 *   - next: A pointer to the next 'item_label' node in the label list. The 'next' pointer connects the label nodes in the linked list,
 *           forming a sequence of labels.
 */
typedef struct node_label * ptr_label;
typedef struct node_label {
    char name_label[MAX_NAME_LABEL_LENGTH];
    int address_label;
    type_of_label type;
    ptr_label next;
} item_label;

/*
 * Function: add_to_list_label
 * ---------------------------
 * Adds a new label node to the label list.
 *
 * This function adds a new label node with the specified 'name', 'address', and 'type' to the label list. The label list is
 * represented by a linked list of 'ptr_label' nodes, where each node contains information about a label, including its name,
 * address, and type (DATA, CODE, EXTERN, or ENTRY). If a node with the same 'name' already exists in the list, the function
 * does nothing and returns an error.
 *
 * Parameters:
 *   - head: A pointer to the pointer of the head node of the label list. The function uses a double pointer to modify the
 *           'head' pointer itself when adding the first node to an empty list.
 *   - name: A string representing the name of the label to be added or updated.
 *   - address: The integer value representing the address associated with the label.
 *   - type: The type_of_label enum value representing the type of the label (DATA, CODE, EXTERN, or ENTRY).
 *
 * Returns:
 *   - error_code: An error code indicating the outcome of the function call.
 *     - NO_ERROR: The label was successfully added or updated in the list.
 *     - LABEL_ALREADY_EXISTS: A node with the same 'name' already exists in the list, and the list was not modified.
 *
 * Notes:
 *   - The function first calls 'create_node_label' to create a new label node with the specified 'name', 'address', and 'type'.
 *   - If the 'head' pointer is initially NULL (empty list), the new node becomes the head of the list.
 *   - If the list is not empty, the function traverses the list to check if a node with the same 'name' already exists.
 *   - If a matching node is found, the function frees the memory allocated for the new node to avoid duplicates and returns
 *     the 'LABEL_ALREADY_EXISTS' error code.
 *   - If no matching node is found, the new node is attached at the end of the list.
 */
error_code add_to_list_label(ptr_label *head, const char* name, int address, type_of_label type);

/* Function: update_address_of_data
 * --------------------------------
 * Updates the addresses of data labels in the label list by adding a given offset.
 *
 * This function is used to update the memory addresses of data labels in the label list. It iterates through the label list
 * starting from the 'head_label' and checks the 'type' field of each label node. If the label is of type DATA, the function adds
 * the provided 'ic' (instruction counter) value to its current address. This is typically done after the first pass of an assembly
 * process, where the addresses of data labels are determined based on the location counter (IC) at that point in the assembly code.
 *
 * Parameters:
 *   - head_label: A pointer to the head of the label list. The 'head_label' should point to the first node of the label list, and
 *                 the function iterates through the list from this starting point.
 *   - ic: An integer value representing the instruction counter. The 'ic' value is used to update the addresses of data labels in
 *         the label list.
 */
void update_address_of_data(ptr_label head_label, int ic);

/* Function: search_in_list_label
 * ------------------------------
 * Search for a label node in the label list based on its name.
 *
 * Parameters:
 *   - head_label: A pointer to the head of the label list. The 'head_label' should point to the first node of the label list, and
 *                 the function iterates through the list from this starting point.
 *   - name_label: A pointer to a constant character string representing the name of the label to search for. The 'name_label'
 *                 parameter specifies the label's name to be found in the list.
 *
 * Returns:
 *   - If a matching label node is found in the label list, a pointer to that label node is returned.
 *   - If no matching label is found in the list, the function returns NULL.
 *
 * Notes:
 *   - If a label with a matching name is found, a pointer to that label node is returned immediately, and the function exits.
 *   - If no matching label is found after iterating through the entire list, the function returns NULL to indicate that the label
 *     was not found in the list.
 *   - This function is useful for searching for a specific label in the list, for example, during the second pass of an assembly
 *     process, where references to labels need to be resolved to their memory addresses.
 */
ptr_label search_in_list_label(ptr_label head_label, const char *name_label);

/* Function: mark_label_as_entry
 * -----------------------------
 * Mark a label node as an ENTRY type in the label list.
 *
 * Parameters:
 *   - head_label: A pointer to the head of the label list. The 'head_label' should point to the first node of the label list, and
 *                 the function iterates through the list from this starting point.
 *   - name_label: A pointer to a constant character string representing the name of the label to mark as ENTRY. The 'name_label'
 *                 parameter specifies the label's name to be marked as an ENTRY type.
 *
 * Returns:
 *   - If a matching label node is found in the label list, its 'type' field is updated to ENTRY, and the function returns 'NO_ERROR'.
 *   - If no matching label is found in the list, the function returns 'CANT_FIND_LABEL_TO_ENTRY' to indicate that the specified label
 *     was not found and, therefore, could not be marked as an ENTRY type.
 *
 * Notes:
 *   - For each label node in the list, the function compares the 'name_label' parameter with the 'name_label' field of the current
 *     label node using the 'strcmp' function from the C standard library.
 *   - If a label with a matching name is found, its 'type' field is updated to ENTRY, indicating that it is now an ENTRY type label.
 *   - If no matching label is found after iterating through the entire list, the function returns 'CANT_FIND_LABEL_TO_ENTRY' to
 *     indicate that the specified label was not found and, therefore, could not be marked as an ENTRY type.
 */
error_code mark_label_as_entry(ptr_label head_label, const char *name_label);

/* Function: get_entry_list
 * ------------------------
 * Generate a list of ENTRY type labels and their corresponding addresses.
 *
 * This function traverses the label list starting from the 'head_label' and generates a formatted string containing the names
 * and addresses of all the labels marked as ENTRY type in the list. The function calculates the required memory size to store the
 * list and allocates memory dynamically to hold the generated string. The generated string is formatted as follows:
 *
 *   "<label_name_1>\t<address_1>\n"
 *   "<label_name_2>\t<address_2>\n"
 *   ...
 *
 * Each line represents an ENTRY type label and its associated address separated by a tab character. The '\n' character is used to
 * separate each label-entry pair and create a newline in the final string. The memory allocated for the string should be freed by
 * the caller when it is no longer needed.
 *
 * Parameters:
 *   - head_label: A pointer to the head of the label list. The 'head_label' should point to the first node of the label list, and
 *                 the function iterates through the list from this starting point.
 *
 * Returns:
 *   - If there are ENTRY type labels in the label list, this function returns a dynamically allocated character string containing
 *     the list of ENTRY labels and their corresponding addresses in the specified format. The caller is responsible for freeing
 *     the allocated memory when it is no longer needed.
 *   - If there are no ENTRY type labels in the label list or the label list is empty, the function returns NULL.
 *
 * Notes:
 *   - The function iterates through the label list, starting from the 'head_label', to calculate the required memory size for the
 *     generated list.
 *   - For each label node in the list, the function checks if the 'type' field is set to ENTRY. If so, it calculates the size needed
 *     to store the label name, address, and formatting characters.
 *   - After allocating memory, the function iterates through the label list again, starting from the 'head_label', to build the
 *     formatted string by appending each ENTRY label's name and address to the string.
 */
char * get_entry_list(ptr_label head);

/* Function: free_list_label
 * -------------------------
 * Free the memory occupied by the label list.
 *
 * Parameters:
 *   - head: A pointer to the head of the label list. The 'head' should point to the first node of the label list to begin freeing
 *           the memory.
 *
 * Notes:
 *   - This function is designed to work with a linked list of labels, where each label node is of type 'item_label' (ptr_label).
 *   - The function assumes that the 'head' pointer points to a valid label list. If 'head' is NULL or points to an empty list, the
 *     function has no effect.
 */
void free_list_label(ptr_label *head);

/* Function: print_list_label
 * --------------------------
 * (For Debugging) Print the list of labels with their details.
 *
 * This function prints the details of the labels in the linked list, starting from the specified head node. The details include the
 * name, address, and type of each label in the list. The label type is represented as a textual string using the 'get_type_as_text'
 * function.
 *
 * Parameters:
 *   - head: The head node of the linked list containing the labels.
 *
 * Remarks:
 *   - The function is marked with the '__attribute__((unused))' attribute, which tells the compiler not to produce a warning if the
 *     function is unused (not called) in the code.
 *   - The 'print_list_label' function is intended for debugging or informational purposes when you want to see the details of the
 *     labels in the linked list. It does not return any values; it only prints the information to the standard output (console).
 */
__attribute__((unused)) void print_list_label(ptr_label head);

#endif /* LABEL_LIST_H */