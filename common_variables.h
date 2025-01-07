// variables.h
#ifndef COMMONVARIABLES_H
#define COMMONVARIABLES_H

// extern const int num_vars;

// extern int DEFAULT; 
// extern int DERIVATIVE; 
// extern int OFFSET; 
// extern int DERIVATIVE_OFFSET; 

const int num_vars = 3;

// Our different telemetry states
int DEFAULT = 1;
int DERIVATIVE = 2;
int OFFSET = 3;
int DERIVATIVE_OFFSET = 4;

int global_current_mode = DEFAULT;

#endif