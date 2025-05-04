#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

/**
 * @brief Calculates the expected growth factor from a given rule set.
 * 
 * @param rules The array of rules to calculate the growth factor from.
 * 
 * @return The calculated growth factor, which is the average length of the rules
 * in the rule set. If no rules are present, the minimum growth factor is 1.5.
 */
double calculate_growth_factor(Rule rules[]) { // calculate the expected growth from the rule set
    double growth_factor = 0;
    int rule_count = 0;
    
    for (int i = 0; rules[i].character != '\0'; i++) {
        growth_factor += strlen(rules[i].rule); // add the length of each rule
        rule_count++;
    }
    
    if (rule_count > 0) { // calculate how many characters each rule produces
        growth_factor = growth_factor / rule_count;
    } else {
        growth_factor = 1.5;
    }
    
    if (growth_factor < 1.5) { // minimum growth factor if no rules are present
        growth_factor = 1.5;
    }
    
    return growth_factor;
}

/**
 * @brief Calculates the buffer size based on the expected growth of the parsed string.
 * 
 * This function takes the length of the axiom, the growth factor, and the number of iterations
 * as parameters and returns the size of the buffer, in bytes. The buffer size is calculated
 * as the expected size of the parsed string plus one for the null terminator. If the parsed
 * string is expected to be larger than 1,000,000 bytes, the buffer size is capped at
 * 1,000,000 bytes. If the buffer size is less than 10 times the length of the axiom, the
 * buffer size is set to 10 times the length of the axiom.
 * 
 * @param axiom_len The length of the axiom string.
 * @param growth_factor The expected growth factor of the parsed string.
 * @param iterations The number of iterations to apply the growth factor.
 * 
 * @return The calculated buffer size, in bytes.
 */
size_t calculate_buffer_size(size_t axiom_len, double growth_factor, int iterations) { // calculate the buffer size based on the expected growth
    size_t parsed_string_size = axiom_len * pow(growth_factor, iterations) + 1; // calculate the expected size of the parsed string
    size_t buffer_size;
    
    if (parsed_string_size > 1000000) { // set buffer size
        buffer_size = 1000000;
    } else {
        buffer_size = parsed_string_size;
    }
    
    if (buffer_size < axiom_len * 10) {
        buffer_size = axiom_len * 10;
    }
    
    return buffer_size;
}

/**
 * @brief Allocates memory for both buffers.
 * 
 * This function allocates memory for both the current and next buffers, and assigns
 * the pointers to the allocated memory to the provided pointers. If allocation fails,
 * the function frees any allocated memory and returns 0. If allocation succeeds, the
 * function returns 1.
 * 
 * @param current_buffer The pointer to store the allocated memory for the current buffer.
 * @param next_buffer The pointer to store the allocated memory for the next buffer.
 * @param buffer_size The size of the memory to allocate, in bytes.
 * 
 * @return 1 if allocation succeeds, 0 if allocation fails.
 */
int buffer_allocate(char** current_buffer, char** next_buffer, size_t buffer_size) { // allocate memory for both buffers
    *current_buffer = malloc(buffer_size);
    *next_buffer = malloc(buffer_size);
    
    if (!(*current_buffer) || !(*next_buffer)) {
        free(*current_buffer);
        free(*next_buffer);
        return 0;
    }
    
    return 1; // returning 1 for success, 0 for failure
}

/**
 * @brief Resizes the buffer if the needed size exceeds the current buffer size.
 * 
 * This function checks whether the buffer needs to be resized based on the
 * specified needed size. If resizing is necessary, it increases the buffer size
 * by either doubling the current size or adding 1,000,000 bytes, whichever is larger,
 * and reallocates memory accordingly.
 * 
 * @param buffer The buffer to be resized.
 * @param needed_size The size that the buffer needs to accommodate.
 * @param buffer_size A pointer to the current size of the buffer. This value
 * will be updated to reflect the new buffer size if resizing occurs.
 * 
 * @return The pointer to the resized buffer, or the original buffer if resizing
 * was not necessary.
 */

char* buffer_resize(char* buffer, size_t needed_size, size_t* buffer_size) { // resize a buffer if needed
    if (needed_size >= *buffer_size) {
        size_t new_size;
        
        if (*buffer_size * 2 > needed_size + 1000000) { // double buffer size 
            new_size = *buffer_size * 2;
        } else {
            new_size = needed_size + 1000000; // or add 1mb
        }
        
        *buffer_size = new_size; 
        buffer = realloc(buffer, new_size); // reallocate memory
    }
    
    return buffer;
}

/**
 * @brief Apply one iteration of the L-System to the current buffer.
 * 
 * This function takes the current buffer, a pointer to the next buffer, the size of the next buffer,
 * an array of Rule structs, and the growth factor as parameters. It applies one iteration of the
 * L-System to the current buffer, writing the result to the next buffer. If the next buffer is not
 * large enough to accommodate the result, it is resized. The function returns 1 on success and 0 on failure.
 * 
 * @param current_buffer The current state of the L-System.
 * @param next_buffer A pointer to the buffer that will store the result of applying one iteration of the L-System.
 * @param buffer_size A pointer to the current size of the next buffer. This value will be updated to reflect the new buffer size if resizing occurs.
 * @param rules An array of Rule structs that define the L-System.
 * @param growth_factor The expected growth factor of the L-System.
 * 
 * @return 1 on success, 0 on failure.
 */
