// variables.h
#ifndef COMMONVARIABLES_H
#define COMMONVARIABLES_H

const int num_vars = 3;

const int COMPRESSION_FACTOR = 1000;

// Our different telemetry states
int DEFAULT = 1;
int DERIVATIVE = 2;
int OFFSET = 3;
int DERIVATIVE_OFFSET = 4;

int global_current_mode = DERIVATIVE;

#endif