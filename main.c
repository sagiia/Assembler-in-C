#include "main.h"

/*
 * Function: main
 * --------------
 * The main function is the entry point of the assembly program.
 * It takes command-line arguments (file names) and initiates the assembly process.
 *
 * Parameters:
 *   argc: The number of command-line arguments, including the program name itself.
 *   argv: An array of pointers to strings, where each element represents a command-line argument.
 *
 * Returns:
 *   int: The return status of the program. It returns 0 on successful completion.
 *
 * Notes:
 *   - If no files are supplied to the program, it displays an error message and exits.
 *   - The assembly process will be carried out on each file provided in the command-line arguments.
 */
int main(int argc, char **argv) {
    /* Start of the project process */
    start_assembly(argc, argv);
    return 0;
}

void start_assembly(int countFiles, char **arrayFiles) {
    int i;
    file_exists_status file_status;

    /* If no files are supplied as command-line arguments */
    if (countFiles == 1) {
        puts("");
        print_red();
        fprintf(stderr, "Error, assembly files should be provided.\n");
        puts("");
        exit(EXIT_FAILURE);
    }

    /* Iterate through all the files provided as command-line arguments */
    for (i = 1; i < countFiles; i++) {

        puts("");
        puts("--------------------------------------------------------------------------------");
        printf("File Name: %s:\n\n", arrayFiles[i]);

        file_status = file_exists(arrayFiles[i]);
        switch (file_status) {
            case EXISTS: /* If the file exists, start the assembly process for the current file */
                start_assembly_process_on_file(arrayFiles[i]);
                break;
            case TOO_LONG: /* If the file name is too long, print an error message and skip processing this file */
                print_red();
                printf("ERROR- The file name is too long!\n");
                print_reset();
                break;
            case NO_EXISTS: /* If the file does not exist, print an error message and skip processing this file */
                print_red();
                printf("ERROR- The file was not found!\n");
                print_reset();
                break;
        }
    }
    /* Print a separator line to signify the end of the assembly process */
    puts("");
    puts("--------------------------------------------------------------------------------");
}

void start_assembly_process_on_file(char* name_file) {
    ptr_file file_struct;

    /* Create a new file structure to manage the assembly process for the current file */
    file_struct = create_new_file_struct(name_file);

    /* Perform pre-assembly operations to handle comments, white spaces, and macros */
    start_pre_assembly(file_struct);

    /* Execute the first pass of the assembly process to construct the symbol table and calculate addresses */
    start_first_pass(file_struct);

    /*execute the second pass */
    start_second_pass(file_struct);

    /* Print the result of the assembly process for the current file */
    print_end_of_file(file_struct);

    /* Free the memory allocated for the file structure to release resources */
    free_file(file_struct);
}

void print_end_of_file(ptr_file file_struct){
    /* Print success message if no errors were encountered */
    if (file_struct->error_flag == FALSE){
        printf("\nCompilation completed successfully.\n");
        printf("Lines parsed into file: %d.\n", file_struct->IC + file_struct->DC - FIRST_CELL_IN_MEMORY);

    /* Print error message if there were errors during the assembly process */
    } else {
        printf("\nNumber of errors: %d.\n", file_struct->count_error);
        printf("Compilation not completed.\n");
    }
}