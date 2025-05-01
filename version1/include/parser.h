#ifndef PARSER_H
#define PARSER_H

#include "l_system.h" // include the L-System struct so the parser function can accept one

char* parser(const char* axiom, Rule rules[], int iterations); // function prototype

#endif