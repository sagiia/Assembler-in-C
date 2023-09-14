#include "first_pass.h"
/*
 * Static variable: sfile
 * ----------------------
 * A pointer to the 'file' struct representing the current assembly source file being processed in
 * the first pass stage.
 */
static ptr_file sfile;

/*
 * Function: first_pass_on_curr_file
 * ---------------------------------
 * Performs the first pass of the assembly process on the current assembly source file.
 *
 * This function is responsible for executing the first pass of the assembly process on the provided
 * assembly source file. During the first pass, the function performs the following actions for each line:
 *   - Reads each line of the source file.
 *   - Checks if the line is empty; if so, it skips further processing and moves to the next line.
 *   - Initializes the line position to 0 for parsing.
 *   - Updates the line data structure to store information about the current line.
 *   - Identifies and processes labels in the line using 'actions_on_label'.
 *   - Determines the type of line (instruction, directive, etc.) and processes it accordingly
 *     using 'action_by_status'.
 *   - Frees the memory occupied by the line data structure.
 *   - Increments the count of processed lines.
 *   - After processing all lines, updates the addresses of labels used in data directives (if no errors occurred).
 *
 * Notes:
 *   - The global 'sfile' variable contains information about the source file, such as the file streams, line structures,
 *     the symbol table, and various counters used during the first pass.
 *   - The function updates the 'error_flag' of the 'sfile' variable if any errors occur during the first pass.
 *     The 'error_flag' indicates whether the first pass was successful without any errors.
 */
static void first_pass_on_curr_file();

/*
 * Function: update_files
 * ----------------------
 * Updates the file streams for the pre-assembly process.
 *
 * This function is responsible for updating the file streams used during the pre-assembly process.
 * It first closes the currently opened file streams, then opens the intermediate file ('file_am')
 * for reading and assigns the file stream to it.
 *
 * Notes:
 *   - The function uses the 'fclose' function to close the currently opened file streams for the
 *     assembly source file ('file_as') and the intermediate file ('file_am').
 *   - After closing the file streams, the function opens the intermediate file in read mode ('r')
 *     using the 'open_file' utility function and assigns the new file stream to 'file_am'.
 */
static void update_files();

/*
 * Function: update_next_line
 * --------------------------
 * Updates the 'line_text' buffer with the next line from the intermediate file and returns it.
 *
 * This function is responsible for reading the next line from the intermediate file ('file_am')
 * and updating the 'line_text' buffer with its content. The 'line_text' buffer is used to store
 * the current line being processed during the pre-assembly process.
 *
 * Returns:
 *   char *: A pointer to the 'line_text' buffer containing the next line from the intermediate file.
 *
 * Notes:
 *   - The function uses the 'memset' function to clear the 'line_text' buffer before reading the next line.
 *   - It then uses the 'fgets' function to read the next line from the intermediate file and stores it in 'line_text'.
 */
static char * update_next_line();

/*
 * Function: update_line_to_array
 * ------------------------------
 * Updates the 'line_struct' pointer with a new line structure created from the 'line_text' buffer.
 *
 * This function is responsible for creating a new line structure using the content of the 'line_text' buffer.
 * The 'line_text' buffer contains the current line being processed during the pre-assembly process.
 * The line structure is used to parse the line into separate words and store relevant information for further processing.
 *
 * Notes:
 *   - The function uses the 'create_new_line_struct' function from 'text_tool.h' to create a new line structure.
 *   - The new line structure is then assigned to the 'line_struct' pointer in the global 'sfile' variable,
 *     updating it with the parsed information from the 'line_text' buffer.
 */
static void update_line_to_array();

/*
 * Function: actions_on_label
 * --------------------------
 * Performs actions based on the presence of a label in the current line during the first pass.
 *
 * This function is responsible for handling actions related to the presence of a label in the current line
 * during the first pass of the assembly process. It checks if the line contains a label and takes appropriate
 * actions accordingly, such as adding the label to the label list or reporting errors if the label is misplaced.
 *
 * Notes:
 *   - The function first checks if the current line contains a label using the 'is_label' function from 'text_tool.h'.
 *   - If a label is found in the line, the function determines the type of label (instruction, directive, entry, or extern)
 *     using the 'get_word_status' function from 'text_tool.h'.
 *   - Based on the label type, the function takes appropriate actions:
 *       - If the label is of type STATUS_ENTRY or STATUS_EXTERN, it indicates an error because these labels should not be defined
 *         before the corresponding entry or extern directives. The function reports the error using the 'add_error' function from 'error_tool.h'.
 *       - If the label is of any other type, it adds the label to the label list using the 'add_new_label_to_list' function from 'label_list.h'.
 *       - The label is then deleted from the line structure using the 'delete_label_from_line_struct' function from 'text_tool.h'
 *         to facilitate further processing of the line without the label keyword.
 */
