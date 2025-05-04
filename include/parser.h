#ifndef PARSER_H
#define PARSER_H

#include "l_system.h" // include the L-System struct so the parser function can accept one
#include <stddef.h>

double calculate_growth_factor(Rule rules[]);
size_t calculate_buffer_size(size_t axiom_len, double growth_factor, int iterations);
int buffer_allocate(char** current_buffer, char** next_buffer, size_t buffer_size);
char* buffer_resize(char* buffer, size_t needed_size, size_t* buffer_size);
int iterate(char* current_buffer, char** next_buffer, size_t* buffer_size, Rule rules[], double growth_factor);
char* parser(const char* axiom, Rule rules[], int iterations); 
char* finalize_parser(char* buffer);

#endif