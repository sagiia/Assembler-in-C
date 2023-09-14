#include "macro_list.h"

/*
 * Function: create_node_macro
 * ---------------------------
 * Creates a new macro node with the provided 'name' and 'text' and returns a pointer to the newly
 * allocated node.
 *
 * Parameters:
 *   name (const char*): The name to be assigned to the new macro node.
 *   text (const char*): The text or content to be assigned to the new macro node.
 *
 * Returns:
 *   ptr_macro: A pointer to the newly created macro node.
 *
 * Notes:
 *   - The function dynamically allocates memory for the new macro node using 'malloc'.
 *   - The 'name' and 'text' provided are copied to the respective fields of the newly created node.
 *   - The 'next' pointer of the node is initialized to NULL as it is not yet linked to any other nodes.
 */
static ptr_macro create_node_macro(const char* name, const char* text);

error_code add_to_list_macro(ptr_macro *head, const char* name, const char* text) {
    /* Create a new macro node using the provided 'name' and 'text'. */
    ptr_macro new_node = create_node_macro(name, text);

    /* Check if the linked list is empty (head is NULL). */
    if (!(*head)){
        /* If the list is empty, set the new node as the head of the list. */
        *head = new_node;
        return NO_ERROR;
    } else {
        /* If the list is not empty, traverse the list to check if a macro with the same name already exists. */
        ptr_macro  temp_node = *head;
        while(temp_node->next != NULL && strcmp(temp_node->name_macro,name) != 0){
            temp_node = temp_node->next;
        }

        /* If a macro with the same name already exists, free the new node and return an error code. */
        if (strcmp(temp_node->name_macro,name) == 0){
            free(new_node);
            return MACRO_ALREADY_EXISTS;
        } else {
            /* If the macro with the given 'name' doesn't exist, add the new node to the end of the list. */
            temp_node->next = new_node;
            return NO_ERROR;
        }
    }
}

static ptr_macro create_node_macro(const char* name, const char* text) {
    /* Allocate memory for the new macro node. */
    ptr_macro new_node = (ptr_macro)malloc(sizeof(item_macro));

    /* Check if memory allocation was successful. */
    if (new_node == NULL){
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Copy the provided 'name' and 'text' to the respective fields of the macro node. */
    strcpy(new_node->name_macro, name);
    strcpy(new_node->text_macro, text);

    /* Initialize the 'next' pointer of the macro node to NULL. */
    new_node->next = NULL;

    /* Return a pointer to the newly created macro node. */
    return new_node;
}

ptr_macro search_in_list_macro(ptr_macro head_macro, const char *name_macro) {
    /* Start the search from the head of the linked list. */
    ptr_macro temp_head = head_macro;

    /* Traverse the linked list until the end or until a matching macro node is found. */
    while (temp_head) {
        /* Compare the 'name_macro' of the current node with the provided 'name_macro'. */
        if (strcmp(name_macro, temp_head->name_macro) == 0) {
            /* If a matching macro node is found, return a pointer to that node. */
            return temp_head;
        }
        /* Move to the next node in the linked list. */
        temp_head = temp_head->next;
    }
    /* If the macro with the specified name is not found, return NULL. */
    return NULL;
}

void free_list_macro(ptr_macro *head){
    ptr_macro temp_head;

    /* Traverse the linked list and free memory occupied by each node. */
    while (*head){
        /* Save the address of the next node. */
        temp_head = *head;

        /* Update 'head' to point to the next node in the linked list. */
        *head = (*head)->next;

        /* Free the memory occupied by the current node. */
        free(temp_head);
    }
    /* After freeing all nodes, set the 'head' pointer to NULL to indicate an empty list. */
    *head = NULL;
}

__attribute__((unused)) void print_list_macro(ptr_macro head){
    ptr_macro temp_head = head;

    /* Traverse the linked list and print the name and text of each macro node. */
    while (temp_head){
        printf("Name: %s\tText of macro: %s\n",temp_head->name_macro, temp_head->text_macro);
        temp_head = temp_head->next;
    }
}