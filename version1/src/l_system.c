#include "l_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

void print_system(L_System sys) {
    printf("\n" "This system has these details:" "\n\n\t");
    printf("Axiom: %s" "\n\t", sys.axiom);
    printf("Rule(s): {" "\n\t\t");
    for (int i = 0; i < SIZE; i++) {
        if (sys.rules[i].character == '\0' && sys.rules[i].rule[0] == '\0') {
            break;
        }
        printf("%c -> %s" "\n\t\t", sys.rules[i].character, sys.rules[i].rule);
    }
    printf("}" "\n\t");
    printf("Iterations: %d" "\n\t", sys.iterations);
    printf("Turn Angle: %.2f" "\n\t", sys.turn_angle);
    printf("Starting Direction: %.2f" "\n\n", sys.start_direction);
}