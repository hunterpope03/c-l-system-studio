#include <stdio.h>

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
    L_System example_data;
    char* result;

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
                
                example_data = example_library[example_input];
                print_system(example_data);
                
                result = parser(example_data.axiom, example_data.rules, example_data.iterations);
                
                printf("Result: %ld" "\n\n", strlen(result));

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): ");
                getchar();

                visualize(result, example_data.turn_angle, example_data.start_direction);

                printf("\n\n");
                break;
            case 3:
                
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

