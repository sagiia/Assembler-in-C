#include "second_pass.h"

/*
 * Static variable: sfile
 * ----------------------
 * A pointer to the 'file' struct representing the current assembly source file being processed in
 * the second pass stage.
 */
static ptr_file sfile;

/*
 * Static variable: extern_list
 * -------------------------------
 * A character array to store a list of external labels that need to be resolved by the linker in the second pass of the assembly process.
 */
static char extern_list[MAX_CHARACTERS_ASSEMBLY_FILE];

/*
 * Function: second_pass_on_curr_file
 * ----------------------------------
 * Perform the second pass on the current assembly source file.
 *
 * This function is responsible for executing the second pass of the assembly process on the provided assembly source file.
 * It reads the file line by line, skipping empty lines and processing valid lines containing assembly instructions and data.
 * The second pass handles instructions and data sections, generates the final machine code, and creates the output file with
 * the assembled machine code and the list of external labels encountered during the pass.
 *
 * Parameters:
 *   - original_file_struct (ptr_file): A pointer to the 'file' struct representing the current assembly source file from the first pass.
 *                                      The 'original_file_struct' contains information about the source file, such as file streams,
 *                                      line structures, and the linked list of labels defined in the first pass.
 *
 * Notes:
 *   - The function initializes the 'extern_list' array as an empty string, which will be used to store the list of external labels
 *     encountered during the pass, if any.
 *   - The function sets the 'IC' (Instruction Counter) to the value of 'FIRST_CELL_IN_MEMORY', indicating the starting memory
 *     address for the assembled instructions.
 */
static void second_pass_on_curr_file();

/*
 * Function: update_files
 * ----------------------
 * Update file streams for the second pass.
 *
 * This function updates the file streams used in the second pass. It is responsible for resetting the position indicator
 * of the assembly source file back to the beginning of the file using the 'rewind' function. By doing so, it allows the
 * second pass to read the assembly source file from the beginning and perform assembly operations on it without having
 * to open and close the file again.
 *
 * Notes:
 *   - The function operates on the 'sfile' global variable, which represents the current file being processed in the second pass.
 */
static void update_files();

/*
 * Function: update_next_line
 * --------------------------
 * Read the next line from the assembly source file and store it in the 'line_text' buffer.
 *
 * This function is responsible for reading the next line from the assembly source file represented by the file stream
 * 'sfile->file_am'. It uses the 'fgets' function to read a line of text from the file and stores it in the 'line_text'
 * buffer of the 'sfile' global variable. The 'memset' function is used to clear the buffer before reading the new line
 * to ensure that any previous content is removed.
 *
 * Returns:
 *   char *: A pointer to the 'line_text' buffer, which contains the text of the next line read from the file.
 *           The caller can use this pointer to process the line further.
 */
static char * update_next_line();

/*
 * Function: update_line_to_array
 * ------------------------------
 * Create a new line structure and update 'sfile->line_struct' with it.
 *
 * This function is responsible for creating a new line structure from the contents of the current assembly source line
 * stored in 'sfile->line_text'. The line structure is created using the 'create_new_line_struct' function, which allocates
 * memory and stores the line information in a structured format.
 *
 * After creating the line structure, this function updates the 'sfile->line_struct' pointer to point to the newly created
 * structure, effectively storing the line information for the current line of the assembly source file. The 'sfile' global
 * variable represents the current file being processed in the second pass.
 */
static void update_line_to_array();

/*
 * Function: skip_on_label
 * -----------------------
 * Skip the label if the current line contains one.
 *
 * This function is responsible for handling lines that contain a label in the assembly source file during the second pass.
 * If the current assembly source line has a label, this function skips the label by updating the 'sfile->pos_in_line' to point
 * to the next word after the label. It then deletes the label from the line structure, removing it from further processing.
 *
 * The 'sfile' global variable represents the current file being processed in the second pass. The 'sfile->line_text' global
 * variable holds the contents of the current line of the assembly source file, and 'sfile->line_struct' points to the line's
 * structured representation, which includes the label information.
 *
 * Note:
 *   - is_label: Function to check if the current line contains a label.
 *   - delete_label_from_line_struct: Function to delete the label information from the line structure.
 */
