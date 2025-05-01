#include "parser.h"
#include <string.h>

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* parser(const char* axiom, Rule rules[], int iterations) {
    double growth_factor = 0;
    int rule_count = 0;
    
    for (int i = 0; rules[i].character != '\0'; i++) {
        growth_factor += strlen(rules[i].rule);
        rule_count++;
    }
    
    if (rule_count > 0) {
        growth_factor = growth_factor / rule_count;
    } else {
        growth_factor = 1.5;
    }
    
    if (growth_factor < 1.5) {
        growth_factor = 1.5;
    }
    
    size_t axiom_len = strlen(axiom);
    size_t parsed_string_size = axiom_len * pow(growth_factor, iterations) + 1;
    size_t buffer_size;
    
    if (parsed_string_size > 1000000) {
        buffer_size = 1000000;
    } else {
        buffer_size = parsed_string_size;
    }
    
    if (buffer_size < axiom_len * 10) {
        buffer_size = axiom_len * 10;
    }
    
    char* current_buffer = malloc(buffer_size);
    char* next_buffer = malloc(buffer_size);
    
    if (!current_buffer || !next_buffer) {
        free(current_buffer);
        free(next_buffer);
        return NULL;
    }
    
    strcpy(current_buffer, axiom);
    
    for (int iteration = 0; iteration < iterations; iteration++) {
        next_buffer[0] = '\0';
        size_t current_buffer_len = strlen(current_buffer);
        size_t next_buffer_len = 0;
        
        if (iteration > 0) {
            size_t needed_mem = current_buffer_len * growth_factor * 1.2;
            if (buffer_size < needed_mem) {
                buffer_size = needed_mem;
                next_buffer = realloc(next_buffer, buffer_size);
                if (!next_buffer) {
                    free(current_buffer);
                    return NULL;
                }
            }
        }
        
        for (size_t i = 0; i < current_buffer_len; i++) {
            int matched = 0;
            
            for (int j = 0; rules[j].character != '\0'; j++) {
                if (current_buffer[i] == rules[j].character) {
                    size_t rule_length = strlen(rules[j].rule);
                    
                    if (next_buffer_len + rule_length >= buffer_size - 1) {
                        size_t new_size;
                        if (buffer_size * 2 > next_buffer_len + rule_length + 1000000) {
                            new_size = buffer_size * 2;
                        } else {
                            new_size = next_buffer_len + rule_length + 1000000;
                        }
                        buffer_size = new_size;
                        next_buffer = realloc(next_buffer, buffer_size);
                        
                        if (!next_buffer) {
                            free(current_buffer);
                            return NULL;
                        }
                    }
                    
                    memcpy(next_buffer + next_buffer_len, rules[j].rule, rule_length);
                    next_buffer_len += rule_length;
                    matched = 1;
                    break;
                }
            }
            
            if (!matched) {
                if (next_buffer_len + 1 >= buffer_size - 1) {
                    buffer_size = buffer_size * 2;
                    next_buffer = realloc(next_buffer, buffer_size);
                    
                    if (!next_buffer) {
                        free(current_buffer);
                        return NULL;
                    }
                }
                
                next_buffer[next_buffer_len] = current_buffer[i];
                next_buffer_len++;
            }
        }
        
        next_buffer[next_buffer_len] = '\0';
        
        if (next_buffer_len + 1 > buffer_size) {
            buffer_size = next_buffer_len * 2;
            current_buffer = realloc(current_buffer, buffer_size);
            
            if (!current_buffer) {
                free(next_buffer);
                return NULL;
            }
        }
        
        char* temp = current_buffer;
        current_buffer = next_buffer;
        next_buffer = temp;
    }
    
    size_t final_len = strlen(current_buffer) + 1;
    char* result = realloc(current_buffer, final_len);
    
    if (!result) {
        result = current_buffer;
    }
    
    free(next_buffer);
    return result;
}
