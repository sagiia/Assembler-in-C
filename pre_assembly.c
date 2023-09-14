#include "pre_assembly.h"

/*
 * Static variable: curr_macro_text
 * -------------------------------
 * An array of characters used to store the current text or content associated with a macro.
 * The 'curr_macro_text' array has a maximum size of 'MAX_CHARACTERS_ASSEMBLY_FILE'.
 * During the pre-assembly process, this variable is used to accumulate the lines of text that
 * make up the body of a macro being defined.
 */
static char curr_macro_text[MAX_CHARACTERS_ASSEMBLY_FILE];

/*
 * Static variable: curr_macro_name
 * -------------------------------
 * An array of characters used to store the name of the current macro being defined.
 * The 'curr_macro_name' array has a maximum size of 'MAX_ASSEMBLY_LINE_LENGTH'.
 * During the pre-assembly process, this variable is updated with the name of a macro when the
 * 'START_MACRO' directive is encountered in the assembly code.
 */
static char curr_macro_name[MAX_ASSEMBLY_LINE_LENGTH];

/*
 * Static variable: macro_flag
 * ---------------------------
 * A boolean variable that acts as a flag during the pre-assembly process to indicate whether the
 * current code is inside a macro definition.
 * When 'macro_flag' is set to TRUE, it means that the pre-assembly is currently processing the
 * lines of a macro. When it is FALSE, the pre-assembly is processing regular assembly code lines.
 */
static bool macro_flag;

/*
 * Global variable: temp_head_macro_list
 * ------------------------------------
 * A pointer to the head of the linked list of macros used during the pre-assembly process.
 * The 'temp_head_macro_list' points to the first node in the linked list of 'item_macro' representing
 * the macros defined in the assembly code.
 * This variable is used to traverse the list of macros and perform operations related to macro processing.
 */
ptr_macro temp_head_macro_list;

/*
 * Static variable: sfile
 * ----------------------
 * A pointer to the 'file' struct representing the current assembly source file being processed in
 * the pre-assembly stage.
 */
static ptr_file sfile;

/*
 * Function: pre_assembly_on_curr_file
 * ----------------------------------
 * Perform pre-assembly processing for the current assembly source file.
 *
 * This function is responsible for handling the pre-assembly process for the current assembly source file.
 * It goes through the source file line by line and performs various actions based on the status of the first word
 * in each line. The pre-assembly process includes identifying macro definitions, expanding macros, and generating
 * the intermediate file with macro-expanded code and macro definitions.
 */
static void pre_assembly_on_curr_file();

/*
 * Function: update_files
 * ----------------------
 * Update files for pre-assembly processing.
 *
 * This function updates the files required for the pre-assembly process. It opens the intermediate file with the '.am' extension
 * for writing the pre-assembled code. The intermediate file will store the expanded code with macros replaced.
 */
static void update_files();

/*
 * Function: update_next_line
 * --------------------------
 * Update the next line of code from the source assembly file.
 *
 * This function reads the next line of code from the source assembly file and stores it in the 'line_text' array of the
 * 'sfile' structure. The 'line_text' array will hold the text of the current line being processed during pre-assembly.
 *
 * Returns:
 *   char*: A pointer to the 'line_text' array, which holds the text of the next line of code read from the source file.
 *          If the end of the file is reached or an error occurs during reading, the function returns NULL.
 */
static char * update_next_line();

/*
 * Function: update_line_to_array
 * ------------------------------
 * Update the current line of code from 'line_text' to a structured line representation.
 *
 * This function creates a new structured line representation for the current line of code stored in the 'line_text' array
 * of the 'sfile' structure. The structured line representation is created using the 'create_new_line_struct' function,
 * which extracts individual words (tokens) from the line and organizes them into a structured format. The structured line
 * representation is stored in the 'line_struct' field of the 'sfile' structure.
 */
static void update_line_to_array();

