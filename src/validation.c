#include "validation.h"
#include "app.h" // include the app header to flush the buffer after scanf()

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

/**
 * Prompts the user to enter a valid axiom string for an L-System.
 *
 * The function validates the input based on specific requirements:
 * - The axiom must be between 1 and 15 characters long.
 * - The axiom cannot contain spaces.
 * - The axiom can only include letters and the symbols '+', '-', '[', and ']'.
 * 
 * If the input does not meet these criteria, the user is prompted again 
 * until a valid axiom is entered. The validated axiom is then copied to 
 * the provided axiom parameter.
 *
 * @param axiom A character array where the validated axiom will be stored.
 */
void validate_axiom(char* axiom) { // validate the inputted axiom
    char input[100];
    int length;

    printf("Enter the axiom (see above key for requirements): ");

    while (1) {
        if (fgets(input, sizeof(input), stdin)) { 
            input[strcspn(input, "\n")] = 0;
            length = strlen(input);
    
            if (length == 0) { 
                printf("\nERROR: Axiom must be more than 0 characters long, try again: ");
            } else if (length > 15) {
                printf("\nERROR: Axiom must be less than or equal to 10 characters long, try again: ");
            } else if (strchr(input, ' ') != NULL) {
                printf("\nERROR: Axiom must not contain spaces, try again: ");
            } else {
                int valid = 1;
                for (int i = 0; i < length; i++) {
                    if (!isalpha(input[i]) && input[i] != '+' && input[i] != '-' && input[i] != '[' && input[i] != ']') {
                        valid = 0;
                        break; // ensure only valid characters in the axiom
                    }
                }
                
                if (!valid) {
                    printf("\nERROR: Axiom must contain only allowed characters, try again: ");
                } else {
                    strcpy(axiom, input);
                    break;
                }
            }
        }
    }
}

/**
 * Finds the indices of the characters that need rules in the axiom.
 *
 * The function iterates through the axiom and checks each character. If the
 * character is a letter and has not been seen before, it is added to the array
 * of indices. The function also keeps track of which characters have been seen
 * in order to avoid duplicates.
 *
 * @param axiom The axiom string to be searched.
 * @param rules_indices An array to store the indices of the characters that
 *        need rules.
 */
void rules_for(char *axiom, int *rules_indices) { // find the indices of the characters that need rules in the axiom
    int index_range = strlen(axiom);
    int rules_index = 0;
    int seen[256] = {0};
    
    for (int i = 0; i < index_range; i++) {
        if (isalpha(axiom[i]) && !seen[toupper(axiom[i])]) {
            rules_indices[rules_index++] = i;
            seen[toupper(axiom[i])] = 1;
        }
    }
    
    rules_indices[rules_index] = -1;
}

/**
 * Validates a single rule struct.
 * 
 * Checks that the rule does not contain spaces and is not longer than 15 characters.
 * Also checks that the rule only contains letters and the symbols +, -, [, and ].
 * 
 * @param rule The rule string to be validated.
 * 
 * @return 1 if the rule is valid, 0 otherwise.
 */
int validate_single_rule(char* rule) { // validate a single rule struct
    if (strchr(rule, ' ') != NULL) {
        printf("ERROR: Rule cannot contain spaces, try again.\n");
        return 0;
    }
    
    if (strlen(rule) > 15) {
        printf("ERROR: Rule cannot be longer than 15 characters, try again.\n");
        return 0;
    }
    
    for (int i = 0; i < strlen(rule); i++) {
        if (!isalpha(rule[i]) && rule[i] != '+' && rule[i] != '-' && rule[i] != '[' && rule[i] != ']') {
            printf("ERROR: Rule can only contain letters and the symbols +, -, [, and ], try again.\n");
            return 0;
        }
    }
    
    return 1;
}

