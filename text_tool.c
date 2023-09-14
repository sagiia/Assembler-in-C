#include "text_tool.h"

/* Function: initialize_variables (Static Function)
 * -----------------------------------------------
 * Initializes the fields of a line struct to zero.
 *
 * This function takes a pointer to a line struct 'line_struct' and resets all fields of the struct
 * to zero or empty strings. This function is intended for internal use within the 'create_new_line_struct'
 * function to initialize the newly created line struct.
 *
 * Parameters:
 *   - line_struct: A pointer to the line struct to be initialized.
 *
 * Remarks:
 *   - The 'initialize_variables' function is called to ensure that all fields of the newly created line struct
 *     have a known and consistent initial state. This helps avoid issues with uninitialized memory.
 *   - The function uses the 'memset' function to set all fields of the line struct to zero or empty strings.
 *   - The 'initialize_variables' function is declared as a static function, which means it is only accessible
 *     within the scope of this source file and cannot be used directly from other source files.
 */
static void initialize_variables(ptr_line line_struct);

/* Function: fix_comma_in_line
 * ---------------------------
 * Modifies a line of assembly code to add spaces around commas.
 *
 * Parameters:
 *   - line: A pointer to a string containing the original line of assembly code.
 *
 * Returns:
 *   - char*: A pointer to a string containing the modified line with spaces around commas. This string is
 *            stored in a static buffer, and the caller should not attempt to modify or free the memory.
 *            The buffer is reused for each call to this function, so the contents are not persistent.
 *
 * Notes:
 *   - The function scans through the input 'line' character by character and replaces any commas encountered
 *     with a comma surrounded by spaces. For example, "ADD R1,R2" becomes "ADD R1 , R2" after modification.
 *   - The function ensures that the modified line ends with a newline character '\n' and a null terminator '\0'
 *     to ensure compatibility with standard C strings.
 *   - The function is designed to handle assembly code lines of up to 'MAX_ASSEMBLY_LINE_LENGTH' characters in length.
 *     If the input line exceeds this length, the behavior is undefined.
 *   - The function does not modify the original input 'line'.
 */
static char * fix_comma_in_line(const char *line);

/* Function: is_register
 * ---------------------
 * Checks if the given text represents a valid register in assembly code.
 *
 * This function takes a pointer to a character array 'text', representing a string of text in assembly code.
 * It checks whether the 'text' matches any of the predefined register names ('@r1', '@r2', ..., '@r7').
 * If 'text' matches any of the register names, the function returns 'TRUE'; otherwise, it returns 'FALSE'.
 *
 * Parameters:
 *   - text: A pointer to a character array (string) representing the text to be checked for a register match.
 *
 * Returns:
 *   - bool: 'TRUE' if 'text' represents a valid register ('@r1', '@r2', ..., '@r7'), 'FALSE' otherwise.
 *
 * Notes:
 *   - The function is case-sensitive; register names must be in lowercase (e.g., "@r1") to be recognized as valid.
 *   - The function is 'static', meaning it is only accessible within the current translation unit (source file).
 *     This is done to limit its visibility to other parts of the program and ensure it is only used internally.
 *     If you need to use this function in other files, you may consider moving it to a shared header file.
 */
static bool is_register(char * text);

/* Function: get_specific_bits
 * ---------------------------
 * Extracts a specific sequence of bits from an unsigned integer word.
 *
 * This function is used to extract a specific sequence of bits from the input 'word', an unsigned integer. The 'start'
 * and 'end' parameters define the range of bits to be extracted, both inclusive. The extracted sequence is then
 * returned as an unsigned integer.
 *
 * Parameters:
 *   - word: The unsigned integer containing the bit sequence from which bits are to be extracted.
 *   - start: The starting position (index) of the bit sequence to be extracted (0-based index).
 *   - end: The ending position (index) of the bit sequence to be extracted (0-based index).
 *
 * Returns:
 *   - unsigned int: An unsigned integer containing the extracted bit sequence.
 *
 * Notes:
 *   - The function uses bit manipulation to extract the specific bit sequence efficiently.
 *   - It calculates the length of the bit sequence to be extracted and creates a corresponding mask with '1's in the
 *     relevant positions to retain the bits of interest while setting other bits to 0.
 *   - The function then applies the mask to the original 'word' using bitwise AND and shifts the extracted sequence
 *     to the least significant bits (LSB) position to obtain the final result.
 *   - The function is primarily used as a helper function within other functions that require specific bits from a word.
 */