static void skip_on_label();

/*
 * Function: get_word_status
 * -------------------------
 * Determine the status of the given word in the assembly source code.
 *
 * This function is used during the second pass of the assembly process to determine the status of a word in the assembly
 * source code. The word's status helps identify whether the word corresponds to a specific directive (e.g., .data, .string)
 * or operation code (e.g., MOV, ADD, SUB) in the assembly language.
 *
 * Parameters:
 *   word_text (char*): The word to be analyzed for its status.
 *
 * Returns:
 *   line_status: The status of the word, which represents a specific directive or operation code.
 *
 * Note:
 *   - line_status: Enum defining the possible values for the status of a word in the assembly source code.
 */
static line_status get_word_status(char *word_text);

/*
 * Function: action_by_status
 * --------------------------
 * Perform actions based on the status of the assembly source code line.
 *
 * This function is called during the second pass of the assembly process to perform specific actions based on the status
 * of the assembly source code line. The status is determined by the 'get_word_status' function, which categorizes the line
 * into different types, such as directives (e.g., .data, .string, .extern) or operation codes (e.g., MOV, ADD, SUB).
 *
 * Parameters:
 *   status (line_status): The status of the current assembly source code line, representing a specific directive or operation code.
 *
 * Note:
 *   - The 'action_by_status' function is called for each line of the assembly source code during the second pass.
 *   - Depending on the line's status, it may perform different actions, such as marking entry labels or completing missing
 *     instructions.
 *   - If the status corresponds to .data, .string, or .extern directives, no further action is taken for that line, as these
 *     directives don't require any additional processing during the second pass.
 *   - For the STATUS_ENTRY status, the function calls 'mark_entry_labels' to handle entry label processing, marking the
 *     corresponding labels in the symbol table.
 *   - For the STATUS_CODE status, the function calls 'complete_missing_instructions' to handle instruction processing,
 *     completing any missing instructions and resolving their addressing methods.
 */
static void action_by_status(line_status status);

/*
 * Function: add_error
 * -------------------
 * Add an error to the error list and set the error flag.
 *
 * This function is used to add an error to the error list during the second pass of the assembly process. It takes an error
 * code as input and prints the corresponding error message along with the line number where the error occurred. The function
 * then sets the error flag to indicate that errors have been encountered during the assembly process.
 *
 * Parameters:
 *   error_code (error_code): The error code representing the type of error encountered during the assembly process.
 *
 * Note:
 *   - The 'add_error' function is called whenever an error is detected during the second pass of the assembly process.
 *   - It prints the error message along with the line number where the error occurred using the 'print_error' function.
 *   - After adding the error to the list, it sets the 'error_flag' to TRUE to indicate the presence of errors.
 *   - The 'count_line' and 'count_error' members of 'sfile' (file_struct pointer) are updated accordingly to keep track of
 *     the line number and the total number of errors encountered.
 */
static void add_error(error_code error_code);

/*
 * Function: update_error_status
 * ----------------------------
 * Update the error status based on the error code.
 *
 * This function is used to update the error status during the second pass of the assembly process. It takes an error code
 * as input and checks whether it represents a 'NO_ERROR'. If the error code indicates an error, the function calls the
 * 'add_error' function to add the error to the error list and set the error flag.
 *
 * Parameters:
 *   error_code (error_code): The error code representing the type of error encountered during the assembly process.
 *
 * Note:
 *   - The 'update_error_status' function is called after processing a line during the second pass of the assembly process.
 *   - It checks whether the error code is 'NO_ERROR' to determine if there was any error in the line processing.
 *   - If the error code is not 'NO_ERROR', the function adds the error to the list and sets the 'error_flag' to TRUE.
 *   - The 'error_flag' indicates the presence of errors during the assembly process.
 */
static void update_error_status(error_code error_code);