/*
 * Function: get_first_word_status
 * -------------------------------
 * Get the status of the first word in the current line of code.
 *
 * This function examines the first word (token) in the structured representation of the current line of code,
 * which is stored in the 'sfile->line_struct->word1' field. It determines the status of the first word based
 * on the following conditions:
 *   - If the first word matches the name of an existing macro in the macro list ('head_macro_list'), the function
 *     returns 'STATUS_MACRO_NAME' to indicate that the line contains the name of an existing macro.
 *   - If the first word matches the predefined 'START_MACRO' string, the function checks if there is an ongoing
 *     macro definition (indicated by the 'macro_flag' being TRUE). If a macro definition is already in progress,
 *     it indicates an error (NESTED_MACRO_DEFINITION) and returns 'STATUS_MCRO' to indicate the start of a new macro
 *     definition.
 *   - If the first word matches the predefined 'END_MACRO' string, the function returns 'STATUS_ENDMCRO' to indicate
 *     the end of a macro definition.
 *   - If 'macro_flag' is TRUE, it means that the current line is part of a macro definition, and the function returns
 *     'STATUS_TEXT_OF_MACRO' to indicate that the line contains text belonging to the macro being defined.
 *   - If none of the above conditions are met, the function returns 'STATUS_TEXT_OF_CODE' to indicate that the line
 *     contains regular code text outside of any macro definition.
 *
 * Returns:
 *   first_word_status: An enumerated value representing the status of the first word in the current line of code.
 */
static first_word_status get_first_word_status();

/*
 * Function: action_by_status
 * -------------------------
 * Perform an action based on the given 'status'.
 *
 * This function takes the 'status' as input and performs a corresponding action based on the status value.
 * The 'status' is typically determined by the 'get_first_word_status' function and represents the type of code
 * contained in the current line of code. The function uses a switch statement to handle different status values
 * and calls the appropriate helper function to execute the corresponding action.
 *
 * Parameters:
 *   status (first_word_status): The status of the first word in the current line of code, which indicates the type
 *                               of code in the line.
 */
static void action_by_status(first_word_status status);

/*
 * Function: add_error
 * -------------------
 * Add an error to the pre-assembly process and update the error status of the current file.
 *
 * This function is responsible for adding an error with the specified 'error_code' to the pre-assembly process.
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
 *   error_code (error_code): The error code indicating the type of error encountered during the pre-assembly process.
 *
 * Notes:
 *   - This function is used to manage error status during the pre-assembly process.
 *   - It is typically called when an operation returns an error code.
 *   - If the 'error_code' is not equal to 'NO_ERROR', the 'add_error' function is called to handle the error.
 */
static void update_error_status(error_code error_code);

/*
 * Function: paste_macro_text
 * --------------------------
 * Paste the text of the current macro at the end of the macro file.
 *
 * This function is responsible for appending the text of the current macro ('temp_head_macro_list->text_macro')
 * to the end of the macro file ('sfile->file_am'). It seeks to the end of the file using 'fseek' with 'SEEK_END'
 * as the origin, and then writes the macro text to the file using 'fputs'.
 *
 * Notes:
 *   - This function is called when the pre-assembly process encounters a line of code that is part of a defined macro.
 *   - It appends the text of the macro to the macro file so that it can be used during the assembly process.
 *   - The 'temp_head_macro_list' points to the current macro being processed during the pre-assembly.
 *   - The 'file_am' is the file where the macro text is stored during the pre-assembly process.
 */
static void paste_macro_text();

/*
 * Function: update_name_of_macro
 * ------------------------------
 * Update the name of the current macro being processed.
 *
 * This function is called when the pre-assembly process encounters the "START_MACRO" directive,
 * indicating the start of a new macro definition. It updates the name of the current macro ('curr_macro_name')
 * based on the second word of the current line in the assembly file ('sfile->line_struct->word2').
 * Additionally, it sets the 'macro_flag' to TRUE, indicating that the pre-assembly process is currently
 * inside a macro definition.
 */
static void update_name_of_macro();

/*
 * Function: add_new_macro_to_list
 * ------------------------------
 * Add the current macro to the linked list of macros.
 *
 * This function is called when the pre-assembly process encounters the "END_MACRO" directive,
 * indicating the end of a macro definition. It adds the current macro, represented by 'curr_macro_name'
 * (the name) and 'curr_macro_text' (the text or content), to the linked list of macros associated with
 * the current file ('sfile->head_macro_list').
 */
static void add_new_macro_to_list();

/*
 * Function: add_text_to_macro
 * ---------------------------
 * Appends the current line's text to the text of the current macro being defined.
 *
 * This function is called when the pre-assembly process is inside a macro definition (indicated by 'macro_flag'
 * being TRUE), and it encounters lines of text that are part of the macro content. The function appends the text
 * of the current line ('sfile->line_text') to the text of the current macro being defined ('curr_macro_text').
 */
static void add_text_to_macro();

/*
 * Function: paste_code_text
 * -------------------------
 * Pastes the current line of code text to the output assembly file (file_am).
 *
 * This function is called when the pre-assembly process is outside of a macro definition (indicated by 'macro_flag'
 * being FALSE) and encounters lines of code text that are not part of any macro definition. The function pastes
 * the current line of code ('sfile->line_text') directly to the output assembly file ('file_am'), which represents
 * the pre-assembled assembly file without macros.
 */
