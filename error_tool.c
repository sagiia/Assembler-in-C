#include "error_tool.h"

const char* error_messages[] = {
        /* NO_ERROR */ "No error.",
        /* LABEL_ALREADY_EXISTS */ "A label cannot be declared more than once.",
        /* MACRO_ALREADY_EXISTS */ "A macro cannot be declared more than once.",
        /* COMMA_REQUIRED_BETWEEN_VALUES */ "A comma is required between operands.",
        /* DATA_NEED_NUM_VALUE */ "The data directive accepts only numbers.",
        /* CANT_DEFINE_LABEL_BEFORE_ENTRY */ "It is not possible to define a label before an entry directive.",
        /* CANT_DEFINE_LABEL_BEFORE_EXTERN */ "It is not possible to define a label before an extern directive.",
        /* STRING_STRUCTURE_NOT_VALID */ "String should start with quotes.",
        /* STRING_MUST_END_IN_QUOTES */ "String should end with quotes.",
        /* STRING_DIRECTIVE_ACCEPTS_ONE_PARAMETER */ "The string directive takes one argument.",
        /* TOO_MUCH_WORDS_FOR_INSTRUCTION */ "Too many words for instruction.",
        /* CANT_FIND_LABEL_TO_ENTRY */ "The entry label was not found.",
        /* INVALID_LABEL_NAME */ "The label name is invalid.",
        /* INSTRUCTION_NAME_NOT_EXIST */ "Instruction does not exist.",
        /* INSTRUCTION_SHOULD_RECEIVE_TWO_OPERANDS */ "The instruction should receive two operands.",
        /* COMMA_REQUIRED_BETWEEN_OPERANDS */ "A comma is required between two operands.",
        /* INSTRUCTION_SHOULD_RECEIVE_ONE_OPERAND */ "The instruction should receive one operand.",
        /* INSTRUCTION_SHOULD_NOT_RECEIVE_OPERANDS */ "The instruction should not accept operands.",
        /* INVALID_ADDRESS_METHOD_FOR_INSTRUCTION */ "The instruction cannot receive this operand.",
        /* MUST_PROVIDE_LABELS_TO_EXTERN */ "Must provide labels to extern directive.",
        /* MUST_PROVIDE_LABELS_TO_ENTRY */ "Must provide labels to entry directive.",
        /* MUST_PROVIDE_VALUES_TO_DATA */ "Must provide values to data directive.",
        /* INVALID_COMMA_POSITION */ "Invalid comma position.",
        /* LABEL_NOT_FOUND */ "The label does not found.",
        /* NESTED_MACRO_DEFINITION */ "You cannot define a nested macro.",
        /* MACRO_NAME_IS_INSTRUCTION_OR_DIRECTIVE */ "The macro name is a reserved instruction or directive."
};

void print_error(error_code code, int line) {
    print_red();
    printf("Error in line %d", line);
    print_reset();
    printf(" - %s\n", error_messages[code]);
}