static unsigned int get_specific_bits(unsigned int word, int start, int end);

/* Function: pow_math
 * ------------------
 * Calculates the result of an exponentiation operation with integer inputs.
 *
 * This function calculates the result of 'BASE_POW' raised to the power of 'exp', where 'BASE_POW' is a constant integer
 * value representing the base of the exponentiation. The exponent 'exp' is a non-negative integer that determines how
 * many times 'BASE_POW' should be multiplied by itself.
 *
 * Parameters:
 *   - exp: The exponent to which 'BASE_POW' is raised. It must be a non-negative integer.
 *
 * Returns:
 *   - int: The result of 'BASE_POW' raised to the power of 'exp'.
 *
 * Example:
 *   - Suppose 'BASE_POW' is 2, and 'exp' is 3. The function call would be: 'pow_math(3)'. The result of 2^3 is 8, so the
 *     function will return 8.
 *
 * Notes:
 *   - The function uses bitwise operations to efficiently calculate the exponentiation result.
 *   - If the input 'exp' is negative, the function will return -1 to indicate an invalid input.
 *   - The function is primarily used as a helper function within other functions that require exponentiation.
 */
static int pow_math(int exp);

ptr_line create_new_line_struct(const char *text_line){
    int i = 0; /* Line index */
    int j; /* Word index */
    static char temp_word[MAX_ASSEMBLY_LINE_LENGTH] = "";
    static char temp_line[MAX_ASSEMBLY_LINE_LENGTH] = "";
    ptr_line new_line;

    /* Fix comma-related issues in the input line */
    strcpy(temp_line, fix_comma_in_line(text_line));

    /* Dynamic memory allocation for the new line struct */
    new_line = (ptr_line)malloc(sizeof(item_line)); /* Dynamic memory allocation */
    if (new_line == NULL){ /* Checking whether the memory allocation was successful */
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Initialize all variables in the new line struct to zero */
    initialize_variables(new_line);

    i = skip_white_character(i,text_line);

    /* If the line is empty (only contains a white character), set 'count' to zero and return */
    if(temp_line[i] == '\n'){
        new_line ->count = ZERO;
        return new_line;
    }

    /* Macro to extract individual words from 'temp_line' and store them in the line struct */
    GET_WORD(word1, ONE)
    GET_WORD(word2, TWO)
    GET_WORD(word3, TREE)
    GET_WORD(word4, FOUR)
    GET_WORD(word5, FIVE)

    /* Set 'source' and 'destination' flags to 'NOT_EXIST', and 'count' to 'TOO_MUCH' indicating more than five words */
    new_line->source = NOT_EXIST;
    new_line->destination = NOT_EXIST;
    new_line ->count = TOO_MUCH;
    return new_line;
}

static void initialize_variables(ptr_line line_struct){
    memset(line_struct->word1, 0, MAX_ASSEMBLY_LINE_LENGTH);
    memset(line_struct->word2, 0, MAX_ASSEMBLY_LINE_LENGTH);
    memset(line_struct->word3, 0, MAX_ASSEMBLY_LINE_LENGTH);
    memset(line_struct->word4, 0, MAX_ASSEMBLY_LINE_LENGTH);
    memset(line_struct->word5, 0, MAX_ASSEMBLY_LINE_LENGTH);
    line_struct->count = 0;
}

void delete_label_from_line_struct(ptr_line line_struct){
    /* Copy the content of each word to the previous word, effectively deleting the first word */
    strcpy(line_struct->word1,line_struct->word2);
    strcpy(line_struct->word2,line_struct->word3);
    strcpy(line_struct->word3,line_struct->word4);
    strcpy(line_struct->word4,line_struct->word5);

    /* Clear the content of the fifth word (set all characters to null) */
    memset(line_struct->word5, 0, MAX_ASSEMBLY_LINE_LENGTH);

    /* If the line struct's 'count' indicates that there were less than five words originally, decrement the 'count' */
    if (line_struct->count != TOO_MUCH){
        (line_struct->count)--;
    }
}

void free_line(ptr_line line_struct){
    /* Check if 'line_struct' is not NULL before attempting to free the memory */
    if (line_struct != NULL){
        free(line_struct);
    }
}

static char * fix_comma_in_line(const char *line){
    int i = 0; /* Index for scanning the input line */
    int j = 0; /* Index for building the modified line */

    /* Buffer to store the modified line with spaces around commas */
    static char temp_line[MAX_ASSEMBLY_LINE_LENGTH] = "";

    /* Scan through the input 'line' and modify commas to include spaces around them */
    while (line[i] != '\n' && line[i] != '\0'){
        if (line[i] == ','){
            /* Add a space before the comma */
            temp_line[j] = ' ';
            j++;

            /* Copy the comma to the modified line */
            temp_line[j] = line[i];
            j++;

            /* Add a space after the comma */
            temp_line[j] = ' ';
            j++;
        } else {
            /* Copy non-comma characters as they are */
            temp_line[j] = line[i];
            j++;
        }

        /* Move to the next character in the input line */
        i++;
    }
    /* Ensure the modified line ends with a newline character and a null terminator */
    temp_line[j] = '\n';
    j++;
    temp_line[j] = '\0';

    /* Return a pointer to the modified line (stored in the static buffer) */
    return  temp_line;
}

int skip_white_character(int curr_position, const char *text){
    /* Scan through 'text' from 'curr_position' until a non-white space character is encountered */
    while (text[curr_position] == ' ' || text[curr_position] == '\t') {
        curr_position++;
    }

    /* Return the updated position after skipping over white space characters */
    return curr_position;
}

int skip_one_word_in_line(int curr_position, const char *line_text){
    /* Skip over leading white space characters */
    curr_position = skip_white_character(curr_position, line_text);

    /* Scan through 'line_text' until the end of the current word is reached */
    while (line_text[curr_position] != ' ' && line_text[curr_position] != '\t' && line_text[curr_position] != '\0'){
        (curr_position)++;
    }

    /* Return the updated position after skipping over the current word */
    return curr_position;
}

bool is_end_line(int curr_position, const char *line_text){
    /* Scan through 'line_text' until the end of the line or a non-white space character is encountered */
    while (line_text[curr_position] != '\0' && line_text[curr_position] != '\n'){
        if (line_text[curr_position] == '\t' || line_text[curr_position] == ' '){
            curr_position++;
        } else {
            return FALSE; /* Line is not empty if a non-white space character is encountered */
        }
    }

    /* Return 'true' if the end of the line is reached without encountering non-white space characters */
    return TRUE;
}


bool is_label(ptr_line line_struct){
    int i = 0;
    char temp_first_word[MAX_ASSEMBLY_LINE_LENGTH];
    strcpy(temp_first_word, line_struct->word1);

    /* Find the last character in the first word */
    while (temp_first_word[i + 1] != '\0'){
        i++;
    }

    /* Check if the last character is a colon (':'), indicating a label */
    if(temp_first_word[i] == ':') {
        /* Remove the colon by replacing it with the null terminator */
        line_struct->word1[i] = '\0';
        return TRUE; /* The first word is a label */
    }
    return FALSE; /* The first word is not a label */
}

bool is_number(char * text){
    int i = 0;

    /* Skip leading sign if present ('+' or '-') */
    if (text[i] == '+' || text[i] == '-'){
        i++;
    }

    /* Check each character in 'text' for being a digit */
    while (text[i] != '\0' && text[i] != '\n'){
        /* If a non-digit character is found, 'text' is not a valid numeric value */
        if (!isdigit(text[i])){
            return FALSE;
        }
        i++;
    }

    return TRUE;
}

bool is_line_empty(const char *line_text){
    int i = 0;

    i = skip_white_character(i,line_text);
    /* Check if the line starts with a comment (denoted by ';') */
    if (line_text[i] == ';'){
        return TRUE;
    }

    /* Iterate through the characters in 'line_text' to check for whitespace characters */
    while(line_text[i] != '\0' && line_text[i] != '\n'){
        /* If a non-whitespace character is found, the line is not empty */
        if (line_text[i] != ' ' && line_text[i] != '\t'){
            return FALSE;
        }
        i++;
    }

    /* The line is empty or contains only whitespace characters */
    return TRUE;
}

static bool is_register(char *text) {
    /* List of valid register names in assembly (e.g., '@r1', '@r2', ..., '@r7') */
    if (strcmp(text, "@r1") == 0 || strcmp(text, "@r2") == 0 ||
        strcmp(text, "@r3") == 0 || strcmp(text, "@r4") == 0 ||
        strcmp(text, "@r5") == 0 || strcmp(text, "@r6") == 0 ||
        strcmp(text, "@r7") == 0 || strcmp(text, "@r0") == 0) {
        /* The text matches one of the valid register names */
        return TRUE;
    }
    /* The text does not match any of the valid register names */
    return FALSE;
}

bool is_label_name_valid(char *name_label){
    int i = 0;
    int count_chars = 0;

    /* Check if 'name_label' is a reserved word; if so, it is not a valid label name */
    if (is_name_a_reserved_word(name_label) == TRUE){
        return FALSE;
    }

    /* Check if the first character of 'name_label' is an alphabet character */
    if (isalpha(name_label[i]) == 0 ){
        return FALSE;
    }

    /* Iterate through 'name_label' to check for valid characters and count the number of characters */
    while (name_label[i] != '\n' && name_label[i] != '\0' && count_chars <= MAX_NAME_LABEL_LENGTH){
        /* 'name_label' contains a non-alphanumeric character, so it is not a valid label name */
        if (isalpha(name_label[i]) == 0 && isdigit(name_label[i]) == 0){
            return FALSE;
        }
        count_chars++;
        i++;
    }

    /* Check if 'name_label' exceeds the maximum allowable label name length */
    if (count_chars == MAX_NAME_LABEL_LENGTH){
        return FALSE;
    } else {
        return TRUE;
    }
}

bool is_name_a_reserved_word(char *name){
    if (strcmp(name, ".data") == 0) { return TRUE; }
    if (strcmp(name, ".string") == 0) { return TRUE; }
    if (strcmp(name, ".entry") == 0) { return TRUE; }
    if (strcmp(name, ".extern") == 0) { return TRUE; }
    if (strcmp(name, "@r0") == 0) { return TRUE; }
    if (strcmp(name, "@r2") == 0) { return TRUE; }
    if (strcmp(name, "@r3") == 0) { return TRUE; }
    if (strcmp(name, "@r4") == 0) { return TRUE; }
    if (strcmp(name, "@r5") == 0) { return TRUE; }
    if (strcmp(name, "@r6") == 0) { return TRUE; }
    if (strcmp(name, "@r7") == 0) { return TRUE; }
    if (strcmp(name, "mov") == 0) { return TRUE; }
    if (strcmp(name, "cmp") == 0) { return TRUE; }
    if (strcmp(name, "add") == 0) { return TRUE; }
    if (strcmp(name, "sub") == 0) { return TRUE; }
    if (strcmp(name, "not") == 0) { return TRUE; }
    if (strcmp(name, "clr") == 0) { return TRUE; }
    if (strcmp(name, "lea") == 0) { return TRUE; }
    if (strcmp(name, "inc") == 0) { return TRUE; }
    if (strcmp(name, "dec") == 0) { return TRUE; }
    if (strcmp(name, "jmp") == 0) { return TRUE; }
    if (strcmp(name, "bne") == 0) { return TRUE; }
    if (strcmp(name, "red") == 0) { return TRUE; }
    if (strcmp(name, "prn") == 0) { return TRUE; }
    if (strcmp(name, "jsr") == 0) { return TRUE; }
    if (strcmp(name, "rts") == 0) { return TRUE; }
    if (strcmp(name, "stop") == 0) { return TRUE; }
    return FALSE;
}


instruction_type get_instruction_type(const char *word){
    if (strcmp(word, "mov") == 0) { return MOV; }
    if (strcmp(word, "cmp") == 0) { return CMP; }
    if (strcmp(word, "add") == 0) { return ADD; }
    if (strcmp(word, "sub") == 0) { return SUB; }
    if (strcmp(word, "not") == 0) { return NOT; }
    if (strcmp(word, "clr") == 0) { return CLR; }
    if (strcmp(word, "lea") == 0) { return LEA; }
    if (strcmp(word, "inc") == 0) { return INC; }
    if (strcmp(word, "dec") == 0) { return DEC; }
    if (strcmp(word, "jmp") == 0) { return JMP; }
    if (strcmp(word, "bne") == 0) { return BNE; }
    if (strcmp(word, "red") == 0) { return RED; }
    if (strcmp(word, "prn") == 0) { return PRN; }
    if (strcmp(word, "jsr") == 0) { return JSR; }
    if (strcmp(word, "rts") == 0) { return RTS; }
    if (strcmp(word, "stop") == 0) { return STOP; }
    return NOT_INSTRUCTION;
}

addressing_method get_addressing_method_type(char *word){
    if (word[0] == '\0') { return NOT_EXIST; }
    if (is_number(word) == TRUE) { return IMMEDIATE; }
    if (is_register(word) == TRUE) { return REGISTER; }
    return DIRECT;
}

int get_number_of_register(char *register_name) {
    return (register_name[2] - '0');
}

char * convert_binary_to_64base(unsigned int word){
    /* Base64 encoding lookup table */
    char base64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        '4', '5', '6', '7', '8', '9', '+', '/'};

    /* Allocate memory for the base64 representation */
    char *word_in_64base = (char *) malloc(BASE64_CHAR_LENGTH);
    if (word_in_64base == NULL){
        /* Memory allocation failure */
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }
    /* Convert the 6-bit binary word to base64 representation */
    word_in_64base[0] = base64_table[get_specific_bits(word, 6, 11)];
    word_in_64base[1] = base64_table[get_specific_bits(word,0,5)];
    word_in_64base[2] = '\n'; /* Add newline character */
    word_in_64base[3] = '\0'; /* Null terminator to indicate the end of the string */
    return word_in_64base;
}

