#include "geodesic.h"

#ifndef __HAVE_MATHIO_H__

#define __HAVE_MATHIO_H__

int scan_coordinates(FILE *in, struct Coordinates *loc);
int scan_vector(FILE *in, struct Vector *vector);

void start_print(FILE *out, int i);
void error();

long double sqr(long double operand);
long double atan2_modified(long double y, long double x);

#endif