static void actions_on_label();

/*
 * Function: add_new_label_to_list
 * ------------------------------
 * Adds a new label to the label list based on the provided label status during the first pass.
 *
 * This function is responsible for adding a new label to the label list based on the provided label status
 * during the first pass of the assembly process. It first checks if the label name is valid using the 'is_label_name_valid'
 * function from 'text_tool.h'. If the label name is valid, the function adds the label to the label list with its corresponding
 * address (IC for code labels, DC for data labels) and type (code or data).
 *
 * Parameters:
 *   temp_status (line_status): The status of the current line, indicating the type of label (code, data, string, etc.).
 *
 * Notes:
 *   - The function checks if the label name is valid using the 'is_label_name_valid' function from 'text_tool.h'.
 *   - If the label name is valid, the function adds the label to the label list with the corresponding address and type:
 *       - If the label is of type STATUS_DATA or STATUS_STRING, it adds the label with the current 'DC' value as the address
 *         and the 'DATA' type.
 *       - If the label is of type STATUS_CODE, it adds the label with the current 'IC' value as the address and the 'CODE' type.
 *   - If the label name is invalid, the function reports an error of type INVALID_LABEL_NAME using the 'add_error' function from 'error_tool.h'.
 */
static void add_new_label_to_list(line_status temp_status);

/*
 * Function: get_word_status
 * -------------------------
 * Determines the status of a given word based on predefined keywords during the first pass.
 *
 * This function determines the status of a given word based on predefined keywords during the first pass of the assembly process.
 * It compares the input 'word_text' with predefined directives and instructions to identify the type of the word. The function returns
 * the corresponding 'line_status' enumeration value representing the status of the word.
 *
 * Parameters:
 *   word_text (char*): The text of the word to be evaluated for its status.
 *
 * Returns:
 *   line_status: The status of the word, represented by one of the 'line_status' enumeration values:
 *                - STATUS_DATA: Indicates that the word is a ".data" directive.
 *                - STATUS_STRING: Indicates that the word is a ".string" directive.
 *                - STATUS_EXTERN: Indicates that the word is a ".extern" directive.
 *                - STATUS_ENTRY: Indicates that the word is an ".entry" directive.
 *                - STATUS_CODE: Indicates that the word is an instruction or label (code status).
 *
 * Notes:
 *   - The function compares the 'word_text' with predefined directives and instructions to determine its status.
 */
static line_status get_word_status(char *word_text);

/*
 * Function: action_by_status
 * --------------------------
 * Performs appropriate actions based on the given line status during the first pass.
 *
 * This function performs the appropriate actions based on the given 'status' during the first pass of the assembly process.
 * The 'status' parameter represents the status of the current line, which is determined using the 'get_word_status' function.
 * Depending on the line status, this function takes different actions to process the line appropriately during the first pass.
 *
 * Parameters:
 *   status (line_status): The status of the current line, as determined by the 'get_word_status' function.
 *
 * Notes:
 *   - The function uses a switch-case statement to handle different line statuses.
 *   - Based on the 'status', the function calls the corresponding helper functions to perform specific actions:
 *     - For STATUS_DATA, it calls the 'save_data' function to handle the ".data" directive.
 *     - For STATUS_STRING, it calls the 'save_string' function to handle the ".string" directive.
 *     - For STATUS_EXTERN, it calls the 'add_extern_labels' function to handle the ".extern" directive and add external labels.
 *     - For STATUS_ENTRY, it returns without taking any action, as the ".entry" directive is handled separately during the second pass.
 *     - For STATUS_CODE, it calls the 'add_instructions' function to handle instructions and labels in the code section.
 */
static void action_by_status(line_status status);

