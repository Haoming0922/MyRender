#ifndef LINE_H
#define LINE_H

#include "tgaimage.h"
#include <cmath>
 
// Xiaolin Wu's line algorithm (antialiasing)
void lineWu(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

// Bresenham's line algorithm
void lineBresenham(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);


#endif