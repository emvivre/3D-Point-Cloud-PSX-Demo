#ifndef _BISHOP_H_
#define _BISHOP_H_

#include <sys/types.h>
#include <libgte.h>

#define BISHOP_NB_POINTS 40000

extern SVECTOR bishop_point_cloud[BISHOP_NB_POINTS];
extern u_char bishop_point_cloud_color[BISHOP_NB_POINTS][3];

#endif