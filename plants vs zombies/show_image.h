#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "console_draw2.h"
#ifndef SHOW_IMAGE_
#define SHOW_IMAGE_
extern void putString(int x, int y, char *p, int fg_color, int bg_color);

typedef struct _image {
    int row;
    int col;
    char *pixel;
    int *color;
} Image;

extern Image * read_image(char *pixel_file, char *color_file);

extern void show_image(Image *ip, int offx, int offy);
extern void destroy_image(Image *ip);

extern char getpixel(Image *ip, int x, int y);
extern int getcolor(Image *ip, int x, int y);
#endif // SHOW_IMAGE