/*
 * Function: mark_entry_labels
 * ---------------------------
 * Mark labels as entry points.
 *
 * This function is used to mark labels as entry points during the second pass of the assembly process. It is called
 * when encountering an entry directive in the assembly code. The function iterates through the label names provided in
 * the entry directive and marks them as entry points in the label list.
 *
 * Note:
 *   - The 'mark_entry_labels' function is called when an entry directive is encountered during the second pass of the
 *     assembly process.
 *   - It sets the 'entry_flag' to TRUE to indicate that the entry directive has been encountered in the current line.
 *   - The 'entry_flag' is used to handle multiple entry directives in the assembly code.
 *   - The function extracts the label names from the entry directive and marks them as entry points in the label list.
 *   - It checks the validity of each label name and updates the error status accordingly.
 *   - If the entry directive contains an invalid label name or other errors are encountered, the function adds the
 *     errors to the error list and sets the error flag.
 */
static void mark_entry_labels();

/*
 * Function: complete_missing_instructions
 * ---------------------------------------
 * Complete missing instructions during the second pass of the assembly process.
 *
 * This function is called during the second pass of the assembly process when an instruction with missing operands is
 * encountered. It completes the missing operands and generates the rest of the instruction to be added to the instruction
 * array.
 *
 * Note:
 *   - It determines the instruction type by extracting the first word from the line and looking it up in the instruction
 *     set.
 *   - The function updates the addressing method types for the source and destination operands based on the instruction
 *     type.
 *   - The 'IC' (Instruction Counter) is incremented to allocate memory for the instruction's first word in the
 *     instruction array.
 *   - The rest of the instruction is generated and added to the instruction array.
 *   - If there are any errors related to invalid addressing methods or missing operands, they are handled in the
 *     'update_the_rest_of_the_instruction_to_array' function.
 */
static void complete_missing_instructions();

/*
 * Function: update_addressing_method_type
 * ---------------------------------------
 * Update addressing method types for the source and destination operands based on the instruction type.
 *
 * This function is responsible for determining and updating the addressing method types for the source and destination
 * operands of an instruction based on the given instruction type.
 *
 * Parameters:
 *   type (instruction_type): The type of instruction to be processed.
 *
 * Note:
 *   - The 'update_addressing_method_type' function is called during the second pass of the assembly process.
 *   - The function takes the 'instruction_type' as a parameter, which is determined by the first word of the instruction.
 *   - Depending on the instruction type, the function updates the 'source' and 'destination' fields in the 'line_struct'
 *     with appropriate addressing method types.
 */
static void update_addressing_method_type(instruction_type type);

/*
 * Function: update_the_rest_of_the_instruction_to_array
 * -----------------------------------------------------
 * Update the rest of the instruction data into the instruction array during the second pass of the assembly process.
 *
 * This function is responsible for updating the rest of the instruction data into the instruction array during the
 * second pass of the assembly process. It handles different addressing methods for the source and destination operands,
 * retrieves label information from the symbol table, and generates the appropriate encoding for the instruction words.
 *
 * Note:
 *   - The 'update_the_rest_of_the_instruction_to_array' function is called after processing the first word of the
 *     instruction and updating the addressing method types for the source and destination operands using the
 *     'update_addressing_method_type' function.
 *   - The function processes the source and destination addressing methods separately.
 *   - For source addressing methods REGISTER and IMMEDIATE, the function increments the instruction counter ('IC') by one
 *     to accommodate the next instruction word.
 *   - For source addressing method DIRECT, the function searches for the corresponding label node in the symbol table
 *     and generates the appropriate instruction word for RELOCATABLE or EXTERNAL addressing methods.
 *   - The 'EXTERNS_LIST' array is updated with external labels encountered during the second pass for later use in the
 *     output files.
 *   - For destination addressing methods REGISTER and IMMEDIATE, the function increments the instruction counter ('IC')
 *     by one if the source addressing method is not REGISTER, as the destination word can be the last instruction word.
 *   - For destination addressing method DIRECT, the function searches for the corresponding label node in the symbol
 *     table and generates the appropriate instruction word for RELOCATABLE or EXTERNAL addressing methods.
 */
