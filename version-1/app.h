#ifndef APP_H
#define APP_H

// function prototypes
void flush_buffer();
int start_menu();
void validate_axiom(char *axiom);
void rules_for(char *axiom, int indices[]);
void validate_rules(char *rules);
int validate_iterations();
float validate_turn_angle();
float validate_start_direction();

#endif