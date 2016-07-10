#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/types.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

extern const int OT_LENGTH;
extern u_long* ot;
extern void set_clear_color(unsigned char r, unsigned char g, unsigned char b);
extern void set_clear_color(unsigned char i);
extern void clear_color_buffer();
extern void enable_auto_clear_color_buffer();
extern void disable_auto_clear_color_buffer();
extern void display();

#endif 