static void paste_code_text();

/*
 * Function: print_end_of_pre_assembly
 * -----------------------------------
 * Prints the completion message for the pre-assembly process if no errors were encountered.
 *
 * This function is called after the pre-assembly process has finished processing all lines in the input assembly file.
 * If no errors were encountered during the pre-assembly process (indicated by 'sfile->error_flag' being FALSE),
 * the function prints a completion message along with the total number of macros found during the process.
 * If errors were encountered during pre-assembly, the function does not print anything.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None (The completion message may be printed to the console if no errors occurred during pre-assembly)
 *
 * Notes:
 *   - This function is called at the end of the pre-assembly process, regardless of whether errors were encountered.
 *   - The 'sfile' variable holds the information about the current file being pre-assembled, including the number of macros
 *     found during the process ('count_macro') and whether any errors were encountered ('error_flag').
 *   - If no errors were encountered during pre-assembly (i.e., 'error_flag' is FALSE), the function prints a completion message
 *     using 'printf' to indicate that the pre-assembly process has been successfully completed and displays the total number of
 *     macros found ('count_macro').
 *   - If errors were encountered during pre-assembly (i.e., 'error_flag' is TRUE), the function does not print anything.
 *   - The completion message is printed to the console, assuming that the standard output is connected to the console.
 *   - This function is specific to providing a user-friendly completion message for the pre-assembly process and does not affect
 *     the pre-assembly output files.
 */
static void print_end_of_pre_assembly();

void start_pre_assembly(ptr_file file_struct){
    sfile = file_struct;
    pre_assembly_on_curr_file();
}

static void pre_assembly_on_curr_file() {
    first_word_status status;

    /* Set the macro_flag to FALSE initially, indicating not inside a macro definition. */
    macro_flag = FALSE;

    /* Open the intermediate file with '.am' extension for writing pre-assembled code. */
    update_files();

    /* Process each line of the source file until the end of the file is reached. */
    while(update_next_line() != NULL){
        /* Increment the count of processed lines. */
        (sfile->count_line)++;

        /* Convert the current line to a line structure for easy access to individual words and information. */
        update_line_to_array();

        /* Determine the status of the first word in the line (e.g., macro name, macro start, macro end, or code text). */
        status = get_first_word_status();

        /* Based on the status, perform the appropriate action for the line. */
        action_by_status(status);

        /* Free the 'line_struct' representing the current line to release allocated memory (prevent memory leaks). */
        free_line(sfile->line_struct);
    }
    /* Free the linked list of macros ('head_macro_list') to release allocated memory used for storing macro information. */
    free_list_macro(&sfile->head_macro_list);

    /* Print a message indicating the successful completion of the pre-assembly process and the number of macros found and expanded. */
    print_end_of_pre_assembly();
}

static void update_files(){
    /* Open the intermediate file with '.am' extension for writing pre-assembled code. */
    sfile->file_am = open_file(sfile->name_file, EXT_MACRO,"w");
}

static char * update_next_line(){
    /* Read the next line of code from the source assembly file and store it in 'line_text' array. */
    return fgets(sfile->line_text,sizeof (sfile->line_text),sfile->file_as);
}

static void update_line_to_array(){
    /* Create a structured line representation for the current line of code from 'line_text'. */
    sfile->line_struct = create_new_line_struct(sfile->line_text);
}

static first_word_status get_first_word_status(){

    /* Check if the first word matches the name of an existing macro in the macro list. */
    temp_head_macro_list = search_in_list_macro(sfile->head_macro_list,sfile->line_struct->word1);
    if (temp_head_macro_list){
        return STATUS_MACRO_NAME;
    }

    /* Check if the first word is the predefined 'START_MACRO'. */
    if (strcmp(sfile->line_struct->word1, START_MACRO) == 0){
        /* If there is an ongoing macro definition, report an error (NESTED_MACRO_DEFINITION). */
        if (macro_flag == TRUE){
            add_error(NESTED_MACRO_DEFINITION);
        }
        /* Return 'STATUS_MCRO' to indicate the start of a new macro definition. */
        return STATUS_MCRO;
    }

    /* Check if the first word is the predefined 'END_MACRO'. */
    if (strcmp(sfile->line_struct->word1, END_MACRO) == 0){
        /* Return 'STATUS_ENDMCRO' to indicate the end of a macro definition. */
        return STATUS_ENDMCRO;
    }

    /* If 'macro_flag' is TRUE, the current line is part of a macro definition. */
    if(macro_flag == TRUE){
        /* Return 'STATUS_TEXT_OF_MACRO' to indicate that the line contains text belonging to the macro being defined. */
        return STATUS_TEXT_OF_MACRO;
    }
    /* If none of the above conditions are met, it is regular code text outside of any macro definition.
     * Return 'STATUS_TEXT_OF_CODE' to indicate that the line contains regular code text. */
    return STATUS_TEXT_OF_CODE;
}