/*
 * Function: add_error
 * -------------------
 * Add an error to the first pass process and update the error status of the current file.
 *
 * This function is responsible for adding an error with the specified 'error_code' to the first pass process.
 * It calls the 'print_error' function to display the error message associated with the 'error_code' and the current line number.
 * Additionally, it sets the 'error_flag' of the current file ('sfile') to TRUE to indicate that an error has occurred in the assembly process.
 *
 * Parameters:
 *   error_code (error_code): The error code indicating the type of error encountered during the pre-assembly process.
 */
static void add_error(error_code error_code);

/*
 * Function: update_error_status
 * -----------------------------
 * Update the error status of the current file based on the given 'error_code'.
 *
 * This function is responsible for updating the error status of the current file ('sfile')
 * based on the provided 'error_code'. If the 'error_code' is not equal to 'NO_ERROR',
 * it calls the 'add_error' function to add the corresponding error to the pre-assembly process.
 *
 * Parameters:
 *   error_code (error_code): The error code indicating the type of error encountered during the first pass process.
 *
 * Notes:
 *   - This function is used to manage error status during the first pass process.
 *   - It is typically called when an operation returns an error code.
 *   - If the 'error_code' is not equal to 'NO_ERROR', the 'add_error' function is called to handle the error.
 */
static void update_error_status(error_code error_code);

/*
 * Function: save_data
 * -------------------
 * Process and saves the data values in the current line to the data array.
 *
 * This function is responsible for processing the data values in the current line and saving them to the 'data_array'
 * of the current file struct. It skips the first word (which is assumed to be ".data") and then iterates through the
 * remaining words, parsing them as integers and storing them in the data array.
 *
 * Notes:
 *   - The 'DC' (Data Counter) is incremented for each data value added to the 'data_array'.
 *   - The function handles error conditions, such as missing values after the ".data" directive and invalid comma positions.
 *   - If a word in the line is not a valid number, an error is added to the file's error list.
 */
static void save_data();

/*
 * Function: save_string
 * ---------------------
 * Process and saves the string defined in the current line to the data array.
 *
 * This function is responsible for processing the string defined in the current line and saving its ASCII values
 * to the 'data_array' of the current file struct. It skips the first word (which is assumed to be ".string") and
 * then iterates through the characters within the quotes, converting each character to its ASCII value and storing
 * it in the data array. The string is null-terminated in the data array.
 *
 * Notes:
 *   - The 'DC' (Data Counter) is incremented for each character added to the 'data_array'.
 *   - The function handles various error conditions, such as missing quotes, invalid string structure, and extra parameters.
 *   - If the string does not start or end in quotes or contains an invalid structure, an appropriate error is added to the file's error list.
 */
static void save_string();

/*
 * Function: add_extern_labels
 * ---------------------------
 * Process and add extern labels defined in the current line to the external label list.
 *
 * This function is responsible for processing the extern labels defined in the current line
 * and adding them to the external label list of the current file struct. It sets the 'extern_flag'
 * to TRUE, indicating that the current file contains extern labels. The function then iterates through
 * the labels, extracts them from the line, and validates each label name. Valid labels are added to the
 * external label list with the 'EXTERN' label type.
 *
 * Notes:
 *   - The function handles various error conditions, such as missing labels, invalid label names, and invalid comma positions.
 *   - If a label name is not valid or the comma is not correctly placed, an appropriate error is added to the file's error list.
 */
static void add_extern_labels();

/*
 * Function: add_instructions
 * --------------------------
 * Process and add instructions to the machine code array.
 *
 * This function is responsible for processing instructions found in the current line and adding them to the
 * machine code array of the current file struct. It first determines the type of instruction based on the first
 * word of the line. Then, it updates the addressing method type for the instruction, checks for errors related to
 * the instruction, and adds the instruction's binary representation to the machine code array. If no errors are found,
 * the instruction and its operands are added to the machine code array.
 *
 * Notes:
 *   - The 'get_instruction_type' function is used to determine the type of instruction based on the first word of the line.
 *   - The 'update_addressing_method_type' function updates the addressing method type for the current instruction.
 *   - The 'check_errors_for_instructions' function checks for errors related to the instruction, such as invalid operands or missing labels.
 *   - If any errors are encountered during processing, the 'error_flag' of the 'sfile' is set to TRUE, indicating the presence of errors.
 *   - If no errors are found, the binary representation of the instruction and its operands are added to the machine code array.
 */
static void add_instructions();

