#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#define SIZE 31

typedef struct {
    char character;
    char rule[SIZE];
} Rule; // rule struct for each character in an L-System

struct L_System {
    char axiom[SIZE];
    int rules_for_indices[SIZE];
    Rule rules[SIZE];
    int iterations;
    float turn_angle;
    float start_direction;
}; // L-System struct

typedef struct L_System L_System;

void print_system(L_System sys); // function prototype

#endif