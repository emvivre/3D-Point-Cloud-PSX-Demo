#ifndef _PIXELFB_H_
#define _PIXELFB_H_

enum { STP_ON = 0, STP_OFF = 1};
enum { DONT_CLEAR_BACKGROUND=0, CLEAR_BACKGROUND=1 };
enum { TEXTURE_4BIT=0, TEXTURE_8BIT=1, TEXTURE_16BIT=2 };
enum { BLEND_50_PLUS_50=0, BLEND_100_PLUS_100=1, BLEND_100_MINUS_100=2, BLEND_100_PLUS_25=3, BLEND_UNKNOWN=4 };
enum { SHADE_TEXTURE_ON = 0, SHADE_TEXTURE_OFF = 1};

struct PixelFB {
    int stp : 1;
    unsigned int r : 5;
    unsigned int g : 5;
    unsigned int b : 5;
    PixelFB() {}
    PixelFB(unsigned int r, unsigned int g, unsigned int b, int stp = STP_OFF) : stp(stp), r(r), g(g), b(b) {}
    operator unsigned short() { return *(unsigned short*)this; }
    static inline PixelFB aliceblue() { return PixelFB(30, 31, 31); }
    static inline PixelFB antiquewhite() { return PixelFB(31, 29, 26); }
    static inline PixelFB aqua() { return PixelFB(0, 31, 31); }
    static inline PixelFB aquamarine() { return PixelFB(15, 31, 26); }
    static inline PixelFB azure() { return PixelFB(30, 31, 31); }
    static inline PixelFB beige() { return PixelFB(30, 30, 27); }
    static inline PixelFB bisque() { return PixelFB(31, 28, 24); }
    static inline PixelFB black() { return PixelFB(0, 0, 0); }
    static inline PixelFB blanchedalmond() { return PixelFB(31, 29, 25); }
    static inline PixelFB blue() { return PixelFB(0, 0, 31); }
    static inline PixelFB blueviolet() { return PixelFB(17, 5, 28); }
    static inline PixelFB brown() { return PixelFB(20, 5, 5); }
    static inline PixelFB burlywood() { return PixelFB(27, 23, 16); }
    static inline PixelFB cadetblue() { return PixelFB(11, 19, 20); }
    static inline PixelFB chartreuse() { return PixelFB(15, 31, 0); }
    static inline PixelFB chocolate() { return PixelFB(26, 13, 3); }
    static inline PixelFB coral() { return PixelFB(31, 15, 10); }
    static inline PixelFB cornflowerblue() { return PixelFB(12, 18, 29); }
    static inline PixelFB cornsilk() { return PixelFB(31, 31, 27); }
    static inline PixelFB crimson() { return PixelFB(27, 2, 7); }
    static inline PixelFB cyan() { return PixelFB(0, 31, 31); }
    static inline PixelFB darkblue() { return PixelFB(0, 0, 17); }
    static inline PixelFB darkcyan() { return PixelFB(0, 17, 17); }
    static inline PixelFB darkgoldenrod() { return PixelFB(23, 16, 1); }
    static inline PixelFB darkgray() { return PixelFB(21, 21, 21); }
    static inline PixelFB darkgreen() { return PixelFB(0, 12, 0); }
    static inline PixelFB darkkhaki() { return PixelFB(23, 22, 13); }
    static inline PixelFB darkmagenta() { return PixelFB(17, 0, 17); }
    static inline PixelFB darkolivegreen() { return PixelFB(10, 13, 5); }
    static inline PixelFB darkorange() { return PixelFB(31, 17, 0); }
    static inline PixelFB darkorchid() { return PixelFB(19, 6, 25); }
    static inline PixelFB darkred() { return PixelFB(17, 0, 0); }
    static inline PixelFB darksalmon() { return PixelFB(29, 18, 15); }
    static inline PixelFB darkseagreen() { return PixelFB(17, 23, 17); }
    static inline PixelFB darkslateblue() { return PixelFB(9, 7, 17); }
    static inline PixelFB darkslategray() { return PixelFB(5, 9, 9); }
    static inline PixelFB darkturquoise() { return PixelFB(0, 25, 26); }
    static inline PixelFB darkviolet() { return PixelFB(18, 0, 26); }
    static inline PixelFB deeppink() { return PixelFB(31, 2, 18); }
    static inline PixelFB deepskyblue() { return PixelFB(0, 23, 31); }
    static inline PixelFB dimgray() { return PixelFB(13, 13, 13); }
    static inline PixelFB dodgerblue() { return PixelFB(3, 18, 31); }
    static inline PixelFB firebrick() { return PixelFB(22, 4, 4); }
    static inline PixelFB floralwhite() { return PixelFB(31, 31, 30); }
    static inline PixelFB forestgreen() { return PixelFB(4, 17, 4); }
    static inline PixelFB fuchsia() { return PixelFB(31, 0, 31); }
    static inline PixelFB gainsboro() { return PixelFB(27, 27, 27); }
    static inline PixelFB ghostwhite() { return PixelFB(31, 31, 31); }
    static inline PixelFB gold() { return PixelFB(31, 26, 0); }
    static inline PixelFB goldenrod() { return PixelFB(27, 20, 4); }
    static inline PixelFB gray() { return PixelFB(16, 16, 16); }
    static inline PixelFB green() { return PixelFB(0, 16, 0); }
    static inline PixelFB greenyellow() { return PixelFB(21, 31, 5); }
    static inline PixelFB honeydew() { return PixelFB(30, 31, 30); }
    static inline PixelFB hotpink() { return PixelFB(31, 13, 22); }
    static inline PixelFB indianred () { return PixelFB(25, 11, 11); }
    static inline PixelFB indigo  () { return PixelFB(9, 0, 16); }
    static inline PixelFB ivory() { return PixelFB(31, 31, 30); }
    static inline PixelFB khaki() { return PixelFB(30, 28, 17); }
    static inline PixelFB lavender() { return PixelFB(28, 28, 31); }
    static inline PixelFB lavenderblush() { return PixelFB(31, 30, 30); }
    static inline PixelFB lawngreen() { return PixelFB(15, 31, 0); }
    static inline PixelFB lemonchiffon() { return PixelFB(31, 31, 25); }
    static inline PixelFB lightblue() { return PixelFB(21, 27, 28); }
    static inline PixelFB lightcoral() { return PixelFB(30, 16, 16); }
    static inline PixelFB lightcyan() { return PixelFB(28, 31, 31); }
    static inline PixelFB lightgoldenrodyellow() { return PixelFB(31, 31, 26); }
    static inline PixelFB lightgray() { return PixelFB(26, 26, 26); }
    static inline PixelFB lightgreen() { return PixelFB(18, 29, 18); }
    static inline PixelFB lightpink() { return PixelFB(31, 22, 24); }
    static inline PixelFB lightsalmon() { return PixelFB(31, 20, 15); }
    static inline PixelFB lightseagreen() { return PixelFB(4, 22, 21); }
    static inline PixelFB lightskyblue() { return PixelFB(16, 25, 31); }
    static inline PixelFB lightslategray() { return PixelFB(14, 17, 19); }
    static inline PixelFB lightsteelblue() { return PixelFB(22, 24, 27); }
    static inline PixelFB lightyellow() { return PixelFB(31, 31, 28); }
    static inline PixelFB lime() { return PixelFB(0, 31, 0); }
    static inline PixelFB limegreen() { return PixelFB(6, 25, 6); }
    static inline PixelFB linen() { return PixelFB(31, 30, 28); }
    static inline PixelFB magenta() { return PixelFB(31, 0, 31); }
    static inline PixelFB maroon() { return PixelFB(16, 0, 0); }
    static inline PixelFB mediumaquamarine() { return PixelFB(12, 25, 21); }
    static inline PixelFB mediumblue() { return PixelFB(0, 0, 25); }
    static inline PixelFB mediumorchid() { return PixelFB(23, 10, 26); }
    static inline PixelFB mediumpurple() { return PixelFB(18, 14, 27); }
    static inline PixelFB mediumseagreen() { return PixelFB(7, 22, 14); }
    static inline PixelFB mediumslateblue() { return PixelFB(15, 13, 29); }
    static inline PixelFB mediumspringgreen() { return PixelFB(0, 31, 19); }
    static inline PixelFB mediumturquoise() { return PixelFB(9, 26, 25); }
    static inline PixelFB mediumvioletred() { return PixelFB(24, 2, 16); }
    static inline PixelFB midnightblue() { return PixelFB(3, 3, 14); }
    static inline PixelFB mintcream() { return PixelFB(30, 31, 31); }
    static inline PixelFB mistyrose() { return PixelFB(31, 28, 28); }
    static inline PixelFB moccasin() { return PixelFB(31, 28, 22); }
    static inline PixelFB navajowhite() { return PixelFB(31, 27, 21); }
    static inline PixelFB navy() { return PixelFB(0, 0, 16); }
    static inline PixelFB oldlace() { return PixelFB(31, 30, 28); }
    static inline PixelFB olive() { return PixelFB(16, 16, 0); }
    static inline PixelFB olivedrab() { return PixelFB(13, 17, 4); }
    static inline PixelFB orange() { return PixelFB(31, 20, 0); }
    static inline PixelFB orangered() { return PixelFB(31, 8, 0); }
    static inline PixelFB orchid() { return PixelFB(27, 14, 26); }
    static inline PixelFB palegoldenrod() { return PixelFB(29, 29, 21); }
    static inline PixelFB palegreen() { return PixelFB(19, 31, 19); }
    static inline PixelFB paleturquoise() { return PixelFB(21, 29, 29); }
    static inline PixelFB palevioletred() { return PixelFB(27, 14, 18); }
    static inline PixelFB papayawhip() { return PixelFB(31, 29, 26); }
    static inline PixelFB peachpuff() { return PixelFB(31, 27, 23); }
    static inline PixelFB peru() { return PixelFB(25, 16, 7); }
    static inline PixelFB pink() { return PixelFB(31, 24, 25); }
    static inline PixelFB plum() { return PixelFB(27, 20, 27); }
    static inline PixelFB powderblue() { return PixelFB(22, 28, 28); }
    static inline PixelFB purple() { return PixelFB(16, 0, 16); }
    static inline PixelFB red() { return PixelFB(31, 0, 0); }
    static inline PixelFB rosybrown() { return PixelFB(23, 17, 17); }
    static inline PixelFB royalblue() { return PixelFB(8, 13, 28); }
    static inline PixelFB saddlebrown() { return PixelFB(17, 8, 2); }
    static inline PixelFB salmon() { return PixelFB(31, 16, 14); }
    static inline PixelFB sandybrown() { return PixelFB(30, 20, 12); }
    static inline PixelFB seagreen() { return PixelFB(5, 17, 10); }
    static inline PixelFB seashell() { return PixelFB(31, 30, 29); }
    static inline PixelFB sienna() { return PixelFB(20, 10, 5); }
    static inline PixelFB silver() { return PixelFB(24, 24, 24); }
    static inline PixelFB skyblue() { return PixelFB(16, 25, 29); }
    static inline PixelFB slateblue() { return PixelFB(13, 11, 25); }
    static inline PixelFB slategray() { return PixelFB(14, 16, 18); }
    static inline PixelFB snow() { return PixelFB(31, 31, 31); }
    static inline PixelFB springgreen() { return PixelFB(0, 31, 15); }
    static inline PixelFB steelblue() { return PixelFB(8, 16, 22); }
    static inline PixelFB tan() { return PixelFB(26, 22, 17); }
    static inline PixelFB teal() { return PixelFB(0, 16, 16); }
    static inline PixelFB thistle() { return PixelFB(27, 23, 27); }
    static inline PixelFB tomato() { return PixelFB(31, 12, 8); }
    static inline PixelFB turquoise() { return PixelFB(8, 28, 26); }
    static inline PixelFB violet() { return PixelFB(29, 16, 29); }
    static inline PixelFB wheat() { return PixelFB(30, 27, 22); }
    static inline PixelFB white() { return PixelFB(31, 31, 31); }
    static inline PixelFB whitesmoke() { return PixelFB(30, 30, 30); }
    static inline PixelFB yellow() { return PixelFB(31, 31, 0); }
    static inline PixelFB yellowgreen() { return PixelFB(19, 25, 6); }
} __attribute__((packed));

#endif