/*
 * Function: update_addressing_method_type
 * ---------------------------------------
 * Update the addressing method types for the operands of the given instruction.
 *
 * This function updates the addressing method types for the source and destination operands of the given instruction,
 * based on the instruction type. It also handles special cases where the instruction has a single operand.
 * The updated addressing method types are stored in the 'line_struct' of the current file struct.
 *
 * Parameters:
 *   type (instruction_type): The type of the instruction, obtained from the first word of the line.
 *
 * Notes:
 *   - The 'get_addressing_method_type' function is used to determine the addressing method type of an operand based on its text.
 *   - Depending on the instruction type, the function updates the 'source' and 'destination' fields of the 'line_struct'.
 *   - In special cases where the instruction has a single operand, the 'source' field is set to NOT_EXIST, and the 'destination'
 *     field is set to the addressing method type of the operand (word2).
 *   - The 'word4' of the 'line_struct' is updated with the content of 'word2' in cases where the instruction has a single operand.
 */
static void update_addressing_method_type(instruction_type type);

/*
 * Function: check_errors_for_instructions
 * ---------------------------------------
 * Check for errors in the given instruction and update the error flag accordingly.
 *
 * This function checks for various errors in the given instruction, such as incorrect number of operands,
 * missing or extra comma between operands, and invalid addressing method for specific instructions.
 * It updates the error flag in the 'sfile' struct accordingly if any errors are found.
 *
 * Parameters:
 *   type (instruction_type): The type of the instruction, obtained from the first word of the line.
 *
 * Notes:
 *   - The 'count' field in the 'line_struct' represents the number of words in the instruction line.
 *   - The function uses the 'instruction_type' to handle specific checks for each instruction type.
 *   - The 'get_addressing_method_type' function is used to obtain the addressing method type of operands.
 *   - Depending on the instruction type, the function checks for valid operand counts and correct comma usage.
 */
static void check_errors_for_instructions(instruction_type type);

/*
 * Function: add_first_word_of_instruction_to_array
 * -----------------------------------------------
 * Add the first word of the instruction to the instruction array.
 *
 * This function constructs the first word of the instruction based on the given 'type' of the instruction
 * and the addressing methods of the operands. The first word consists of several fields that represent
 * the encoding type, destination address, opcode, and source address of the instruction.
 * The constructed first word is then added to the 'instruction_array' at the current 'IC' index.
 *
 * Parameters:
 *   type (instruction_type): The type of the instruction, obtained from the first word of the line.
 *
 * Notes:
 *   - The 'instruction_array' is an array that stores the binary representation of the instructions.
 *   - The 'IC' is incremented after adding the first word, preparing for the next word of the instruction.
 *   - The 'encoding_type' field is set to 'ABSOLUTE' as all instructions are absolute in the first pass.
 *   - The 'destination_address' and 'source_address' fields are obtained from the 'line_struct'.
 *   - The 'opcode' field is set based on the 'type' of the instruction.
 *   - The 'memcpy' function is used to copy the bit-field structure to the 'instruction_array'.
 */
static void add_first_word_of_instruction_to_array(instruction_type type);

/*
 * Function: add_the_rest_of_the_instruction_to_array
 * -------------------------------------------------
 * Add the rest of the instruction to the instruction array.
 *
 * This function adds the remaining words of the instruction to the 'instruction_array' based on the addressing
 * methods of the source and destination operands. The function constructs the word(s) of the instruction as
 * needed and adds them to the 'instruction_array'.
 *
 * Notes:
 *     'IC' (Instruction Counter), 'instruction_array', and the 'data_array'.
 *   - The 'instruction_array' is an array that stores the binary representation of the instructions.
 *   - The 'IC' represents the index in the 'instruction_array' where the next word of the instruction should be added.
 *   - The function uses bit-field structures to construct the different word formats of the instruction.
 *   - The 'ABSOLUTE' encoding type is used for all instructions in the first pass.
 *   - The function converts numeric values from the instruction line to integers using 'atoi'.
 *   - Depending on the addressing method of the source and destination operands, the appropriate fields of the
 *     word(s) of the instruction are constructed and added to the 'instruction_array'.
 */
static void add_the_rest_of_the_instruction_to_array();

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
 * Function: update_address_label_of_data
 * --------------------------------------
 * Update the addresses of data labels.
 *
 * This function updates the addresses of data labels in the linked list of labels represented by
 * 'head_label_list'. The update is performed based on the 'IC' (Instruction Counter) value of the current
 * source file 'sfile'. The 'IC' represents the current address of the first instruction in the instruction
 * array. The function traverses the linked list of labels and updates the address of data labels found in
 * the data array (starting from the 'IC' value).
 */
