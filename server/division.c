// division.c
#include <math.h>

double divide(double a, double b) {
    if (b != 0)
        return a / b;
    else
        return INFINITY; // Indicates division by zero
}
