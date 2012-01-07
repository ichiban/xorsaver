#include "xorsaver.h"

int main(int argc, char **argv)
{
  SDL_Surface *screen;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "init failed: %s\n", SDL_GetError());
    return 1;
  }

  SDL_ShowCursor(SDL_DISABLE);

  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH,
				  SDL_FULLSCREEN | SDL_HWSURFACE))) {
    fprintf(stderr, "settint video mode failed: %s\n", SDL_GetError());
    return 1;
  }

  while (SDL_PollEvent(&event) >= 0) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      SDL_Quit();
      break;
    default:
      draw_xor_texture(screen);
    }
  }

  fprintf(stderr, "waiting for events failed: %s\n", SDL_GetError());
  return 1;
}

void draw_xor_texture(SDL_Surface *surface)
{
  Uint32 t = SDL_GetTicks();
  int x, y;
  Uint8 xor;
  Uint32 color;

  if (SDL_MUSTLOCK(surface)) {
    if (SDL_LockSurface(surface) < 0) {
      fprintf(stderr, "lock failed: %s\n", SDL_GetError());
      return;
    }
  }

  for (y = 0; y < HEIGHT; y++) {  
    for (x = 0; x < WIDTH; x++) {
      xor = (x ^ y) + t;
      color = SDL_MapRGB(surface->format, xor, xor, xor);
      putpixel(surface, x, y, color);
    }
  }

  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }

  SDL_UpdateRect(surface, 0, 0, 0, 0);
}

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