static void update_the_rest_of_the_instruction_to_array();

/*
 * Function: get_next_word_without_comma
 * -------------------------------------
 * Extract the next word from the line text without including the comma (if present).
 *
 * This function extracts the next word from the current line text, starting from the current position
 * indicated by 'pos_in_line'. It reads characters until it encounters a space, tab, newline, comma, or
 * the end of the line. The function stores the extracted word in the 'temp_word' buffer and increments
 * the 'pos_in_line' index accordingly.
 *
 * Returns:
 *   char*: A pointer to the 'temp_word' buffer containing the extracted word.
 *
 * Notes:
 *   - After extracting the word, the function skips any white characters to move to the next word or
 *     the end of the line.
 */
static char * get_next_word_without_comma();

/*
 * Function: check_for_comma
 * -------------------------
 * Check if the current character at the current position is a comma.
 *
 * This function checks if the character at the current position indicated by 'pos_in_line' in the
 * current line text is a comma. If a comma is found, the function increments 'pos_in_line' and returns
 * TRUE. If the current character is not a comma and not the end of the line, it adds an error message
 * indicating that a comma is required between values and returns FALSE.
 *
 * Returns:
 *   bool: TRUE if a comma is found at the current position; FALSE otherwise.
 */
static bool check_for_comma();

/*
 * Function: add_extern_label_to_array
 * ----------------------------------
 * This function adds the name and address of an external label to the 'extern_list' array. The 'extern_list' array
 * contains information about all external labels encountered during the second pass, and it will be used later to
 * generate the external file.
 *
 * Parameters:
 *   temp_node: A pointer to the label node representing the external label in the symbol table.
 *
 * Notes:
 *   - The 'extern_list' array is a global variable that stores the information of all external labels.
 *   - The function appends the name and address of the external label to the 'extern_list' array in a specific format,
 *     separated by a tab '\t' character and followed by a newline '\n' character to separate each label entry.
 *   - The address of the external label ('IC') is obtained from the 'sfile' global variable, which represents the
 *     current assembly file being processed.
 *   - The 'extern_list' array is later used to generate the external file in the second pass.
 */
static void add_extern_label_to_array(ptr_label temp_node);

/*
 * Function: create_all_files
 * -------------------------
 * This function is responsible for creating all the output files after completing the second pass of the assembler.
 * It generates the entry file (if needed) and the external file (if needed) and then proceeds to create the object file.
 *
 * Notes:
 *   - The function first checks if the 'entry_flag' is set to TRUE, indicating the presence of entry labels in the assembly code.
 *   - If entry labels are found, the function creates the entry file and writes the entry labels and their addresses to the file.
 *   - The entry labels and their addresses are obtained from the 'sfile' global variable, which represents the current assembly file being processed.
 *   - The 'get_entry_list' function is called to generate a formatted string containing the entry labels and addresses for writing to the entry file.
 *   - After writing the entry file, the temporary entry list string is freed to release memory resources.
 *   - Next, the function checks if the 'extern_flag' is set to TRUE, indicating the presence of external labels in the assembly code.
 *   - If external labels are found, the function creates the external file and writes the external labels and their addresses to the file.
 *   - The 'extern_list' array is used to store the information of all external labels encountered during the second pass.
 *   - The 'extern_list' array is a global variable defined outside the scope of this function.
 *   - After writing the external file, the function proceeds to create the object file by calling the 'create_object_file' function.
 */
static void create_all_files();

/*
 * Function: create_object_file
 * ---------------------------
 * This function is responsible for creating the object file after completing the second pass of the assembler.
 * It generates the object file by writing the instructions and data memory contents to the file in a specific format.
 *
 * Notes:
 *   - The function first opens the object file in write mode using the 'open_file' function.
 *   - It then writes the header of the object file, which consists of the value of (IC - FIRST_CELL_IN_MEMORY) and the value of DC.
 *   - The 'IC' (Instruction Counter) holds the number of instruction words (machine code) generated during the second pass.
 *   - The 'DC' (Data Counter) holds the number of data words generated during the second pass.
 *   - The instructions and data memory contents are stored in the 'instruction_array' and 'data_array', respectively.
 *   - The 'convert_binary_to_64base' function is used to convert each instruction or data word to a 64-base representation.
 *   - The converted 64-base strings are then written to the object file using the 'fputs' function.
 *   - After writing the content, the temporary word string is freed to release memory resources.
 *   - Finally, the object file is closed using the 'fclose' function.
 */
