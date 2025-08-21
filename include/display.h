#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8_t.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

void handle_input(Chip8 *chip8, int *running);
void init_window(SDL_Window **window,
        SDL_Renderer **renderer,
        SDL_Texture **sdl_texture);
void buffer_display(Chip8 *chip8, uint32_t *buffer);
void clear_display(Chip8 *chip8);
void draw_display(uint32_t *buffer,
        SDL_Renderer *renderer,
        SDL_Texture *texture);
void close_window(SDL_Window *window,
        SDL_Renderer *renderer,
        SDL_Texture *texture);


#endif
