#ifndef VALIDATION_H
#define VALIDATION_H

#include "l_system.h" // include Rule struct definition

void validate_axiom(char* axiom);
void rules_for(char *axiom, int *rules_indices);
void validate_rules(Rule* rules, char* axiom, int* indices);
int validate_iterations();
float validate_turn_and_start(int data); // function prototypes

#endif