static unsigned int get_specific_bits(unsigned int word, int start, int end){
    unsigned int result;
    int length = end - start + 1; /* Length of bit-sequence */

    /* Create a mask with 'length' number of 1's (e.g., 000...0111...1) */
    unsigned int mask = pow_math(length) - 1;

    /* Shift the mask to the position of the bit sequence to be extracted */
    mask <<= start;

    /* Apply the mask to 'word' using bitwise AND to extract the specific bits */
    result = word & mask;

    /* Shift the extracted sequence to the least significant bits (LSB) position */
    result >>= start;
    return result;
}

static int pow_math(int exp){
    int base = BASE_POW;
    int result = 1;

    /* Check if the exponent is negative (invalid input) and return -1 */
    if(exp < 0){
        return -1;
    }

    /* Perform binary exponentiation algorithm to calculate the result */
    while (exp){
        if (exp & 1){ /* If the current bit of 'exp' is 1 */
            result *= base; /* Multiply the result by the current 'base' value */
        }
        exp >>= 1; /* Right shift 'exp' to move to the next bit */
        base *= base; /* Square the 'base' value for the next iteration */
    }
    return result;
}

void print_red(){
    /*printf("\033[1;31m");*/
}

void print_reset(){
    /*printf("\033[0m");*/
}

__attribute__((unused)) void print_line(ptr_line line_struct){
    printf("%s ",line_struct->word1);
    printf("%s ",line_struct->word2);
    printf("%s ",line_struct->word3);
    printf("%s ",line_struct->word4);
    printf("%s ",line_struct->word5);
    printf("source: %d ",line_struct->source);
    printf("destination: %d ",line_struct->destination);
    printf("count: %d\n",line_struct->count);
}