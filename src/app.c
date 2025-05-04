#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

/**
 * Use after scanf to clear the input buffer.
 *
 * The input buffer is where the console stores all the characters the user
 * inputs. When the user inputs more characters than the size of the buffer,
 * the extra characters are still stored in the buffer. This can cause problems
 * if the user enters something unexpected, as it can cause the program to act
 * unexpectedly.
 *
 * This function will continue to read characters from the console until it
 * encounters a newline character, clearing the extra characters from the
 * buffer.
 */
void flush_buffer() { // use after scanf to clear the input buffer
    while (getchar() != '\n') {
        continue;
    }
}

/**
 * Prints the start menu and returns an integer based on the user's input.
 *
 * 1 : Tutorial
 * 2 : Example L-Systems
 * 3 : Custom L-System
 * 4 : Exit program
 *
 * The user is reprompted until a valid selection is made.
 *
 * @return 1, 2, 3, or 4
 */
int start_menu() { // get start menu input
    char input;
    
    printf("a) Tutorial" "\n");
    printf("b) Example L-Systems" "\n");
    printf("c) Custom L-System" "\n");
    printf("or enter X to exit" "\n\n");
    
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
        } else if (input =='X' || input == 'x') {
            return 4;
        } else {
            printf("\nERROR: Invalid selection. Try again: ");
            flush_buffer();
        }
    }
}

/**
 * Prints the tutorial text.
 *
 * This function prints out a detailed explanation of what an L-System is,
 * how it works, and what it requires to generate a visual representation.
 */
void print_tutorial() { // print tutorial text
    printf("\n\n" "*** Tutorial ***" "\n\n");
    
    printf("- A Lindenmayer(L)-System is a mathematical theory used to create plots of complex geometric botanic figures." "\n\n");
    printf("- An L-System starts with an axiom, transformation rules, and a number of iterations:" "\n\n\t");
        printf("Axiom - a simple starting string, like 'F'" "\n\t");
        printf("Transformation Rules - a set of rules that define how to replace a character in a string with another character or string, like 'F' -> 'F+F'" "\n\t");
        printf("Number of iterations - the number of times the transformation rules are applied, like 8" "\n\n");
    printf("- The system applies the transformation rules to the axiom, resulting in a more complex string. It then applies the rules to this resulting string, repeating this process for the number of iterations." "\n\n");
    printf("- After the iterations are applied, the resulting string is called a parsed L-System." "\n\n");
    printf("- The result is passed into a visualizer which uses each character in the parsed string as a command. It requires a program key (shown below) and the following data:" "\n\n\t");
        printf("Turn Angle - the angle at which to turn, like 90" "\n\t");
        printf("Starting Direction - the angle at which to start the drawing, like 0, used for rotation of the plot" "\n");
}

/**
 * Prints the program key and input restrictions.
 *
 * This function prints out a detailed explanation of how the program works,
 * including the meaning of each character in the L-System, and the input
 * restrictions for each data point.
 */
void print_key() { // print program key and input restrictions
    printf("\n\n" "*** Program Key & Input Restrictions ***" "\n\n");
    
    printf("- A variable is any character that is replaced in a transformation rule and are represented by letters in this program:" "\n\n\t");
        printf("Uppercase Letter : move forward while drawing" "\n\t");
        printf("Lowercase Letter : move forward" "\n\n");
    printf("- A constant is any character that is not replaced in a transformation rule and are represented by symbols in this program:" "\n\n\t");
        printf("+ : turn right at the turn angle" "\n\t");
        printf("- : turn left at the turn angle" "\n\t");
        printf("[ : save state to stack" "\n\t");
        printf("] : remove state from stack" "\n\n");
        
    printf("- Each data point entered into this program has specific requirements:" "\n\n\t");
        printf("Axiom : cannot be empty / cannot be longer than 15 characters / cannot contain spaces / cannot contain any characters other than letters and '+' '-' '[' ']'" "\n\t");
        printf("Transformation Rules : cannot contain spaces / cannot be longer than 15 characters / cannot contain any characters other than letters and '+' '-' '[' ']'" "\n\t");
        printf("Number of Iterations: must be a positive integer ≤ 8" "\n\t");
        printf("Turn Angle & Starting Direction : must be a number ≥ 0 and ≤ 360" "\n\n");
}


/**
 * Prints the example menu and gets a valid selection from the user.
 *
 * Prints out a list of example L-Systems, then asks the user to select one
 * by entering a number. The function will continue to ask for input until a
 * valid number is entered.
 *
 * @return the number of the selected example
 */
int example_menu() { // get example menu input
    int input;
    printf("\n\n" "*** Example Menu ***" "\n\n");
    
    printf("Here are a few example L-Systems:" "\n\n\t");
        printf("1) Fractal Tree" "\n\t");
        printf("2) Fractal Plant" "\n\t");
        printf("3) Bush 1" "\n\t");
        printf("4) Bush 2" "\n\t");
        printf("5) Bush 4" "\n\t");
        printf("6) Board" "\n\t");
        printf("7) Sierpinski Arrowhead" "\n\t");
        printf("8) Pentaplexity" "\n\t");
        printf("9) Dragon Curve" "\n\t");
        printf("10) Hexagonal Gosper" "\n\n");
        
    printf("Select a menu option from above: ");
    while (1) {
        if (scanf("%d", &input) != 1) {
            printf("\nERROR: Invalid input. Try again: ");
            flush_buffer();
            continue;
        }

        if (input >= 1 && input <= 10) {
            return input;
        } else {
            printf("\nERROR: Invalid input. Try again: ");
        }
    }
}

/**
 * Prints the custom menu text.
 *
 * This function prints out the custom menu text, which includes the
 * input restrictions for a custom L-System. It also calls the print_key
 * function to print the program's key and input restrictions.
 */
void print_custom_menu() { // print the custom menu text
    printf("\n\n" "*** Custom Menu ***" "\n\n\n");

    printf("In entering a custom L-System, ensure all inputs are valid and follow the input restrictions below:");

    print_key();
}