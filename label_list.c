#include "label_list.h"

/* Function: create_node_label
 * ---------------------------
 * Creates a new label node and initializes its fields.
 *
 * This function dynamically allocates memory for a new label node and sets its fields to the provided values.
 * The 'name' string is copied to the 'name_label' field of the label node, and the 'address' and 'type' fields
 * are set accordingly. The 'next' pointer of the label node is initialized to NULL.
 *
 * Parameters:
 *   - name: A pointer to a character array (string) representing the name of the label.
 *   - address: An integer representing the address associated with the label.
 *   - type: An enumerated type representing the type of the label (DATA, CODE, EXTERN, or ENTRY).
 *
 * Returns:
 *   - ptr_label: A pointer to the newly created label node.
 *
 * Remarks:
 *   - The function allocates memory for the label node using dynamic memory allocation (malloc).
 *   - The caller is also responsible for managing the memory of the created label node and freeing it when no longer needed.
 *   - If dynamic memory allocation fails, the function prints an error message to stderr and exits the program.
 *     Memory allocation failure may occur if there is insufficient memory available to create the label node.
 */
static ptr_label create_node_label(const char* name, int address, type_of_label type);

/* Function: get_type_as_text
 * --------------------------
 * Get a textual representation of the type of label.
 *
 * This function takes a label type of enumeration 'type_of_label' and returns a corresponding textual representation as a C-style
 * string. The textual representation is used for printing or displaying the label type in a human-readable format.
 *
 * Parameters:
 *   - type: The type of the label (enum 'type_of_label') for which the textual representation is required.
 *
 * Returns:
 *   - A C-style string representing the label type. The possible return values are "DATA", "CODE", "EXTERN", "ENTRY", or "NULL".
 */
static char * get_type_as_text(type_of_label type);

error_code add_to_list_label(ptr_label *head, const char* name, int address, type_of_label type) {
    /* Create a new label node using the provided name, address, and type. */
    ptr_label new_node = create_node_label(name, address, type);

    /* If the list is initially empty, the new node becomes the head of the list. */
    if (!(*head)){
        *head = new_node;
        return NO_ERROR;

    /* If the list is not empty, traverse the list to check for an existing node with the same name. */
    } else {
        ptr_label temp_node = *head;
        while(temp_node->next != NULL && strcmp(temp_node->name_label,name) != 0){
            temp_node = temp_node->next;
        }

        /* An existing node with the same name was found, update its address and type. */
        if (strcmp(temp_node->name_label,name) == 0){
            /* Free the memory allocated for the new node to avoid duplicates. */
            free(new_node);

            /* Return an error code to indicate that the label already exists. */
            return LABEL_ALREADY_EXISTS;
        } else {

            /* Attach the new node at the end of the list. */
            temp_node->next = new_node;

            /* Return a success code to indicate that the label was added to the list. */
            return NO_ERROR;
        }
    }
}