static void action_by_status(first_word_status status){
    switch (status) {
        case STATUS_MACRO_NAME:
            /* Call 'paste_macro_text' to paste the content of an existing macro into the assembly file. */
            paste_macro_text();
            break;
        case STATUS_MCRO:
            /* Call 'update_name_of_macro' to update the name of the macro currently being defined. */
            update_name_of_macro();
            break;
        case STATUS_ENDMCRO:
            /* Call 'add_new_macro_to_list' to add a new macro (including its name and text) to the macro list. */
            add_new_macro_to_list();
            break;
        case STATUS_TEXT_OF_MACRO:
            /* Call 'add_text_to_macro' to add the current line's text to the text of the macro being defined. */
            add_text_to_macro();
            break;
        case STATUS_TEXT_OF_CODE:
            /* Call 'paste_code_text' to paste the current line's text into the assembly file as regular code text. */
            paste_code_text();
            break;
    }
}

static void add_error(error_code error_code){
    /* Call 'print_error' to display the error message associated with the 'error_code' and the current line number. */
    print_error(error_code,sfile->count_line);

    /* Set the 'error_flag' of the current file ('sfile') to TRUE to indicate that an error has occurred. */
    sfile->error_flag = TRUE;

    /* Increment the 'count_error' attribute of 'sfile' to keep track of the total number of errors encountered. */
    (sfile->count_error)++;
}

static void update_error_status(error_code error_code){
    /* Check if the 'error_code' is not equal to 'NO_ERROR'. If it is not 'NO_ERROR', handle the error using 'add_error'. */
    if (error_code != NO_ERROR){
        /* Call 'add_error' to handle the error and update the error status of the current file ('sfile'). */
        add_error(error_code);
    }
}

static void paste_macro_text(){
    /* Seek to the end of the macro file ('sfile->file_am') using 'fseek'. */
    fseek(sfile->file_am, 0, SEEK_END);

    /* Write the text of the current macro ('temp_head_macro_list->text_macro') to the end of the macro file using 'fputs'. */
    fputs(temp_head_macro_list->text_macro, sfile->file_am);
}

static void update_name_of_macro() {
    /* Set 'macro_flag' to TRUE to indicate that the pre-assembly process is currently inside a macro definition. */
    macro_flag = TRUE;

    /* Update the name of the current macro ('curr_macro_name') with the second word of the current line in the assembly file. */
    strcpy(curr_macro_name, sfile->line_struct->word2);
}

static void add_new_macro_to_list(){
    /* Set 'macro_flag' to FALSE to indicate that the pre-assembly process is not inside a macro definition. */
    macro_flag = FALSE;

    /* Increment the macro count to track the number of macros found in the file. */
    (sfile->count_macro)++;

    /* Check if the current macro name is a reserved word or not. */
    if (is_name_a_reserved_word(curr_macro_name) == FALSE){
        /* The current macro name is not a reserved word, so add the macro to the linked list. */
        update_error_status(add_to_list_macro(&sfile->head_macro_list, curr_macro_name, curr_macro_text));
    } else {
        /* The current macro name is a reserved word, so add an error to the error list. */
        add_error(MACRO_NAME_IS_INSTRUCTION_OR_DIRECTIVE);
    }

    /* Clear the 'curr_macro_text' and 'curr_macro_name' arrays for the next macro definition. */
    memset(curr_macro_text, 0, sizeof(curr_macro_text));
    memset(curr_macro_name, 0, sizeof(curr_macro_name));
}

static void add_text_to_macro(){
    /* Concatenate the text of the current line to the text of the current macro being defined. */
    strcat(curr_macro_text, sfile->line_text);
}

static void paste_code_text(){
    /* Pastes the current line of code text to the output assembly file (file_am). */
    fseek(sfile->file_am, 0, SEEK_END);
    fputs(sfile->line_text, sfile->file_am);
}

static void print_end_of_pre_assembly(){
    /* Print the completion message for the pre-assembly process if no errors were encountered. */
    if (sfile->error_flag == FALSE){
        printf("The pre-assembly process has been successfully completed. %d macro found.\n",sfile->count_macro);
    }
}