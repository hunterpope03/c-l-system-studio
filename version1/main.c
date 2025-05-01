#include "app.h"
#include "l_system.h"
#include "parser.h"
#include "visualizer_config.h"
#include "example_library.h"
#include "validation.h"

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main (void) {
    _Bool exit_program = 0;
    int start_input;

    int example_input;
    L_System ExampleData;
    L_System CustomData;
    char* parsed_system;

    initialize_python();
    printf("***** L-System Parser v1.0.0 *****" "\n\n");
    printf("This program explores the mathematical theory of Lindenmayer(L)-Systems." "\n\n");
    
    while (exit_program == 0) {
        start_input = start_menu(); 
        flush_buffer();

        switch (start_input) {
            case 1:
                print_tutorial();
                print_key();
                break;
            case 2:   
                example_input = example_menu() - 1;
                flush_buffer();

                ExampleData = example_library[example_input];
                print_system(ExampleData);
                
                parsed_system = parser(ExampleData.axiom, ExampleData.rules, ExampleData.iterations);
                
                printf("Result: %ld" "\n\n", strlen(parsed_system));

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): ");
                getchar();

                visualize(parsed_system, ExampleData.turn_angle, ExampleData.start_direction);

                printf("\n\n");
                break;
            case 3:
                print_custom_menu();

                validate_axiom(CustomData.axiom);
                rules_for(CustomData.axiom, CustomData.rules_for_indices);
                validate_rules(CustomData.rules, CustomData.axiom, CustomData.rules_for_indices);
                CustomData.iterations = validate_iterations(); 
                flush_buffer();
                CustomData.turn_angle = validate_turn_and_start(1);
                flush_buffer();
                CustomData.start_direction = validate_turn_and_start(2);
                flush_buffer();

                print_system(CustomData);

                parsed_system = parser(CustomData.axiom, CustomData.rules, CustomData.iterations);
                
                printf("Result: %ld" "\n\n", strlen(parsed_system));

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): ");
                getchar();

                visualize(parsed_system, CustomData.turn_angle, CustomData.start_direction);

                printf("\n\n");
                break;
            case 4:
                exit_program = 1;
                break;
            default:
                printf("Unknown error.");
                break;
        }
        
    }

    finalize_python();
    return 0;
}

