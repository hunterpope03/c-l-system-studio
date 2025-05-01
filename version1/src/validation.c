#include "validation.h"
#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

void validate_axiom(char* axiom) {
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
                        break;
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

void rules_for(char *axiom, int *rules_indices) {
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

void validate_rules(Rule* rules, char* axiom, int* indices) {
    int size = 0;
    while (indices[size] != -1) {
        size++;
    }

    char input[SIZE + 1];

    for (int i = 0; i < size; i++) {
        printf("Enter rule for character '%c': ", axiom[indices[i]]);
        fgets(input, SIZE + 1, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strchr(input, ' ') != NULL) {
            printf("ERROR: Rule cannot contain spaces, try again.\n");
            i--;
            continue;
        }

        if (strlen(input) > 15) {
            printf("ERROR: Rule cannot be longer than 15 characters, try again.\n");
            i--;
            continue;
        }

        for (int j = 0; j < strlen(input); j++) {
            if (!isalpha(input[j]) && input[j] != '+' && input[j] != '-' && input[j] != '[' && input[j] != ']') {
                printf("ERROR: Rule can only contain letters and the symbols +, -, [, and ], try again.\n");
                i--;
                break;
            }
        }

        rules[i].character = axiom[indices[i]];
        strcpy(rules[i].rule, input);
    }
}

int validate_iterations() {
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

float validate_turn_and_start(int data) {
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