static void update_address_label_of_data();

void start_first_pass(ptr_file original_file_struct){
    /* Set the global 'sfile' variable to the provided 'original_file_struct'. */
    sfile = original_file_struct;

    /* Initiate the first pass of the assembly process for the current file. */
    first_pass_on_curr_file();
}

static void first_pass_on_curr_file(){
    /* Open the intermediate files for writing. */
    update_files();
    sfile->count_line = 0;

    /* Process each line in the current file. */
    while(update_next_line() != NULL){
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

        /* Process any labels found in the line. */
        actions_on_label();
        if (sfile->line_struct->count == 0){
            continue;
        }

        /* Determine the type of line (instruction, directive, etc.) and process it accordingly. */
        action_by_status(get_word_status(sfile->line_struct->word1));

        /* Free the memory occupied by the line data structure. */
        free_line(sfile->line_struct);
    }

    /* If no errors occurred during the first pass, update the addresses of labels used in data directives. */
    if (sfile->error_flag == FALSE){
        update_address_label_of_data();
    }
}

static void update_files(){
    /* Close the currently opened file streams. */
    fclose(sfile->file_as);
    fclose(sfile->file_am);

    /* Open the intermediate file for reading. */
    sfile->file_am = open_file(sfile->name_file, EXT_MACRO,"r"); /* */
}

static char * update_next_line(){
    /* Clear the 'line_text' buffer before reading the next line. */
    memset(sfile->line_text, 0, MAX_ASSEMBLY_LINE_LENGTH);

    /* Read the next line from the intermediate file and store it in 'line_text'. */
    return fgets(sfile->line_text,sizeof (sfile->line_text),sfile->file_am);
}

static void update_line_to_array(){
    /* Create a new line structure using the content of the 'line_text' buffer. */
    sfile->line_struct = create_new_line_struct(sfile->line_text);
}

static void actions_on_label(){
    line_status temp_status;

    /* Check if the current line contains a label. */
    if(is_label(sfile->line_struct) == TRUE){
        /* Determine the type of label (instruction, directive, entry, or extern). */
        temp_status = get_word_status(sfile->line_struct->word2);
        switch (temp_status) {
            case STATUS_ENTRY:
                /* Error: Entry label should not be defined before the entry directive. */
                add_error(CANT_DEFINE_LABEL_BEFORE_ENTRY);
                sfile->line_struct->count = 0;
                break;
            case STATUS_EXTERN:
                /* Error: Extern label should not be defined before the extern directive. */
                add_error(CANT_DEFINE_LABEL_BEFORE_EXTERN);
                sfile->line_struct->count = 0;
                break;
            default:
                /* Add the label to the label list and remove the label from the line structure. */
                sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);
                add_new_label_to_list(temp_status);
                delete_label_from_line_struct(sfile->line_struct);
                break;
        }
    }
}

static void add_new_label_to_list(line_status temp_status){
    /* Check if the label name is valid. */
    if (is_label_name_valid(sfile->line_struct->word1) == TRUE){
        /* Add the label to the label list with the corresponding address and type. */
        if (temp_status == STATUS_DATA || temp_status == STATUS_STRING){
            update_error_status(add_to_list_label(&sfile->head_label_list, sfile->line_struct->word1, sfile->DC, DATA));
        }
        if (temp_status == STATUS_CODE){
            update_error_status(add_to_list_label(&sfile->head_label_list, sfile->line_struct->word1, sfile->IC, CODE));
        }
    } else {
        /* Error: Invalid label name. */
        add_error(INVALID_LABEL_NAME);
    }
}

static line_status get_word_status(char *word_text){
    /* Compare the 'word_text' with predefined directives and instructions. */
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
    /* If the 'word_text' does not match any predefined directive, it is considered an instruction or label (code status). */
    return STATUS_CODE;
}

