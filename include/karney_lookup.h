#include "geodesic.h"

#ifndef __HAVE_KARNEY_LOOKUP_H__

#define __HAVE_KARNEY_LOOKUP_H__

long double Clookup[6][6] =
{
{
2/3 - ECC2/15 + 4*ECC2*ECC2/105 - 8*ECC2*ECC2*ECC2/315 + 64*ECC2*ECC2*ECC2*ECC2/3465 - 128*ECC2*ECC2*ECC2*ECC2*ECC2/9009*ECC2,
- 1/20 + ECC2/35 - 2*ECC2*ECC2/105 + 16*ECC2*ECC2*ECC2/1155 - 32*ECC2*ECC2*ECC2*ECC2/3003,
1/42 - ECC2/63 + 8*ECC2*ECC2/693 - 80*ECC2*ECC2*ECC2/9009,
- 1/72 + ECC2/99 - 10*ECC2*ECC2/1287,
1/110 - ECC2/143,
- 1/156
},
{
0,
1/180 - ECC2/314 + 2*ECC2*ECC2/945 - 16*ECC2*ECC2*ECC2/10395 + 32*ECC2*ECC2*ECC2*ECC2/27027,
- 1/252 + ECC2/378 - 4*ECC2*ECC2/2079 + 40*ECC2*ECC2*ECC2/27027,
1/360 - ECC2/495 + 2*ECC2*ECC2/1287,
- 1/495 + 2*ECC2/1287,
5/3276
},
{
0,
0,
1/2100 - 1*ECC2/3150 + 4*ECC2*ECC2/17325 - 8*ECC2*ECC2*ECC2/45045,
- 1/1800 + ECC2/2475 - 2*ECC2*ECC2/6435,
1/1925 - 2*ECC2/5005,
- 1/2184
},
{
0,
0,
0,
1/17640 - ECC2/24255 + 2*ECC2*ECC2/63063,
- 1/10780 + ECC2/14014,
4/45864
},
{
0,
0,
0,
0,
1/124740 - ECC2/162162,
- 1/58968
},
{
0,
0,
0,
0,
0,
1/792792
}
};

#endif

