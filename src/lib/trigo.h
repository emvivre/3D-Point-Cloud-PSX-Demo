#ifndef _TRIGO_H_
#define _TRIGO_H_

#define TRIGO_SCALE 4096
#define TRIGO_PI (4096/2)
#define RAD2DEG(X) (180 * (X) / TRIGO_PI)
#define DEG2RAD(X) (TRIGO_PI * (X) / 180)

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

// sin4k : [0, 4095] -> [-4096, 4096]
//    input :  4096 for 2pi
//    output : 4096 * sin(x)
extern int sin4k[4096];

// cos4k : [0, 4095] -> [-4096, 4096]
extern int cos4k[4096];

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
