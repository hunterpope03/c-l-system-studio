// include statement(s)
#include "app.h"
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// main function
int main(void) {
    _Bool exit_program = 0;
    int start_input;
    int example_input;
    L_System example_data;
    char* result;

    initialize_python();
    printf("***** L-System Parser v1.0.0 *****" "\n\n");
    printf("This program explores the mathematical theory of Lindenmayer(L)-Systems." "\n\n");
    
    while (exit_program == 0) {
        start_input = start_menu(); 
        flush_buffer();
        
        switch (start_input) {
            case 1:
                print_tutorial();
                print_key();
                break;
            case 2:   
                example_input = example_menu() - 1;
                flush_buffer();
                
                example_data = example_library[example_input];
                print_system(example_data);
                
                result = parser(example_data.axiom, example_data.rules, example_data.iterations);
                
                printf("Result: %ld" "\n\n", strlen(result));

                printf("Enter any key to visualize the system: (ensure to close the GUI window to proceed): ");
                getchar();

                visualize(result, example_data.turn_angle, example_data.start_direction);

                printf("\n\n");
                break;
            case 3:
                
                break;
            case 4:
                exit_program = 1;
                break;
            default:
                printf("Unknown error.");
                break;
        }
        
    }

    finalize_python();
    return 0;
}

// function definitions
void flush_buffer() {
    while (getchar() != '\n') {
        continue;
    }
}