static void action_by_status(line_status status){
    switch (status) {
        case STATUS_DATA:
            /* Handle the ".data" directive and save data values. */
            save_data();
            break;
        case STATUS_STRING:
            /* Handle the ".string" directive and save string characters. */
            save_string();
            break;
        case STATUS_EXTERN:
            /* Handle the ".extern" directive and add external labels. */
            add_extern_labels();
            break;
        case STATUS_ENTRY:
            return; /* Return without taking any action for ".entry" directive (handled during second pass). */
        case STATUS_CODE:
            /* Handle instructions and labels in the code section. */
            add_instructions();
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
    if (error_code != NO_ERROR){
        /* Call the function to add the error to the file's error list. */
        add_error(error_code);
    }
}

static void save_data(){
    char temp_word[MAX_ASSEMBLY_LINE_LENGTH] = "";
    int temp_number;

    /* Skip the first word (assumed to be ".data") in the current line. */
    sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);

    /* Check if there are no values provided after the ".data" directive. */
    if (is_end_line(sfile->pos_in_line, sfile->line_text) == TRUE){
        /* Error: No values provided after ".data" directive. */
        add_error(MUST_PROVIDE_VALUES_TO_DATA);
        return;
    } else {
        do {
            /* Skip any white spaces before the next data value. */
            sfile->pos_in_line = skip_white_character(sfile->pos_in_line, sfile->line_text);

            /* Check if there is an invalid comma position. */
            if (sfile->line_text[sfile->pos_in_line] == ','){
                /* Error: Comma found at an invalid position. */
                add_error(INVALID_COMMA_POSITION);
                return;
            }

            /* Get the next word without the comma (if any) and store it in 'temp_word'. */
            strcpy(temp_word, get_next_word_without_comma()); /* */

            /* Check if the extracted word is a valid number. */
            if (is_number(temp_word) == TRUE){
                temp_number = atoi(temp_word);

                /* Store the data value in the data array and increment the Data Counter. */
                sfile->data_array[sfile->DC] = (unsigned int) temp_number;
                (sfile->DC)++;
            } else {
                /* Error: Data value is not a valid number. */
                add_error(DATA_NEED_NUM_VALUE);
            }
        } while (check_for_comma() == TRUE); /* Continue processing while there is a comma. */
    }
}

static void save_string(){
    /* Skip the first word (assumed to be ".string") in the current line. */
    sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);

    /* Skip any white spaces after the ".string" directive. */
    sfile->pos_in_line = skip_white_character(sfile->pos_in_line, sfile->line_text);

    /* Check if the first character after ".string" is a quote. */
    if (sfile->line_text[sfile->pos_in_line] == '"'){
        /* Move to the next character after the opening quote. */
        (sfile->pos_in_line)++;

        /* Process the characters within the quotes until a closing quote is found. */
        while(is_end_line(sfile->pos_in_line, sfile->line_text) == FALSE && sfile->line_text[sfile->pos_in_line] != '"'){
            /* Store the ASCII value of the current character in the data array and increment the Data Counter. */
            sfile->data_array[sfile->DC] = (unsigned int) sfile->line_text[sfile->pos_in_line];
            (sfile->DC)++;
            (sfile->pos_in_line)++;
        }

        /* Check if the closing quote is found after processing the characters. */
        if (sfile->line_text[sfile->pos_in_line] == '"'){
            /* Move to the next character after the closing quote. */
            (sfile->pos_in_line)++;

            /* Null-terminate the string in the data array and increment the Data Counter. */
            sfile->data_array[sfile->DC] = '\0';
            (sfile->DC)++;

            /* Check for additional parameters after the closing quote. */
            if (is_end_line(sfile->pos_in_line, sfile->line_text) == FALSE){
                /* Error: String directive should have only one parameter (the string). */
                add_error(STRING_DIRECTIVE_ACCEPTS_ONE_PARAMETER);
            }
        } else {
            /* Error: String must end in quotes. */
            add_error(STRING_MUST_END_IN_QUOTES);
        }
    } else {
        /* Error: Invalid string structure. */
        add_error(STRING_STRUCTURE_NOT_VALID);
    }
}

