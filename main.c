#include "app.h"
#include "l_system.h"
#include "parser.h"
#include "visualizer_config.h"
#include "example_library.h"
#include "validation.h" // include all header files

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // include all C libraries

/**
 * @brief The main entry point of the program.
 *
 * The main function initializes the python environment using `initialize_python()`,
 * prints a welcome message, and then enters a loop to repeatedly show the main
 * menu and execute the user's selection. The loop continues until the user
 * chooses the exit option.
 *
 * @return 0 if the program executes successfull./appy.
 */
int main (void) {
    _Bool exit_program = 0;
    int start_input;

    int example_input;
    L_System ExampleData;
    L_System CustomData;
    char* parsed_system;

    initialize_python(); // setup python environment

    printf("***** L-System Parser v1.0.0 *****" "\n\n");
    printf("This program explores the mathematical theory of Lindenmayer(L)-Systems." "\n\n");
    
    while (exit_program == 0) {
        start_input = start_menu(); // get menu option
        flush_buffer();

        switch (start_input) {
            case 1: // tutorial menu option
                print_tutorial();
                print_key();
                break;
            case 2: // example menu option
                example_input = example_menu() - 1; // align menu input with example library index
                flush_buffer();

                ExampleData = example_library[example_input]; // create example data struct with proper data
                print_system(ExampleData); // print example data details
                
                parsed_system = parser(ExampleData.axiom, ExampleData.rules, ExampleData.iterations); // parse example data
                
                printf("Result: %ld" "\n\n", strlen(parsed_system)); // print parsed system length

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): "); 
                getchar();

                visualize(parsed_system, ExampleData.turn_angle, ExampleData.start_direction); // visualize example data

                printf("\n\n");
                break;
            case 3: // custom menu option
                print_custom_menu();

                validate_axiom(CustomData.axiom);
                rules_for(CustomData.axiom, CustomData.rules_for_indices);
                validate_rules(CustomData.rules, CustomData.axiom, CustomData.rules_for_indices); // intialize custom data struct with proper data
                CustomData.iterations = validate_iterations(); 
                flush_buffer(); 
                CustomData.turn_angle = validate_turn_and_start(1);
                flush_buffer();
                CustomData.start_direction = validate_turn_and_start(2);
                flush_buffer(); // flush line buffer after any scanf() use 

                print_system(CustomData); // print custom data details

                parsed_system = parser(CustomData.axiom, CustomData.rules, CustomData.iterations); // parse custom data
                
                printf("Result: %ld" "\n\n", strlen(parsed_system)); // print parsed system length

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): ");
                getchar();

                visualize(parsed_system, CustomData.turn_angle, CustomData.start_direction); // visualize custom data

                printf("\n\n");
                break;
            case 4: // exit menu option
                exit_program = 1;
                break;
            default: // unknown error 
                printf("Unknown error.");
                break;
        }
        
    }

    finalize_python(); // teardown python environment
    return 0;
}

