#include <stdio.h>
#include <stdlib.h>
#include "geodesic.h"
#include "io.h"

int scan_coordinates(FILE *in, struct Coordinates *loc)
{
	int count = fscanf(in, "%Lf,%Lf", &loc->lat, &loc->lon);

	if (count == 2 && (loc->lat < -90 || loc->lat > 90 || loc->lon < -180 || loc->lon > 180)) {
		fprintf(stderr, "Coordinate out of range: %Lf, %Lf.\n", loc->lat, loc->lon);
		error();
	}

	loc->lat = loc->lat * RAD;
	loc->lon = loc->lon * RAD;

	return count;
}

int scan_vector(FILE *in, struct Vector *vector)
{
	int count = fscanf(in, "%Lf:%Lf", &vector->s, &vector->theta);

	if (count == 2 && (vector->theta < 0 || vector->theta > 360)) {
		fprintf(stderr, "Bearing out of range: %Lf.", vector->theta);
		error();
	}

	vector->theta = vector->theta * RAD;

	return count;
}

void start_print(FILE *out, int i)
{
	if (i == 1)
		fputs("[\n", out);
	else
		fputs(",\n", out);

	fputs("  {\n", out);
	return;
}

void error()
{
	fprintf(stderr, "\nAbort.\n");
	exit(1);
}
