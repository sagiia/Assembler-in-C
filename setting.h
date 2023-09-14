/*
 * Header: setting.h
 * -----------------
 * This header file contains various settings and constants used in the assembly program.
 * It defines macros for memory cell addresses, file-related limitations, array sizes, line lengths,
 * label name length, maximum characters in an assembly file, and other constants used throughout the program.
 * Additionally, it defines the boolean data type with named constants TRUE and FALSE to represent true or false values.
 * These settings and constants are used to configure and control various aspects of the assembly program.
 *
 * Notes:
 *   - The settings and constants in this header file can be easily modified to adapt the assembly program's behavior.
 *   - The boolean data type simplifies conditional expressions and enhances code clarity when dealing with true/false values.
 */

#ifndef SETTING_H
#define SETTING_H

/* The first memory cell address for the program */
#define FIRST_CELL_IN_MEMORY 100

/* Maximum length for a file name, including the extension (Linux limitation) */
#define MAX_FILE_NAME_LENGTH 255

/* Maximum length for a file extension */
#define MAX_FILE_EXTENSION_LENGTH 4

/* Maximum size of an array(instruction or directive) */
#define MAX_ARRAY_SIZE 924

/* Maximum length of an assembly line */
#define MAX_ASSEMBLY_LINE_LENGTH 82

/* Maximum length of a label name */
#define MAX_NAME_LABEL_LENGTH 32

/* Maximum number of characters in an assembly file */
#define MAX_CHARACTERS_ASSEMBLY_FILE 8000

/* Maximum number of digits in an address */
#define MAX_DIGITS_FOR_ADDRESS 4

/* Maximum line length in base-64 */
#define BASE64_CHAR_LENGTH 4

/* Base for mathematical operations */
#define BASE_POW 2

/*
 * Enum: bool
 * -----------
 * This enumeration defines a boolean data type with two named constants: TRUE and FALSE.
 * The boolean data type is commonly used to represent truth values, where TRUE is associated with the value 'true',
 * and FALSE is associated with the value 'false'.
 *
 * Notes:
 *   - The enumeration 'bool' simplifies the representation of true/false values and improves code readability.
 */
typedef enum {
    TRUE,   /* Represents the boolean value 'true' */
    FALSE   /* Represents the boolean value 'false' */
} bool;


#endif /* SETTING_H */