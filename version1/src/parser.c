#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

double calculate_growth_factor(Rule rules[]) {
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
    
    // Ensure minimum growth factor
    if (growth_factor < 1.5) {
        growth_factor = 1.5;
    }
    
    return growth_factor;
}

size_t calculate_buffer_size(size_t axiom_len, double growth_factor, int iterations) {
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
    
    return buffer_size;
}

int allocate_buffers(char** current_buffer, char** next_buffer, size_t buffer_size) {
    *current_buffer = malloc(buffer_size);
    *next_buffer = malloc(buffer_size);
    
    if (!(*current_buffer) || !(*next_buffer)) {
        free(*current_buffer);
        free(*next_buffer);
        return 0; // Failed
    }
    
    return 1; // Success
}

char* resize_buffer_if_needed(char* buffer, size_t needed_size, size_t* buffer_size) {
    if (needed_size >= *buffer_size) {
        size_t new_size;
        
        if (*buffer_size * 2 > needed_size + 1000000) {
            new_size = *buffer_size * 2;
        } else {
            new_size = needed_size + 1000000;
        }
        
        *buffer_size = new_size;
        buffer = realloc(buffer, new_size);
    }
    
    return buffer;
}

int apply_iteration(char* current_buffer, char** next_buffer, size_t* buffer_size, Rule rules[], double growth_factor) {
    size_t current_buffer_len = strlen(current_buffer);
    size_t next_buffer_len = 0;
    
    // Check if we need to resize before processing this iteration
    size_t needed_mem = current_buffer_len * growth_factor * 1.2;
    if (*buffer_size < needed_mem) {
        *buffer_size = needed_mem;
        *next_buffer = realloc(*next_buffer, *buffer_size);
        
        if (!(*next_buffer)) {
            return 0; // Failed
        }
    }
    
    (*next_buffer)[0] = '\0';
    
    // Process each character in the current buffer
    for (size_t i = 0; i < current_buffer_len; i++) {
        int matched = 0;
        
        // Try to match with a rule
        for (int j = 0; rules[j].character != '\0'; j++) {
            if (current_buffer[i] == rules[j].character) {
                size_t rule_length = strlen(rules[j].rule);
                
                // Ensure next_buffer is large enough
                if (next_buffer_len + rule_length >= *buffer_size - 1) {
                    *next_buffer = resize_buffer_if_needed(*next_buffer, 
                                                         next_buffer_len + rule_length + 1, 
                                                         buffer_size);
                    if (!(*next_buffer)) {
                        return 0; // Failed
                    }
                }
                
                // Copy rule to next_buffer
                memcpy(*next_buffer + next_buffer_len, rules[j].rule, rule_length);
                next_buffer_len += rule_length;
                matched = 1;
                break;
            }
        }
        
        // If no rule matched, copy the character as-is
        if (!matched) {
            if (next_buffer_len + 1 >= *buffer_size - 1) {
                *next_buffer = resize_buffer_if_needed(*next_buffer, 
                                                     next_buffer_len + 2, 
                                                     buffer_size);
                if (!(*next_buffer)) {
                    return 0; // Failed
                }
            }
            
            (*next_buffer)[next_buffer_len] = current_buffer[i];
            next_buffer_len++;
        }
    }
    
    // Null-terminate the string
    (*next_buffer)[next_buffer_len] = '\0';
    return 1; // Success
}

char* finalize_result(char* buffer) {
    size_t final_len = strlen(buffer) + 1;
    char* result = realloc(buffer, final_len);
    
    if (!result) {
        result = buffer;
    }
    
    return result;
}

char* parser(const char* axiom, Rule rules[], int iterations) {
    double growth_factor = calculate_growth_factor(rules);
    
    size_t axiom_len = strlen(axiom);
    size_t buffer_size = calculate_buffer_size(axiom_len, growth_factor, iterations);
    
    char* current_buffer;
    char* next_buffer;
    
    if (!allocate_buffers(&current_buffer, &next_buffer, buffer_size)) {
        return NULL;
    }
    
    strcpy(current_buffer, axiom);
    
    for (int iteration = 0; iteration < iterations; iteration++) {
        if (!apply_iteration(current_buffer, &next_buffer, &buffer_size, rules, growth_factor)) {
            free(current_buffer);
            return NULL;
        }
        
        size_t next_buffer_len = strlen(next_buffer);
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
    
    char* result = finalize_result(current_buffer);
    free(next_buffer);
    
    return result;
}