int iterate(char* current_buffer, char** next_buffer, size_t* buffer_size, Rule rules[], double growth_factor) {
    size_t current_buffer_len = strlen(current_buffer);
    size_t next_buffer_length = 0;
    
    size_t needed_mem = current_buffer_len * growth_factor * 1.2; // resize the buffer if needed
    if (*buffer_size < needed_mem) {
        *buffer_size = needed_mem;
        *next_buffer = realloc(*next_buffer, *buffer_size);
        
        if (!(*next_buffer)) {
            return 0; 
        }
    }
    
    (*next_buffer)[0] = '\0';
    
    for (size_t i = 0; i < current_buffer_len; i++) { // loop through each character in current buffer
        int matched = 0;

        for (int j = 0; rules[j].character != '\0'; j++) { // check if each character matches any rule
            if (current_buffer[i] == rules[j].character) {
                size_t rule_length = strlen(rules[j].rule);
                
                if (next_buffer_length + rule_length >= *buffer_size - 1) { // ensure buffer is big enough
                    *next_buffer = buffer_resize(*next_buffer, next_buffer_length + rule_length + 1, buffer_size);
                    if (!(*next_buffer)) {
                        return 0; 
                    }
                }
                
                memcpy(*next_buffer + next_buffer_length, rules[j].rule, rule_length); // copy the rule to the next buffer
                next_buffer_length += rule_length;
                matched = 1;
                break;
            }
        }
        
        if (!matched) { // if no rule for a character, copy just the character to the next buffer
            if (next_buffer_length + 1 >= *buffer_size - 1) { // ensure buffer is big enough
                *next_buffer = buffer_resize(*next_buffer, next_buffer_length + 2, buffer_size);
                if (!(*next_buffer)) {
                    return 0; 
                }
            }
            
            (*next_buffer)[next_buffer_length] = current_buffer[i];
            next_buffer_length++;
        }
    }
    
    (*next_buffer)[next_buffer_length] = '\0'; // null-terminate the resulting string
    return 1;
}

/**
 * @brief Primary parser function.
 * 
 * This function takes an axiom, a set of rules, and the number of iterations as parameters.
 * It then applies the rules to the axiom the specified number of times, and returns the
 * resulting string. The function uses a "ping pong" approach, using two buffers to store the
 * current and next strings. After each iteration, the function checks if the current buffer
 * is big enough to hold the next string. If it is not, the function doubles the size of the
 * buffer and reallocates the memory. The function also swaps the buffers after each iteration.
 * 
 * @param axiom The axiom string.
 * @param rules The set of rules to apply.
 * @param iterations The number of iterations to apply the rules.
 * 
 * @return The parsed string.
 */
char* parser(const char* axiom, Rule rules[], int iterations) { // primary parser function
    double growth_factor = calculate_growth_factor(rules); // get the expected growth factor from the rule set
    
    size_t axiom_len = strlen(axiom);
    size_t buffer_size = calculate_buffer_size(axiom_len, growth_factor, iterations); // get the starting buffer size
    
    char* current_buffer;
    char* next_buffer; // "ping pong" approach
    
    if (!buffer_allocate(&current_buffer, &next_buffer, buffer_size)) { // allocate both buffers
        return NULL;
    }
    
    strcpy(current_buffer, axiom); // copy axiom to current buffer 
    
    for (int iteration = 0; iteration < iterations; iteration++) { // loop through the number of iterations
        if (!iterate(current_buffer, &next_buffer, &buffer_size, rules, growth_factor)) {  // apply one iteration
            free(current_buffer);
            return NULL;
        }
        
        size_t next_buffer_length = strlen(next_buffer); // check if current buffer is big enough
        if (next_buffer_length + 1 > buffer_size) {
            buffer_size = next_buffer_length * 2;
            current_buffer = realloc(current_buffer, buffer_size);
            
            if (!current_buffer) {
                free(next_buffer);
                return NULL;
            }
        }
        
        char* temp = current_buffer;
        current_buffer = next_buffer;
        next_buffer = temp; // swap buffers
    }
    
    char* result = finalize_parser(current_buffer);
    free(next_buffer); // free the next buffer
    
    return result; // return the parsed string
}

/**
 * @brief Resizes the buffer to the final size of the parsed string.
 * 
 * This function takes a pointer to the parsed string as a parameter, and resizes
 * the buffer to the final size of the string. If the realloc fails, the function
 * returns the original buffer.
 * 
 * @param buffer A pointer to the parsed string.
 * 
 * @return The resized buffer, or the original buffer if the realloc fails.
 */
char* finalize_parser(char* buffer) {
    size_t final_len = strlen(buffer) + 1; // calculate size of final string
    char* result = realloc(buffer, final_len); // resize buffer to final size
    
    if (!result) {
        result = buffer;
    }
    
    return result;
}