static void add_extern_labels(){
    char temp_word[MAX_ASSEMBLY_LINE_LENGTH] = "";

    /* Set the extern_flag to TRUE, indicating that the current file contains extern labels. */
    sfile->extern_flag = TRUE;

    /* Skip the first word (assumed to be ".extern") in the current line. */
    sfile->pos_in_line = skip_one_word_in_line(sfile->pos_in_line, sfile->line_text);

    /* Check if the line is empty after the ".extern" directive. */
    if (is_end_line(sfile->pos_in_line, sfile->line_text) == TRUE){
        /* Error: No labels provided after the ".extern" directive. */
        add_error(MUST_PROVIDE_LABELS_TO_EXTERN);
        return;
    } else {
        /* Process each label separated by commas in the line. */
        do {
            /* Skip any white spaces before the label. */
            sfile->pos_in_line = skip_white_character(sfile->pos_in_line, sfile->line_text);

            /* Check for an invalid comma position. */
            if (sfile->line_text[sfile->pos_in_line] == ','){
                /* Error: Comma found at an invalid position. */
                add_error(INVALID_COMMA_POSITION);
                return;
            }

            /* Extract the next label without the comma and store it in 'temp_word'. */
            strcpy(temp_word, get_next_word_without_comma());

            /* Validate the label name extracted from 'temp_word'. */
            if (is_label_name_valid(temp_word) == TRUE){
                /* Add the valid label to the external label list with the 'EXTERN' label type and address 0. */
                update_error_status(add_to_list_label(&sfile->head_label_list, temp_word, 0, EXTERN));
            } else {
                /* Error: Invalid label name. */
                add_error(INVALID_LABEL_NAME);
            }
        } while (check_for_comma() == TRUE); /* Check if there are more labels to process. */
    }
}