static void create_object_file();

void start_second_pass(ptr_file original_file_struct){
    sfile = original_file_struct;
    second_pass_on_curr_file();
}

static void second_pass_on_curr_file(){
    /* Open the intermediate files for writing. */
    update_files();

    sfile->count_line = 0;
    extern_list[0] = '\0';
    sfile->IC = FIRST_CELL_IN_MEMORY;

    /* Process each line in the current file. */
    while(update_next_line() != NULL) {
        /* Increment the count of processed lines. */
        (sfile->count_line)++;

        /* Skip processing if the line is empty. */
        if (is_line_empty(sfile->line_text) == TRUE){
            continue;
        }

        /* Initialize the line position for parsing. */
        sfile->pos_in_line = 0;

        /* Update the line data structure to store information about the current line. */
        update_line_to_array();

        /* Skip on label definitions, already processed in the first pass */
        skip_on_label();
        if (sfile->line_struct->count == 0){
            continue;
        }

        /* Determine the type of line being processed and take appropriate action */
        action_by_status(get_word_status(sfile->line_struct->word1));

        /* Free the memory occupied by the line data structure. */
        free_line(sfile->line_struct);
    }

    /* If no errors, create output files with assembled machine code and list of extern labels */
    if (sfile->error_flag == FALSE){
        create_all_files();
    }

    /* Free memory allocated for the list of labels to avoid memory leaks */
    free_list_label(&sfile->head_label_list);
}

static void update_files(){
    /* Rewind the assembly source file 'sfile->file_am' back to the beginning. */
    rewind(sfile->file_am);
}

static char * update_next_line(){
    /* Clear the 'line_text' buffer to ensure it's empty before reading the new line. */
    memset(sfile->line_text, 0, MAX_ASSEMBLY_LINE_LENGTH);

    /* Read the next line from the assembly source file into the 'line_text' buffer. */
    return fgets(sfile->line_text,sizeof (sfile->line_text),sfile->file_am);
}

static void update_line_to_array(){
    /* Create a new line structure from the current assembly source line 'sfile->line_text'. */
    sfile->line_struct = create_new_line_struct(sfile->line_text);
}

static void skip_on_label() {
    /* Check if the current line contains a label. */
    if (is_label(sfile->line_struct) == TRUE) {
        /* If a label is present, skip it by updating 'sfile->pos_in_line'. */
        sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);

        /* Delete the label from the line structure to avoid interference with further processing. */
        delete_label_from_line_struct(sfile->line_struct);
    }
}

static line_status get_word_status(char *word_text){
    /* Check if the word matches any known directives or operation codes. */
    if (strcmp(word_text, DOT_DATA) == 0){
        return STATUS_DATA;
    }
    if (strcmp(word_text, DOT_STRING) == 0){
        return STATUS_STRING;
    }
    if (strcmp(word_text, DOT_EXTERN) == 0){
        return STATUS_EXTERN;
    }
    if (strcmp(word_text, DOT_ENTRY) == 0){
        return STATUS_ENTRY;
    }
    /* If the word does not match any known directive or operation code, return STATUS_CODE. */
    return STATUS_CODE;
}

static void action_by_status(line_status status){
    /* Perform actions based on the status of the assembly source code line. */
    switch (status) {
    case STATUS_DATA: case STATUS_STRING: case STATUS_EXTERN:
            /* For .data, .string, and .extern directives, no further action is needed in the second pass. */
            return;
        case STATUS_ENTRY:
            /* Handle entry label processing by calling the 'mark_entry_labels' function. */
            mark_entry_labels();
            break;
        case STATUS_CODE:
            /* Handle instruction processing by calling the 'complete_missing_instructions' function. */
            complete_missing_instructions();
            break;
    }
}

