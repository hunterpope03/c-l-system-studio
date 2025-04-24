// include statement(s)
#include "app.h"
#include <stdio.h>
#include <string.h>

// main function
int main(void) {
    int start_input;
    
    printf("*** L-System Parser v1.0.0 ***" "\n\n");
    printf("This program explores the mathematical theory of Lindenmayer(L)-Systems." "\n\n");
    
    start_input = start_menu(); 
}

// function definitions
void flush_buffer() {
    while (getchar() != '\n') {
        continue;
    }
}

int start_menu() {
    char input;
    
    printf("a) Tutorial" "\n");
    printf("b) Example L-Systems" "\n");
    printf("c) Custom L-System" "\n\n");
    
    printf("Select a menu option from above: ");
    while (1) {
        input = getchar();

        if (input == '\n') continue;

        if (input == 'A' || input == 'a') {
            return 1;
        } else if (input == 'B' || input == 'b') {
            return 2;
        } else if (input == 'C' || input == 'c') {
            return 3;
        } else {
            printf("\nERROR: Invalid selection. Try again: ");
            flush_buffer();
        }
    }
}

void validate_axiom(char *axiom) {
    char input[100];
    int length;

    printf("Enter your axiom (see above key for requirements): ");

    while (1) {
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;
            length = strlen(input);

            if (length == 0) {
                printf("\nERROR: Axiom must be more than 0 characters long, try again: ");
            } else if (length > 10) {
                printf("\nERROR: Axiom must be less than or equal to 10 characters long, try again: ");
            } else {
                int valid = 1;
                for (int i = 0; i < length; i++) {
                    if (input[i] != 'F' && input[i] != 'f' && input[i] != '+' && input[i] != '-' && input[i] != '[' && input[i] != ']') {
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
    
    for (int i = 0; i < index_range; i++) {
        if (axiom[i] == 'F' || axiom[i] == 'f') {
            rules_indices[rules_index++] = i;
        }
    }
    
    rules_indices[rules_index] = -1;
}

/* 
    typedef struct {
        char custom_axiom[11];
        int rules_for_indices[11];
        char custom_rules[9][9];
        int custom_iterations;
        float custom_turn_angle;
        float custom_start_direction;
    } custom_system;
    
    custom_system user;
    
    validate_axiom(user.custom_axiom);
    rules_for(user.custom_axiom, user.rules_for_indices);
    validate_rules(user.rules);
    user.custom_iterations = validate_iterations();
    user.custom_turn_angle = validate_turn_angle();
    user.custom_start_direction = validate_start_direction;
*/