static void add_instructions(){
    instruction_type type;

    /* Determine the type of instruction based on the first word of the line. */
    type = get_instruction_type(sfile->line_struct->word1);

    /* Update the addressing method type for the instruction. */
    update_addressing_method_type(type);

    /* Check for errors related to the instruction. */
    check_errors_for_instructions(type);

    /* If no errors are found, add the instruction and its operands to the machine code array. */
    if (sfile->error_flag == FALSE){
        /* Add the binary representation of the instruction's first word to the machine code array. */
        add_first_word_of_instruction_to_array(type);

        /* Add the rest of the instruction (operands) to the machine code array. */
        add_the_rest_of_the_instruction_to_array();
    }
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

static void check_errors_for_instructions(instruction_type type){
    /* Check for errors related to the number of operands */
    if (sfile->line_struct->count == TOO_MUCH || sfile->line_struct->count == FIVE){
        add_error(TOO_MUCH_WORDS_FOR_INSTRUCTION);
    }

    /* Check for instruction-specific errors */
    switch (type) {
        case MOV: case CMP: case ADD: case SUB: case LEA:
            if (sfile->line_struct->count != FOUR){ add_error(INSTRUCTION_SHOULD_RECEIVE_TWO_OPERANDS); }
            if (strcmp(sfile->line_struct->word3, ",") != 0) { add_error(COMMA_REQUIRED_BETWEEN_OPERANDS); }
            break;
        case NOT: case CLR: case INC: case DEC: case JMP: case BNE: case RED: case PRN: case JSR:
            if (sfile->line_struct->count != TWO){ add_error(INSTRUCTION_SHOULD_RECEIVE_ONE_OPERAND); }
            break;
        case RTS: case STOP:
            if (sfile->line_struct->count != ONE){ add_error(INSTRUCTION_SHOULD_NOT_RECEIVE_OPERANDS); }
            break;
        case NOT_INSTRUCTION:
            add_error(INSTRUCTION_NAME_NOT_EXIST);
            break;
    }

    /* Check for invalid addressing methods for specific instructions */
    if (type == MOV || type == ADD || type == SUB){
        if (sfile->line_struct->destination == IMMEDIATE) {
            add_error(INVALID_ADDRESS_METHOD_FOR_INSTRUCTION);
            return;
        }
    }
    if (type == LEA){
        if (sfile->line_struct->destination == IMMEDIATE ||
            sfile->line_struct->source != DIRECT) {
            add_error(INVALID_ADDRESS_METHOD_FOR_INSTRUCTION);
            return;
        }
    }
    if (type == NOT || type == CLR || type == INC || type == DEC || type == JMP || type == BNE ||
        type == RED || type == JSR){
        if (sfile->line_struct->destination == IMMEDIATE) {
            add_error(INVALID_ADDRESS_METHOD_FOR_INSTRUCTION);
            return;
        }
    }
}

static void add_first_word_of_instruction_to_array(instruction_type type){
    /* Bit-field structure to construct the first word of the instruction */
    struct {
        unsigned int encoding_type:2; /* Encoding type field (2 bits) */
        unsigned int destination_address:3; /* Destination address field (3 bits) */
        unsigned int opcode:4; /* Opcode field (4 bits) */
        unsigned int source_address:3; /* Source address field (3 bits) */
    } first_word_of_instruction;

    /* Set the values of the bit-field structure fields */
    first_word_of_instruction.encoding_type = ABSOLUTE;
    first_word_of_instruction.destination_address = sfile->line_struct->destination;
    first_word_of_instruction.opcode = type;
    first_word_of_instruction.source_address = sfile->line_struct->source;

    /* Copy the constructed first word to the instruction array at the current 'IC' index */
    memcpy(&sfile->instruction_array[sfile->IC], &first_word_of_instruction, sizeof(unsigned int));

    /* Increment the 'IC' for the next word of the instruction */
    (sfile->IC)++;
}

static void add_the_rest_of_the_instruction_to_array(){
    int temp_number;

    /* Handle the source operand of the instruction */
    switch (sfile->line_struct->source) {
        case REGISTER:
            {
                /* Bit-field structure for source operand with register addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int register_number_destination:5; /* Register number for destination operand (5 bits) */
                    unsigned int register_number_source:5; /* Register number for source operand (5 bits) */
                } word_of_instruction;

                /* Set the values of the bit-field structure fields */
                word_of_instruction.encoding_type = ABSOLUTE;
                word_of_instruction.register_number_source = get_number_of_register(sfile->line_struct->word2);

                /* Check if the destination operand is also a register */
                if (sfile->line_struct->destination == REGISTER){
                    word_of_instruction.register_number_destination = get_number_of_register(sfile->line_struct->word4);
                } else {
                    word_of_instruction.register_number_destination = NOT_EXIST;
                }

                /* Copy the constructed word to the instruction array at the current 'IC' index */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            }
            break;
        case IMMEDIATE:
            {
                /* Bit-field structure for source operand with immediate addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int value:10; /* Value field for immediate operand (10 bits) */
                } word_of_instruction;

                /* Set the values of the bit-field structure fields */
                word_of_instruction.encoding_type = ABSOLUTE;
                temp_number = atoi(sfile->line_struct->word2);
                word_of_instruction.value = temp_number;

                /* Copy the constructed word to the instruction array at the current 'IC' index */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            }
            break;
        case DIRECT:
            {
                /* Direct addressing method does not require additional words in the instruction */
                sfile->instruction_array[sfile->IC] = 0;
                (sfile->IC)++;
            }
            break;
        case NOT_EXIST:
            /* No source operand, nothing to add */
            break;
    }

    /* Handle the destination operand of the instruction */
    switch (sfile->line_struct->destination) {
        case REGISTER:
            /* Check if the source operand is not a register to avoid duplication */
            if (sfile->line_struct->source != REGISTER){
                /* Bit-field structure for destination operand with register addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int register_number_destination:5; /* Register number for destination operand (5 bits) */
                    unsigned int register_number_source:5; /* Source register number set to NOT_EXIST (5 bits) */
                } word_of_instruction;

                /* Set the values of the bit-field structure fields */
                word_of_instruction.encoding_type = ABSOLUTE;
                word_of_instruction.register_number_source = NOT_EXIST;
                word_of_instruction.register_number_destination = get_number_of_register(sfile->line_struct->word4);

                /* Copy the constructed word to the instruction array at the current 'IC' index */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            }
            break;
        case IMMEDIATE:
            {
                /* Bit-field structure for destination operand with immediate addressing */
                struct {
                    unsigned int encoding_type:2; /* Encoding type field (2 bits) */
                    unsigned int value:10; /* Value field for immediate operand (10 bits) */
                } word_of_instruction;

                /* Set the values of the bit-field structure fields */
                word_of_instruction.encoding_type = ABSOLUTE;
                temp_number = atoi(sfile->line_struct->word4);
                word_of_instruction.value = temp_number;

                /* Copy the constructed word to the instruction array at the current 'IC' index */
                memcpy(&sfile->instruction_array[sfile->IC], &word_of_instruction, sizeof(unsigned int));
                (sfile->IC)++;
            }
            break;
        case DIRECT:
            {
                /* Direct addressing method does not require additional words in the instruction */
                sfile->instruction_array[sfile->IC] = 0;
                (sfile->IC)++;
            }
            break;
        case NOT_EXIST:
            /* No destination operand, nothing to add */
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

static void update_address_label_of_data(){
    update_address_of_data(sfile->head_label_list, sfile->IC);
}