/**
 * Validates and populates the rules for an L-System based on a given axiom.
 *
 * This function ensures that each character in the axiom has a corresponding
 * rule in the provided rules array. It prompts the user to enter a rule for
 * each character that does not already have one, validating the input based
 * on specific criteria. The function also accounts for any new characters
 * introduced in the entered rules that require further rules.
 *
 * The process continues until all required characters have associated rules.
 * The rules are stored in the provided rules array, with each rule's character
 * and transformation string.
 *
 * @param rules An array of Rule structs to be populated with validated rules.
 * @param axiom The axiom string containing characters that need rules.
 * @param indices An array of indices indicating which characters in the axiom
 *        require rules.
 */

void validate_rules(Rule* rules, char* axiom, int* indices) { // validate all of the rules for the system 
    int size = 0;
    while (indices[size] != -1) {
        size++;
    }
    
    char input[SIZE + 1];
    int has_rule[256] = {0};    
    int pending[256] = {0}; 
    
    for (int i = 0; i < size; i++) {
        pending[axiom[indices[i]]] = 1;
    }
    
    int rule_index = 0;
    int done = 0;
    
    while (!done) {
        done = 1; 
        
        for (int c = 0; c < 256; c++) { // ensure that all characters have rules
            if (pending[c] && !has_rule[c]) {
                done = 0;  
                
                int valid_rule = 0;
                while (!valid_rule) {
                    printf("Enter rule for character '%c': ", (char)c);
                    
                    fgets(input, SIZE + 1, stdin);
                    input[strcspn(input, "\n")] = 0;
                    
                    valid_rule = validate_single_rule(input);
                }
                
                rules[rule_index].character = (char)c;
                strcpy(rules[rule_index].rule, input);
                rule_index++;
                
                has_rule[c] = 1;
                pending[c] = 0;
                
                for (int i = 0; i < strlen(input); i++) {
                    char new_c = input[i];
                    if (isalpha(new_c) && !has_rule[new_c]) {
                        if (!pending[new_c]) {
                            pending[new_c] = 2;
                        }
                    }
                }
                
                break;
            }
        }
    }
}

/**
 * Prompts the user to enter the number of iterations for an L-System.
 *
 * The function validates the input based on specific requirements:
 * - The number of iterations must be a positive integer.
 * - The number of iterations must be less than or equal to 8.
 *
 * If the input does not meet these criteria, the user is prompted again
 * until a valid number of iterations is entered. The validated number
 * of iterations is then returned.
 */
int validate_iterations() { // validate the number of iterations
    int input; 

    printf("Enter the number of iterations (see above key for requirements): ");

    while (1) {
        if (scanf("%d", &input)) {
            if (input <= 0) {
                printf("\nERROR: Iterations must be a positive integer, try again: ");
            } else if (input > 8) {
                printf("\nERROR: Iterations must be less than or equal to 8, try again: ");
            } else {
                return input;
            }
        } else {
            printf("\nERROR: Invalid input, try again: ");
            flush_buffer();
        }
    }
}

/**
 * Prompts the user to enter a valid float value for either the turn angle or starting direction.
 *
 * The function validates the input based on specific requirements:
 * - The input must be a float greater than or equal to 0.
 * - The input must be less than or equal to 360.
 * 
 * If the input does not meet these criteria, the user is prompted again
 * until a valid value is entered. The validated input is then returned.
 *
 * @param data An integer that specifies which prompt to display: 
 *             1 for turn angle, otherwise for starting direction.
 * @return The validated float value for the specified parameter.
 */

float validate_turn_and_start(int data) { // validate the turn angle and starting direction
    float input; 
    
    if (data == 1) {
        printf("Enter the turn angle (see above key for requirements): ");
    } else {
        printf("Enter the starting direction (see above key for requirements): ");
    }

    while(1) {
        if (scanf("%f", &input)) {
            if (input < 0) {
                printf("\nERROR: Number must be greater than or equal to 0, try again: ");
            } else if (input >= 361) {
                printf("\nERROR: Number must be less than or equal to 360, try again: ");
            } else {
                return input;
            }
        } else {
            printf("\nERROR: Invalid input, try again: ");
            flush_buffer();
        }
    }
}
