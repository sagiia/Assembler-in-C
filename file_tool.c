#include "file_tool.h"

ptr_file create_new_file_struct(char *name_file){
    /* Dynamically allocate memory for the new file struct */
    ptr_file new_file = (ptr_file)malloc(sizeof(item_file));
    if (new_file == NULL){
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Copy the provided 'name_file' into the new file struct's 'name_file' member */
    strcpy(new_file->name_file,name_file);

    /* Initialize 'line_text' with all zeroes */
    memset(new_file->line_text, 0, MAX_ASSEMBLY_LINE_LENGTH);

    /* Initialize other members with default values */
    new_file->current_line = 0;
    new_file->pos_in_line = 0;
    new_file->count_macro = 0;
    new_file->count_error = 0;
    new_file->count_line = 0;
    new_file->IC = FIRST_CELL_IN_MEMORY;
    new_file->DC = 0;
    new_file->error_flag = FALSE;
    new_file->extern_flag = FALSE;
    new_file->entry_flag = FALSE;

    /* Initialize 'data_array' and 'instruction_array' with all zeroes */
    memset(new_file->data_array, 0, sizeof(new_file->data_array));
    memset(new_file->instruction_array, 0, sizeof(new_file->instruction_array));

    /* Initialize other pointers to NULL */
    new_file->line_struct = NULL;
    new_file->head_macro_list = NULL;
    new_file->head_label_list = NULL;

    /* Open the file with the provided 'name_file' and 'as' extension in read mode */
    new_file->file_as = open_file(name_file,EXT_INPUT,"r");

    /* Return the pointer to the newly created file struct */
    return new_file;
}

void free_file(ptr_file file_struct){
    if (file_struct){
        free(file_struct); /* Free the memory occupied by the file struct */
    }
}

file_exists_status file_exists(char* name_file) {
    /* Check if the file name is too long */
    if (valid_file_name(name_file) == FALSE) {
        return TOO_LONG;
    }
    /* Check if the file exists using the 'access' function */
    if (access(get_file_with_extension(name_file, EXT_INPUT), F_OK) != -1) {
        return EXISTS; /* File exists */
    } else {
        return NO_EXISTS; /* File does not exist */
    }
}

bool valid_file_name(char *name_file) {
    if(strlen(name_file) > MAX_FILE_NAME_LENGTH){
        return FALSE; /* The file name is too long */
    } else {
        return TRUE; /* The name of the file is within the acceptable limits */
    }
}

char* get_file_with_extension(char *name_file, file_ext ext){
    /* Dynamically allocate memory for the full name */
    char *full_name = (char *) malloc(strlen(name_file) + MAX_FILE_EXTENSION_LENGTH);
    if (!full_name){
        fprintf(stderr, "Error in dynamic memory allocation");
        exit(EXIT_FAILURE);
    }

    /* Copying the base name to the new buffer */
    strcpy(full_name, name_file);

    /* Appending the appropriate extension based on the 'ext' value */
    switch (ext) {
        case EXT_MACRO: /* If the file extension is after the pre-assembler operation */
            strcat(full_name, ".am");
            break;
        case EXT_OBJECT: /* If the file extension is after the pass operation */
            strcat(full_name, ".ob");
            break;
        case EXT_EXTERN: /* If the file extension is for extern */
            strcat(full_name, ".ext");
            break;
        case EXT_ENTRY: /* If the file extension is for entry */
            strcat(full_name, ".ent");
            break;
        default: /* If the file extension is the original */
            strcat(full_name, ".as");
            break;
    }
    return full_name; /* Return the pointer to the full file name buffer */
}

FILE* open_file(char *name, file_ext ext, char permission[]) {
    FILE *file;
    char *full_name = get_file_with_extension(name, ext);

    /* Attempt to open the file using the provided 'permission' mode */
    if ((file = fopen(full_name, permission)) == NULL) {
        fprintf(stderr, "Error opening the file - %s\n", full_name);
        free(full_name); /* Free the dynamically allocated buffer for the full file name */
        exit(EXIT_FAILURE); /* Exit the program if the file cannot be opened */
    }

    free(full_name); /* Free the dynamically allocated buffer for the full file name */
    return file; /* Return the pointer to the opened file */
}

__attribute__((unused)) void print_file(ptr_file head){
    /* Print the file name and the line of text in the file */
    printf("Name file: %s\n",head->name_file);
    printf("line text: %s",head->line_text);

    /* Print various attributes of the file structure */
    printf("Current line: %d\n", head->current_line);
    printf("Pos in line: %d\n", head->pos_in_line);
    printf("Count error: %d\n", head->count_error);
    printf("Count line: %d\n", head->count_line);
    printf("IC: %d\n", head->IC);
    printf("DC: %d\n", head->DC);

    /* Print flags indicating the presence of certain conditions */
    if (head->error_flag == TRUE){
        puts("Error flag = TRUE");
    } else {
        puts("Error flag = FALSE");
    }
    if (head->extern_flag == TRUE){
        puts("Extern flag = TRUE");
    } else {
        puts("Error flag = FALSE");
    }
    if (head->entry_flag == TRUE){
        puts("Entry_flag = TRUE");
    } else {
        puts("Error flag = FALSE");
    }

    /* Print the content of the 'line_struct' linked list */
    puts("Line struct:");
    print_line(head->line_struct);

    /* Print the content of the 'head_macro_list' linked list */
    puts("Macro list:");
    print_list_macro(head->head_macro_list);

    /* Print the content of the 'head_label_list' linked list */
    puts("Label list:");
    print_list_label(head->head_label_list);
}