static void add_error(error_code error_code){
    /* Print the error message along with the line number where the error occurred. */
    print_error(error_code,sfile->count_line);

    /* Set the error flag to indicate the presence of errors during the assembly process. */
    sfile->error_flag = TRUE;

    /* Increment the count of total errors encountered during the assembly process. */
    (sfile->count_error)++;
}

static void update_error_status(error_code error_code){
    /* Check if the error code represents 'NO_ERROR' (i.e., no error). */
    if (error_code != NO_ERROR){
        /* Add the error to the error list and set the error flag. */
        add_error(error_code);
    }
}

static void mark_entry_labels(){
    char temp_word[MAX_ASSEMBLY_LINE_LENGTH] = "";

    /* Set the 'entry_flag' to TRUE to indicate that the entry directive has been encountered in the current line. */
    sfile->entry_flag = TRUE;

    /* Skip the entry directive itself to extract the label names. */
    sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);

    /* Check if the entry directive provides any label names. */
    if (is_end_line(sfile->pos_in_line, sfile->line_text) == TRUE){
        /* If there are no label names, add an error indicating that label names must be provided. */
        add_error(MUST_PROVIDE_LABELS_TO_ENTRY);
        return;
    } else {
        /* Process each label name provided in the entry directive. */
        do {
            /* Skip any white characters to extract the label name. */
            sfile->pos_in_line = skip_white_character(sfile->pos_in_line, sfile->line_text);

            /* Check for an invalid comma position. */
            if (sfile->line_text[sfile->pos_in_line] == ','){
                add_error(INVALID_COMMA_POSITION);
                return;
            }

            /* Extract the label name without a comma. */
            strcpy(temp_word, get_next_word_without_comma()); /* */

            /* Check if the label name is valid and update the error status accordingly. */
            if (is_label_name_valid(temp_word) == TRUE){
                update_error_status(mark_label_as_entry(sfile->head_label_list, temp_word));
            } else {
                add_error(INVALID_LABEL_NAME);
            }
        } while (check_for_comma() == TRUE); /* Continue processing if there are more label names separated by commas. */
    }
}

static void complete_missing_instructions(){
    instruction_type type;

    /* Determine the instruction type by extracting the first word from the line and looking it up in the instruction set. */
    type = get_instruction_type(sfile->line_struct->word1);

    /* Update the addressing method types for the source and destination operands based on the instruction type. */
    update_addressing_method_type(type);

    /* Increment the 'IC' (Instruction Counter) to allocate memory for the instruction's first word in the instruction array. */
    (sfile->IC)++;

    /* Generate the rest of the instruction and add it to the instruction array. */
    update_the_rest_of_the_instruction_to_array();
}

static void update_addressing_method_type(instruction_type type){
    /* Check the type of instruction and update addressing method types accordingly */
    switch (type) {
        case MOV: case CMP: case ADD: case SUB: case LEA:
            /* For instructions with two operands, update the addressing method type for both source and destination. */
            sfile->line_struct->source = get_addressing_method_type(sfile->line_struct->word2);
            sfile->line_struct->destination = get_addressing_method_type(sfile->line_struct->word4);
            break;
        case NOT: case CLR: case INC: case DEC: case JMP: case BNE: case RED: case PRN: case JSR:
            /* For instructions with a single operand, update the destination field, and set the source field to NOT_EXIST. */
            sfile->line_struct->source = NOT_EXIST;
            sfile->line_struct->destination = get_addressing_method_type(sfile->line_struct->word2);

            /* Copy the operand to word4 since there's only one operand for these instructions. */
            strcpy(sfile->line_struct->word4, sfile->line_struct->word2);
            break;
        default:
            /* For instructions with no operands, set both source and destination fields to NOT_EXIST. */
            sfile->line_struct->source = NOT_EXIST;
            sfile->line_struct->destination = NOT_EXIST;
            break;
    }
}

