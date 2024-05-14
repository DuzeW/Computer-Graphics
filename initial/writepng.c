/*
 * Copyright 2002-2008 Guillaume Cottenceau, 2015 Aleksander Denisiuk
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>


#define OUT_FILE "initials.png"
#define WIDTH 600
#define HEIGHT 600
#define COLOR_TYPE PNG_COLOR_TYPE_RGB
#define BIT_DEPTH 8


void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void create_png_file()
{
	width = WIDTH;
	height = HEIGHT;
        bit_depth = BIT_DEPTH;
        color_type = COLOR_TYPE;

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(width*bit_depth*3);


}


void write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, width, height,
		     bit_depth, color_type, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
	for (y=0; y<height; y++)
		free(row_pointers[y]);
	free(row_pointers);

        fclose(fp);
}
void write_pixel(int x, int y,
                 png_byte cr, png_byte cg, png_byte cb){
                     png_byte* row = row_pointers[y];
                     png_byte* ptr = &(row[x*3]);
                     ptr[0] = cr;
                     ptr[1] = cg;
                     ptr[2] = cb;
                 }


void bresenham(int i1, int j1, int i2, int j2, png_byte cr, png_byte cg,
               png_byte cb) {
  int m, b, i, j, P;
  if (i2 > i1 && j2 >= j1 && j2 - j1 <= i2 - i1) { // 1 x <-> x
    printf("przypadek 1\n");
    m = 2 * (j2 - j1);
    b = 0;
    write_pixel(i1, j1, cr, cg, cb);
    j = j1;
    P = i2 - i1;

    for (i = i1 + 1; i <= i2; i++) {
      write_pixel(i, j, cr, cg, cb);
      b += m;
      if (b > P) {
        j++;
        b = b - 2 * P;
      }
      write_pixel(i, j, cr, cg, cb);
    }
  } else if (j2 > j1 && i2 >= i1 && i2 - i1 <= j2 - j1) { // 2 y <-> y
    printf("przypadek 2\n");
    m = 2 * (i2 - i1);
    b = 0;
    write_pixel(i1, j1, cr, cg, cb);
    i = i1;
    P = j2 - j1;

    for (j = j1 + 1; j <= j2; j++) {
      write_pixel(i, j, cr, cg, cb);
      b += m;
      if (b > P) {
        i++;
        b = b - 2 * P;
      }
      write_pixel(i, j, cr, cg, cb);
    }
  } else if (i2 > i1 && -j2 >= -j1 && -j2 + j1 <= i2 - i1) { // 3 y -> -y
    printf("przypadek 3\n");
    m = 2 * (-j2 + j1);
    b = 0;
    write_pixel(i1, j1, cr, cg, cb);
    j = j1;
    P = i2 - i1;

    for (i = i1 + 1; i <= i2; i++) {
      write_pixel(i, j, cr, cg, cb);
      b += m;
      if (b > P) {
        j--;
        b = b - 2 * P;
      }
      write_pixel(i, j, cr, cg, cb);
    }

  } else if (-j2 > -j1 && i2 >= i1 && i2 - i1 <= -j2 + j1) { // 4 y -> -y ()
    printf("przypadek 4\n");
    m = 2 * (i2 - i1);
    b = 0;
    write_pixel(i1, j1, cr, cg, cb);
    i = i1;
    P = -j2 + j1;

    for (j = j1 - 1; j >= j2; j--) {
      write_pixel(i, j, cr, cg, cb);
      b += m;
      if (b > P) {
        i++;
        b-= 2 * P;
      }
      write_pixel(i, j, cr, cg, cb);
    }

  } else {
    printf("Tego nie powinno byc\n");
  }
}
void drawCircle(int xc, int yc, int r, png_byte cr, png_byte cg, png_byte cb) {
    int x = 0, y = r;
    int P = 1 - r;

    while (x <= y) {
        write_pixel(xc + x, yc + y, cr, cg, cb);
        write_pixel(xc - x, yc + y, cr, cg, cb);
        write_pixel(xc + x, yc - y, cr, cg, cb);
        write_pixel(xc - x, yc - y, cr, cg, cb);
        write_pixel(xc + y, yc + x, cr, cg, cb);
        write_pixel(xc - y, yc + x, cr, cg, cb);
        write_pixel(xc + y, yc - x, cr, cg, cb);
        write_pixel(xc - y, yc - x, cr, cg, cb);

        if (P <= 0) {
            P += 2 * x + 3;
        } else {
            P += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


typedef struct {
    int x, y;
} Point;

void flood_fill(int x, int y, png_byte old_r, png_byte old_g, png_byte old_b, png_byte new_r, png_byte new_g, png_byte new_b) {
    Point* stack = malloc(width * height * sizeof(Point));
    int top = 0;

    stack[top++] = (Point){x, y};

    while (top > 0) {
        Point p = stack[--top];

        png_byte* row = row_pointers[p.y];
        png_byte* ptr = &(row[p.x*3]);

        if (ptr[0] == old_r && ptr[1] == old_g && ptr[2] == old_b) {
            ptr[0] = new_r;
            ptr[1] = new_g;
            ptr[2] = new_b;

            if (p.x+1 < width) stack[top++] = (Point){p.x+1, p.y};
            if (p.x-1 >= 0) stack[top++] = (Point){p.x-1, p.y};
            if (p.y+1 < height) stack[top++] = (Point){p.x, p.y+1};
            if (p.y-1 >= 0) stack[top++] = (Point){p.x, p.y-1};
        }
    }

    free(stack);
}




void process_file(void)
{
	for (y=0; y<height; y++) {
		png_byte* row = row_pointers[y];
		for (x=0; x<width; x++) {
			png_byte* ptr = &(row[x*3]);
			ptr[0] =  ptr[2] = 0;
			ptr[1] = 255;
		}
	}
	drawCircle(300, 250, 200, 0, 0, 200);
    bresenham(157, 199, 182, 294, 150, 3, 210);
    bresenham(157, 199, 169, 199, 150, 3, 210);
    bresenham(169, 199, 188, 280, 150, 3, 210);
    bresenham(188, 280, 211, 199, 150, 3, 210);
    bresenham(182, 294, 195, 294, 150, 3, 210);
    bresenham(195, 294, 218, 211, 150, 3, 210);
    bresenham(211, 199, 226, 199, 150, 3, 210);
    bresenham(218, 211, 241, 294, 150, 3, 210);
    bresenham(226, 199, 247, 280, 150, 3, 210);
    bresenham(247, 280, 267, 199, 150, 3, 210);
    bresenham(267, 199, 279, 199, 150, 3, 210);
    bresenham(241, 294, 253, 294, 150, 3, 210);
    bresenham(253, 294, 279, 199, 150, 3, 210);

    bresenham(317, 199, 342, 294, 150, 3, 210);
    bresenham(317, 199, 329, 199, 150, 3, 210);
    bresenham(329, 199, 348, 280, 150, 3, 210);
    bresenham(348, 280, 371, 199, 150, 3, 210);
    bresenham(342, 294, 355, 294, 150, 3, 210);
    bresenham(355, 294, 378, 211, 150, 3, 210);
    bresenham(371, 199, 386, 199, 150, 3, 210);
    bresenham(378, 211, 401, 294, 150, 3, 210);
    bresenham(386, 199, 407, 280, 150, 3, 210);
    bresenham(407, 280, 427, 199, 150, 3, 210);
    bresenham(427, 199, 439, 199, 150, 3, 210);
    bresenham(401, 294, 413, 294, 150, 3, 210);
    bresenham(413, 294, 439, 199, 150, 3, 210);

    flood_fill(286, 51, 0, 255, 0, 50, 50, 200);
    flood_fill(158, 200, 0, 255, 0, 170, 100, 210);
    flood_fill(318, 200, 0, 255, 0, 170, 100, 210);
}




int main(int argc, char **argv)
{
	create_png_file();
	process_file();
	write_png_file(OUT_FILE);

        return 0;
}
