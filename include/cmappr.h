#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#ifndef STB_HEADERS
#define STB_HEADERS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#endif

#define TRUE 1
#define FALSE 0


typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

    typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

typedef struct{

  unsigned char  *data;  // image data
  int   h;  // height
  int   w;  // width
  int   n;  // number of color components

}image;

typedef struct{

  char*   input_file;
  char*   input_file2;
  char*   output_file;

  image  in1;  // image data struct
  image  in2;
  image  out_image;  // output colormap sample image
  image  font_gray;
  image  scaled;//
  image  cs;    // rgb color shifted

  int   block_size;
  int   gray_value;
  int   alpha_blend;
  int   alpha_percent;
  float alpha;

}e;



// prototypes
void abort_(const char * s, ...);
float GetPixelGray(float r, float g, float b);
void block_average(e *E);
void get_approx_grayscale(e *E, int gray_value);
//hsv rgb2hsv(rgb in);
//rgb hsv2rgb(hsv in);

void get_command_line_arg_as_string(char **variable, char *cli_input);
void get_command_line_arg_as_int(int *variable, char *cli_input);