static void update_the_rest_of_the_instruction_to_array(){
    ptr_label label_node;

    /* Process the source addressing method. */
    switch (sfile->line_struct->source) {
        case REGISTER: case IMMEDIATE:
            /* Increment the instruction counter ('IC') by one to accommodate the next instruction word. */
            (sfile->IC)++;
            break;
        case DIRECT:
        {
            /* For DIRECT addressing method, search for the corresponding label node in the symbol table. */
            label_node = search_in_list_label(sfile->head_label_list, sfile->line_struct->word2);
            if (label_node != NULL){
                /* Bit-field structure for source operand with direct addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int label_address:10; /* Value field for the address of the operand */
                } word_of_instruction;

                /* For external labels, set the encoding type to EXTERNAL and add the label to the extern_list array. */
                if (label_node->type == EXTERN){
                    word_of_instruction.encoding_type = EXTERNAL;
                    add_extern_label_to_array(label_node);
                } else {
                    /* For relocatable labels, set the encoding type to RELOCATABLE. */
                    word_of_instruction.encoding_type = RELOCATABLE;
                }
                /* Update the label's address in the instruction word. */
                word_of_instruction.label_address = label_node->address_label;

                /* Copy the instruction word to the instruction array and increment the instruction counter ('IC'). */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            } else {
                /* If the label node is not found in the symbol table, add an error for LABEL_NOT_FOUND. */
                add_error(LABEL_NOT_FOUND);
            }
        }
            break;
        case NOT_EXIST:
            break;
    }
    /* Process the destination addressing method. */
    switch (sfile->line_struct->destination) {
        /* Increment the instruction counter ('IC') by one to accommodate the next instruction word (if it is not done on the first pass). */
        case REGISTER:
            if (sfile->line_struct->source != REGISTER){
                (sfile->IC)++;
            }
            break;
            /* Increment the instruction counter ('IC') by one to accommodate the next instruction word. */
        case IMMEDIATE:
            (sfile->IC)++;
            break;
        case DIRECT:
        {
            /* For DIRECT addressing method, search for the corresponding label node in the symbol table. */
            label_node = search_in_list_label(sfile->head_label_list, sfile->line_struct->word4);
            if (label_node != NULL){
                /* Bit-field structure for destination operand with direct addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int label_address:10; /* Value field for the address of the operand */
                } word_of_instruction;

                /* For external labels, set the encoding type to EXTERNAL and add the label to the extern_list array. */
                if (label_node->type == EXTERN){
                    word_of_instruction.encoding_type = EXTERNAL;
                    add_extern_label_to_array(label_node);
                } else {
                    /* For relocatable labels, set the encoding type to RELOCATABLE. */
                    word_of_instruction.encoding_type = RELOCATABLE;
                }
                /* Update the label's address in the instruction word. */
                word_of_instruction.label_address = label_node->address_label;

                /* Copy the instruction word to the instruction array and increment the instruction counter ('IC'). */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            } else {
                /* If the label node is not found in the symbol table, add an error for LABEL_NOT_FOUND. */
                add_error(LABEL_NOT_FOUND);
            }
        }
            break;
        case NOT_EXIST:
            break;
    }
}

static char * get_next_word_without_comma(){
    /* Static buffer to store the extracted word */
    static char temp_word[MAX_ASSEMBLY_LINE_LENGTH] = "";
    int j = 0;

    /* Read characters until a space, tab, newline, comma, or the end of the line is encountered */
    while (sfile->line_text[sfile->pos_in_line] != ' ' &&
           sfile->line_text[sfile->pos_in_line] != '\t' &&
           sfile->line_text[sfile->pos_in_line] != '\n' &&
           sfile->line_text[sfile->pos_in_line] != ',' &&
           sfile->line_text[sfile->pos_in_line] != '\0') {
        temp_word[j] = sfile->line_text[sfile->pos_in_line];
        (sfile->pos_in_line)++;
        j++;
    }

    /* Null-terminate the extracted word */
    temp_word[j] = '\0';

    /* Skip any white characters to move to the next word or the end of the line */
    sfile->pos_in_line = skip_white_character(sfile->pos_in_line, sfile->line_text);
    return temp_word;
}