int start_menu() {
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

void print_tutorial() {
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

void print_key() {
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
        printf("Axiom : cannot be empty / cannot be longer than 10 characters / cannot contain spaces / cannot contain any characters other than letters and '+' '-' '[' ']'" "\n\t");
        printf("Transformation Rules : cannot contain spaces / cannot be longer than 8 characters / cannot contain any characters other than letters and '+' '-' '[' ']'" "\n\t");
        printf("Number of Iterations: must be a positive integer ≤ 8" "\n\t");
        printf("Turn Angle & Starting Direction : must be a number ≥ 0 and ≤ 360" "\n\n");
}

int example_menu() {
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

char* parser(const char* axiom, Rule rules[], int iterations) {
    double avg_growth_factor = 0;
    int rule_count = 0;
    for (int j = 0; rules[j].character != '\0'; j++) {
        avg_growth_factor += strlen(rules[j].rule);
        rule_count++;
    }
    
    avg_growth_factor = (rule_count > 0) ? avg_growth_factor / rule_count : 1.5;
    if (avg_growth_factor < 1.5) avg_growth_factor = 1.5;
    
    size_t estimated_final_size = strlen(axiom) * pow(avg_growth_factor, iterations) + 1;
    
    size_t buffer_size = (estimated_final_size > 1000000) ? 1000000 : estimated_final_size;
    if (buffer_size < strlen(axiom) * 10) buffer_size = strlen(axiom) * 10;
    
    char* current = malloc(buffer_size);
    char* next = malloc(buffer_size);
    
    if (!current || !next) {
        fprintf(stderr, "Initial memory allocation failed\n");
        exit(1);
    }
    
    strcpy(current, axiom);
    
    for (int iter = 0; iter < iterations; iter++) {
        next[0] = '\0';
        size_t current_len = strlen(current);
        size_t next_len = 0;
        
        size_t next_capacity = buffer_size;
        if (iter > 0 && next_capacity < current_len * avg_growth_factor * 1.2) {
            next_capacity = current_len * avg_growth_factor * 1.2;
            next = realloc(next, next_capacity);
            if (!next) {
                fprintf(stderr, "Memory reallocation failed for next buffer\n");
                free(current);
                exit(1);
            }
            buffer_size = next_capacity;
        }
        
        for (size_t i = 0; i < current_len; i++) {
            int matched = 0;
            for (int j = 0; rules[j].character != '\0'; j++) {
                if (current[i] == rules[j].character) {
                    size_t rule_len = strlen(rules[j].rule);
                    
                    if (next_len + rule_len >= buffer_size - 1) {
                        buffer_size = (buffer_size * 2 > next_len + rule_len + 1000000) ? 
                                      buffer_size * 2 : next_len + rule_len + 1000000;
                        next = realloc(next, buffer_size);
                        if (!next) {
                            fprintf(stderr, "Memory reallocation failed during rule application\n");
                            free(current);
                            exit(1);
                        }
                    }
                    
                    memcpy(next + next_len, rules[j].rule, rule_len);
                    next_len += rule_len;
                    
                    matched = 1;
                    break;
                }
            }
            
            if (!matched) {
                if (next_len + 1 >= buffer_size - 1) {
                    buffer_size = buffer_size * 2;
                    next = realloc(next, buffer_size);
                    if (!next) {
                        fprintf(stderr, "Memory reallocation failed during character copy\n");
                        free(current);
                        exit(1);
                    }
                }
                next[next_len++] = current[i];
            }
        }
        
        // Properly null-terminate
        next[next_len] = '\0';
        
        // Resize current buffer if needed for next iteration
        if (next_len + 1 > buffer_size) {
            buffer_size = next_len * 2;  // Double current length for future growth
            current = realloc(current, buffer_size);
            if (!current) {
                fprintf(stderr, "Memory reallocation failed for current buffer\n");
                free(next);
                exit(1);
            }
        }
        
        // Swap buffers
        char* temp = current;
        current = next;
        next = temp;
    }
    
    // Resize current to actual size to save memory
    size_t final_len = strlen(current) + 1;
    char* result = realloc(current, final_len);
    if (!result) {
        // If realloc fails, we can still return the original pointer
        result = current;
    }
    
    free(next);
    return result;
}

void initialize_python() {
    Py_Initialize();
    PyObject *sys_module = PyImport_ImportModule("sys");
    PyObject *sys_path = PyObject_GetAttrString(sys_module, "path");
    PyList_Append(sys_path, PyUnicode_FromString("."));
    Py_DECREF(sys_module);
    Py_DECREF(sys_path);
    
    PyRun_SimpleString(
        "import sys\n"
        "import PyQt5.QtWidgets\n"
        "import threading\n"
        "import time\n"
        "\n"
        "class QuitableTimer(threading.Thread):\n"
        "    def __init__(self, app):\n"
        "        threading.Thread.__init__(self)\n"
        "        self.app = app\n"
        "        self.daemon = True\n"
        "\n"
        "    def run(self):\n"
        "        time.sleep(0.5)\n"
        "        self.app.quit()\n"
    );
}

void finalize_python() {
    Py_Finalize();
}

void visualize(const char* parsed, double turn_angle, double start_direction) {
    PyObject *module = PyImport_ImportModule("visualizer");
    if (!module) {
        PyErr_Print();
        return;
    }
    
    PyObject *cls = PyObject_GetAttrString(module, "LSystemVisualizer");
    if (!cls) {
        PyErr_Print();
        Py_DECREF(module);
        return;
    }
    
    PyObject *args = PyTuple_Pack(3,
        PyUnicode_FromString(parsed),
        PyFloat_FromDouble(turn_angle),
        PyFloat_FromDouble(start_direction)
    );
    
    PyObject *instance = PyObject_CallObject(cls, args);
    if (!instance) {
        PyErr_Print();
        Py_DECREF(module);
        Py_DECREF(cls);
        Py_DECREF(args);
        return;
    }

    PyObject *visualize_method = PyObject_GetAttrString(instance, "visualize");
    if (visualize_method) {
        PyObject_CallObject(visualize_method, NULL);
        Py_DECREF(visualize_method);
        
        PyRun_SimpleString(
            "app = PyQt5.QtWidgets.QApplication.instance()\n"
            "if app:\n"
            "    timer = QuitableTimer(app)\n"
            "    timer.start()\n"
            "    app.exec_()\n"
        );
    } else {
        PyErr_Print();
    }
    
    Py_DECREF(module);
    Py_DECREF(cls);
    Py_DECREF(args);
    Py_DECREF(instance);
}

// gcc -o app app.c -I/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13 -L/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/lib -lpython3.13