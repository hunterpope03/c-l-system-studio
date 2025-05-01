#ifndef VISUALIZER_CONFIG_H
#define VISUALIZER_CONFIG_H

void initialize_python();
void finalize_python();
void visualize(const char* parsed, double turn_angle, double start_direction);

#endif