static bool check_for_comma(){
    if (sfile->line_text[sfile->pos_in_line] == ','){
        /* A comma is found, increment the position and return TRUE. */
        (sfile->pos_in_line)++;
        return TRUE;
    } else {
        /* If the current character is not a comma and not the end of the line, add an error message. */
        if (sfile->line_text[sfile->pos_in_line] != '\n' && sfile->line_text[sfile->pos_in_line] != '\0'){
            add_error(COMMA_REQUIRED_BETWEEN_VALUES);
        }
        /* Return FALSE as a comma is not found at the current position. */
        return FALSE;
    }
}

static void add_extern_label_to_array(ptr_label temp_node){
    char int_ic[MAX_DIGITS_FOR_ADDRESS];

    /* Append the name of the external label to the 'extern_list' array. */
    strcat(extern_list,temp_node->name_label);

    /* Append a tab '\t' character as a separator between the label name and its address. */
    strcat(extern_list,"\t");

    /* Convert the address of the external label ('IC') to a string and append it to the 'extern_list' array. */
    sprintf(int_ic, "%d", sfile->IC);
    strcat(extern_list, int_ic);

    /* Append a newline '\n' character to separate each label entry in the 'extern_list' array. */
    strcat(extern_list, "\n");
}

static void create_all_files(){
    char *temp_entry_list;
    /* Check if entry labels are present in the assembly code. */
    if (sfile->entry_flag == TRUE){
        /* Create the entry file and open it in write mode. */
        sfile->file_ent = open_file(sfile->name_file, EXT_ENTRY, "w");

        /* Get the formatted string containing the entry labels and addresses. */
        temp_entry_list = get_entry_list(sfile->head_label_list);

        /* Write the entry labels and their addresses to the entry file. */
        fputs(temp_entry_list, sfile->file_ent);

        /* Free the temporary entry list string to release memory resources. */
        free(temp_entry_list);

        /* Close the entry file. */
        fclose(sfile->file_ent);

    }
    /* Check if external labels are present in the assembly code. */
    if (sfile->extern_flag == TRUE){
        /* Create the external file and open it in write mode. */
        sfile->file_ext = open_file(sfile->name_file, EXT_EXTERN, "w");

        /* Write the external labels and their addresses from the 'extern_list' array to the external file. */
        fputs(extern_list, sfile->file_ext);

        /* Close the external file. */
        fclose(sfile->file_ext);
    }
    /* Create the object file by calling the 'create_object_file' function. */
    create_object_file();
}

static void create_object_file(){
    int i;
    char *temp_word;

    /* Open the object file in write mode. */
    sfile->file_ob = open_file(sfile->name_file, EXT_OBJECT, "w");

    /* Write the header of the object file: (IC - FIRST_CELL_IN_MEMORY) and DC. */
    fprintf(sfile->file_ob, "%d\t%d\n", sfile->IC - FIRST_CELL_IN_MEMORY, sfile->DC);

    /* Write the instruction memory contents to the object file. */
    for (i = FIRST_CELL_IN_MEMORY; i < sfile->IC; i++){
        /* Convert the instruction word to a 64-base representation. */
        temp_word = convert_binary_to_64base(sfile->instruction_array[i]);

        /* Write the converted 64-base string to the object file. */
        fputs(temp_word, sfile->file_ob);

        /* Free the temporary word string to release memory resources. */
        free(temp_word);
    }
    /* Write the data memory contents to the object file. */
    for (i = 0; i < sfile->DC; i++) {
        /* Convert the data word to a 64-base representation. */
        temp_word = convert_binary_to_64base(sfile->data_array[i]);

        /* Write the converted 64-base string to the object file. */
        fputs(temp_word, sfile->file_ob);

        /* Free the temporary word string to release memory resources. */
        free(temp_word);
    }
    /* Close the object file. */
    fclose(sfile->file_ob);
}