#include "display.h"


void init_window(SDL_Window **window,
        SDL_Renderer **renderer,
        SDL_Texture **sdl_texture) {

    (*window) = SDL_CreateWindow(
        "CHIP_8", // window label
        SDL_WINDOWPOS_CENTERED, // initial x pos
        SDL_WINDOWPOS_CENTERED, // initial y pos
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_ALLOW_HIGHDPI // flags
    );

    if (*window == NULL) {
        printf("Error initializing SDL window\n");
        exit(-1);
    }

    (*renderer) = SDL_CreateRenderer((*window), -1, 0);

    if (*renderer == NULL) {
        printf("Error initializing SDL renderer\n");
        exit(-1);
    }

    (*sdl_texture) = SDL_CreateTexture(*renderer, 0, 0, 0, 0);

    // Set the screen to black initially
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void clear_display(Chip8 *chip8) {
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            chip8->display[y][x] = 0;
        }
    }
}

void buffer_display(Chip8 *chip8, uint32_t *buffer) {
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            uint8_t pixel = chip8->display[y][x];
            buffer[(y * DISPLAY_WIDTH) + x] = (0xFFFFFF00 * pixel) | 0xFFFFFF00;
        }
    }
}

void draw_display(uint32_t *buffer,
        SDL_Renderer *renderer,
        SDL_Texture *texture) {
    SDL_UpdateTexture(texture, NULL, buffer, DISPLAY_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void close_window(SDL_Window *window,
        SDL_Renderer *renderer,
        SDL_Texture *texture) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
}

void handle_input(Chip8 *state, int *running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            *running = 0;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_1: state->keys[0x1] = 1; break;
                case SDLK_2: state->keys[0x2] = 1; break;
                case SDLK_3: state->keys[0x3] = 1; break;
                case SDLK_4: state->keys[0xC] = 1; break;

                case SDLK_q: state->keys[0x4] = 1; break;
                case SDLK_w: state->keys[0x5] = 1; break;
                case SDLK_e: state->keys[0x6] = 1; break;
                case SDLK_r: state->keys[0xD] = 1; break;

                case SDLK_a: state->keys[0x7] = 1; break;
                case SDLK_s: state->keys[0x8] = 1; break;
                case SDLK_d: state->keys[0x9] = 1; break;
                case SDLK_f: state->keys[0xE] = 1; break;

                case SDLK_z: state->keys[0xA] = 1; break;
                case SDLK_x: state->keys[0x0] = 1; break;
                case SDLK_c: state->keys[0xB] = 1; break;
                case SDLK_v: state->keys[0xF] = 1; break;

                case SDLK_ESCAPE: *running = 0; break;
            }
        } else if (e.type == SDL_KEYUP) {
            // mirror the above but set to 0
            switch (e.key.keysym.sym) {
                case SDLK_1: state->keys[0x1] = 0; break;
                case SDLK_2: state->keys[0x2] = 0; break;
                case SDLK_3: state->keys[0x3] = 0; break;
                case SDLK_4: state->keys[0xC] = 0; break;

                case SDLK_q: state->keys[0x4] = 0; break;
                case SDLK_w: state->keys[0x5] = 0; break;
                case SDLK_e: state->keys[0x6] = 0; break;
                case SDLK_r: state->keys[0xD] = 0; break;

                case SDLK_a: state->keys[0x7] = 0; break;
                case SDLK_s: state->keys[0x8] = 0; break;
                case SDLK_d: state->keys[0x9] = 0; break;
                case SDLK_f: state->keys[0xE] = 0; break;

                case SDLK_z: state->keys[0xA] = 0; break;
                case SDLK_x: state->keys[0x0] = 0; break;
                case SDLK_c: state->keys[0xB] = 0; break;
                case SDLK_v: state->keys[0xF] = 0; break;
            }
        }
    }
}
