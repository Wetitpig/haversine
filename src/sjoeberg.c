#include <stdlib.h>
#include <math.h>
#include "vincenty.h"
#include "sjoeberg.h"

#define E2C2 ECC * sqr(c)

int binom(int n, int r)
{
	int nr;
	int fac[2];
	fac[0] = 1;
	fac[1] = 1;

	nr = n - r;
	while (nr)
		fac[1] *= nr--;

	while (n > r)
		fac[0] *= n--;

	return fac[0] / fac[1];
}

long double E(struct Coordinates *vertex, int k, long double c)
{
	int j;
	long double hsum = 0, z, p, f, S, h0;
	long double feval[k + 1];

	z = sqr(c / cosl(vertex->lat)) * (1 - ECC);
	p = 1 - 2 * E2C2;
	f = (1 - E2C2) * E2C2;
	S = f + p * z - sqr(z);
	if (S < 0)
		S = 0;
	h0 = sqr(c) * (1 - ECC);

	for (j = 0; j <= k; j++) {
		switch(j)
		{
			case 0:
			feval[0] = (2 * z - p) / sqrtl(sqr(p) + 4 * f);
			if (feval[0] > 1)
				feval[0] = 1;
			if (feval[0] < -1)
				feval[0] = -1;
			feval[0] = asinl(feval[0]);
			break;

			case 1:
			feval[1] = logl(2 * (f - sqrtl(f * S)) / z + p) / sqrtl(f);
			break;

			default:
			j--;
			feval[j + 1] = - sqrtl(S) / powl(z, j) - p * (2 * j - 1) / 2 * feval[j] + (j - 1) * feval[j - 1];
			feval[j + 1] /= (j * f);
			j++;
			break;
		}

		hsum += powl(-h0, j) * feval[j] * binom(k, j);
	}

	return hsum / 2;
}

int isblock(struct Coordinates *vertex)
{
	int block = 1;
	block &= (vertex->lat == (vertex + 1)->lat && (vertex + 2)->lat == (vertex + 3)->lat && (vertex + 1)->lon == (vertex + 2)->lon && vertex->lon == (vertex + 3)->lon);
	block |= (vertex->lon == (vertex + 1)->lon && (vertex + 2)->lon == (vertex + 3)->lon && (vertex + 1)->lat == (vertex + 2)->lat && vertex->lat == (vertex + 3)->lat);
	return block;
}

int ispolariso(struct Coordinates *vertex)
{
	int k;
	for (k = 0; k < 3; k++) {
		if (fabsl((vertex + k)->lat) / RAD == 90 && (vertex + (k + 1) % 3)->lat == (vertex + (k + 2) % 3)->lat)
			break;
	}
	return k;
}

long double ellipblock(struct Coordinates *vertex)
{
	long double lat[2], lon[2];
	if (vertex->lat < (vertex + 2)->lat) {
		lat[0] = vertex->lat;
		lat[1] = (vertex + 2)->lat;
	}
	else {
		lat[0] = (vertex + 2)->lat;
		lat[1] = vertex->lat;
	}

	if (vertex->lon < (vertex + 2)->lon) {
		lon[0] = vertex->lon;
		lon[1] = (vertex + 2)->lon;
	}
	else {
		lon[0] = (vertex + 2)->lon;
		lon[1] = vertex->lon;
	}

	long double londiff = lon[1] - lon[0];
	long double slat = sinl(lat[1]);
	long double latdiff = slat / (2 * (1 - ECC * sqr(slat))) + logl((1 + sqrtl(ECC) * slat) / (1 - sqrtl(ECC) * slat)) / (4 * sqrtl(ECC));
	slat = sinl(lat[0]);
	latdiff -= slat / (2 * (1 - ECC * sqr(slat))) + logl((1 + sqrtl(ECC) * slat) / (1 - sqrtl(ECC) * slat)) / (4 * sqrtl(ECC));

	return sqr(RAD_MIN) * londiff * latdiff;
}

struct Vector sjoeberg(struct Coordinates *vertex, int i, int s, int a)
{
	struct vincenty_result inter[2];

	long double prev, next, excess = 0;
	long double area, darea = 0;

	long double perimeter = 0;

	int h, k;

	if (a == 1) {
		if (i == 4 && isblock(vertex)) {
			area = ellipblock(vertex);
			a = 0;
		}
		else if (i == 3 && (k = ispolariso(vertex)) < 3) {
			struct Coordinates *triangle = malloc(sizeof(struct Coordinates) * 4);

			triangle->lat = (vertex + k)->lat;
			for (h = 1; h < 4; h++)
				(triangle + h)->lat = (vertex + ((k + h) % 3))->lat;
			triangle->lon = (vertex + ((k + 2) % 3))->lon;
			(triangle + 1)->lon = (vertex + ((k + 1) % 3))->lon;
			(triangle + 2)->lon = (triangle + 1)->lon;
			(triangle + 3)->lon = triangle->lon;

			area = ellipblock(triangle);
			free(triangle);
			a = 0;
		}
	}

	for (h = 0; h < i; h++) {
		inter[0] = vincenty_inverse(vertex + h, vertex + ((h + 1) % i));

		if (s == 1)
			perimeter += inter[0].distance;

		if (a == 1) {
			next = inter[0].start;
			if (h == 0) {
				inter[1] = vincenty_inverse(vertex, vertex + i - 1);
				prev = inter[1].start;
			}
			else
				prev = normalise_a(inter[1].end - M_PI);
			excess += normalise_a(next - prev);

			if ((vertex + h)->lon != (vertex + ((h + 1) % i))->lon && fabsl((vertex + h)->lat) != M_PI_2 && fabsl((vertex + ((h + 1) % i))->lat) != M_PI_2) {
				long double c, interarea;
				for (k = 1; k < 6; k++) {
					interarea = powl(ECC, k) * (k + 1) / (2 * k + 1);
					c = cosl(reduced_latitude((vertex + h)->lat)) * sinl(inter[0].start);
					c = c + cosl(reduced_latitude((vertex + ((h + 1) % i))->lat)) * sinl(inter[0].end);
					c = c / 2;

					if ((vertex + h)->lon < (vertex + ((h + 1) % i))->lon)
						interarea *= (E(vertex + h, k, c) - E(vertex + ((h + 1) % i), k, c));
					else
						interarea *= (E(vertex + ((h + 1) % i), k, c) - E(vertex + h, k, c));
					darea += interarea;
				}
			}

			inter[1] = inter[0];
		}
	}

	if (a == 1) {
		excess = excess - (i - 2) * M_PI;
		area = sqr(RAD_MIN) * (excess + darea);
	}

	struct Vector res;
	res.s = perimeter;
	res.theta = area;
	return res;
}
