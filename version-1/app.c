// include statement(s)
#include "app.h"
#include <stdio.h>

// main function
int main(void) {
    char start_input;
    
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
            return input;
        } else if (input == 'B' || input == 'b') {
            return input;
        } else if (input == 'C' || input == 'c') {
            return input;
        } else {
            printf("\nERROR: Invalid selection. Try again: ");
            flush_buffer();
        }
    }
}