#ifndef XORSAVER_H
#define XORSAVER_H

#include <stdio.h>
#include <SDL.h>

#define WIDTH 640
#define HEIGHT 480
#define DEPTH 16

int main(int argc, char **argv);
void draw_xor_texture(SDL_Surface *surface);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