static ptr_label create_node_label(const char* name, int address, type_of_label type) {
    /* Dynamically allocate memory for the new label node. */
    ptr_label new_node = (ptr_label)malloc(sizeof(item_label));

    /* Check if memory allocation was successful. */
    if (new_node == NULL){
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Copy the provided 'name' string to the 'name_label' field of the label node. */
    strcpy(new_node->name_label, name);

    /* Set the 'address_label' and 'type' fields of the label node. */
    new_node->address_label = address;
    new_node->type = type;

    /* Initialize the 'next' pointer of the label node to NULL. */
    new_node->next = NULL;

    /* Return a pointer to the newly created label node. */
    return new_node;
}

void update_address_of_data(ptr_label head_label, int ic){
    /* Create a temporary pointer to traverse the label list, starting from the head. */
    ptr_label temp_head = head_label;

    /* Iterate through the label list until the end is reached (temp_head becomes NULL). */
    while (temp_head){
        /* Check if the current label node is of type DATA. */
        if (temp_head->type == DATA){
            /* If the label is of type DATA, update its address by adding the 'ic' value. */
            temp_head->address_label += ic;
        }
        /* Move to the next label node in the list. */
        temp_head = temp_head->next;
    }
}

ptr_label search_in_list_label(ptr_label head_label, const char *name_label){
    /* Create a temporary pointer to traverse the label list, starting from the head. */
    ptr_label temp_head = head_label;

    /* Iterate through the label list until the end is reached (temp_head becomes NULL). */
    while(temp_head){
        /* Compare the 'name_label' parameter with the 'name_label' field of the current label node. */
        if (strcmp(name_label,temp_head->name_label) == 0){
            /* If a matching label is found, return a pointer to that label node. */
            return temp_head;
        }
        /* Move to the next label node in the list. */
        temp_head = temp_head->next;
    }
    /* If no matching label is found in the list, return NULL. */
    return NULL;
}

error_code mark_label_as_entry(ptr_label head_label, const char *name_label){
    /* Create a temporary pointer to traverse the label list, starting from the head. */
    ptr_label temp_head = head_label;

    /* Iterate through the label list until the end is reached (temp_head becomes NULL). */
    while(temp_head){
        /* Compare the 'name_label' parameter with the 'name_label' field of the current label node. */
        if (strcmp(name_label,temp_head->name_label) == 0){
            /* If a matching label is found, mark it as an ENTRY type and return NO_ERROR. */
            temp_head->type = ENTRY;
            return NO_ERROR;
        }
        /* Move to the next label node in the list. */
        temp_head = temp_head->next;
    }
    /* If no matching label is found in the list, return CANT_FIND_LABEL_TO_ENTRY. */
    return CANT_FIND_LABEL_TO_ENTRY;
}

char *get_entry_list(ptr_label head_label) {
    char *entry_list;
    /* Create a buffer to hold the address converted to a string. */
    char int_address[MAX_DIGITS_FOR_ADDRESS];

    /* Initialize the size of the entry list to 0. */
    size_t entry_list_size = 0;

    /* Create a temporary pointer to traverse the label list, starting from the head. */
    ptr_label temp_node = head_label;

    /* Calculate the required memory size to store the entry list. */
    while (temp_node) {
        /* Check if the label node's type is ENTRY. */
        if (temp_node->type == ENTRY) {
            /* Calculate the size needed for the label name, address, and formatting characters. */
            entry_list_size += strlen(temp_node->name_label) + MAX_DIGITS_FOR_ADDRESS + 3;
        }
        /* Move to the next label node in the list. */
        temp_node = temp_node->next;
    }

    /* Allocate memory for the entry list string based on the calculated size. */
    entry_list = (char *)malloc(entry_list_size + 1);
    if (entry_list == NULL){
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Initialize the entry list string as an empty string. */
    entry_list[0] = '\0';

    /* Reset the temporary pointer to the head of the label list for traversal. */
    temp_node = head_label;

    /* Build the formatted entry list string by appending label names and addresses. */
    while (temp_node) {
        /* Check if the label node's type is ENTRY. */
        if (temp_node->type == ENTRY) {
            /* Convert the address to a string representation. */
            sprintf(int_address, "%d", temp_node->address_label);

            /* Concatenate the label name, tab character, address, and newline characters to the entry list string. */
            strcat(entry_list, temp_node->name_label);
            strcat(entry_list, "\t");
            strcat(entry_list, int_address);
            strcat(entry_list, "\n");
        }
        /* Move to the next label node in the list. */
        temp_node = temp_node->next;
    }
    /* Return the generated entry list string. */
    return entry_list;
}

void free_list_label(ptr_label *head){
    /* Create a temporary pointer to traverse the label list. */
    ptr_label temp_head;

    /* Iterate through the label list and free the memory for each label node. */
    while (*head){
        /* Store the next node's address in 'temp_head'. */
        temp_head = *head;

        /* Move 'head' to the next node in the list. */
        *head = (*head)->next;

        /* Free the memory allocated for the current label node. */
        free(temp_head);
    }
    /* Set the 'head' pointer to NULL, indicating an empty label list. */
    *head = NULL;
}

__attribute__((unused)) void print_list_label(ptr_label head){
    ptr_label temp_head = head;
    while(temp_head){
        printf("Name: %s\tAddress: %d\tType: %s\n",temp_head->name_label, temp_head->address_label,get_type_as_text(temp_head->type));
        temp_head = temp_head->next;
    }
}

static char * get_type_as_text(type_of_label type){
    switch (type) {
        case DATA:
            return "DATA";
        case CODE:
            return "CODE";
        case EXTERN:
            return "EXTERN";
        case ENTRY:
            return "ENTRY";
    }
    return "NULL";
}