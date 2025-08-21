#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "chip8_t.h"
#include "display.h"

#define PROGRAM_START 0x200

void usage(void) {
    printf("Please supply a ROM as the first command line argument\n");
}

uint16_t get_opcode(Chip8 *state) {
    uint8_t msb = state->ram[state->pc];
    uint8_t lsb = state->ram[state->pc + 1];

    uint16_t opcode = (msb << 8) | lsb;
    return opcode;
}

void unimplemented_instruction(uint16_t opcode){
    printf("Unimpleted Instruction: 0x%08x\n", opcode);
}

void emulate_chip8(Chip8 *state) {
    uint16_t opcode = get_opcode(state);

    state->current_op = opcode;

    state->pc += 2;

    switch (opcode & 0xF000) { // MSB
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // CLS
                    clear_display(state);
                    break;
                case 0x00EE: // RET
                    state->pc = state->stack[--state->sp];
                    break;
            }
        case 0x1000: // JP addr
            state->pc = opcode & 0x0FFF;
            break;
        case 0x2000: // CALL addr
            state->stack[state->sp++] = state->pc;
            state->pc = opcode & 0x0FFF;
            break;
        case 0x3000: // SE Vx, byte
            if (state->registers[(opcode & 0x0F00) << 8] ==
                    (opcode & 0x00FF)) {
                state->pc += 2;
            }
            break;
        case 0x4000: // SNE Vx, byte
            if (state->registers[(opcode & 0x0F00) >> 8]!=
                    (opcode & 0x00FF)) {
                state->pc += 2;
            }
            break;
        case 0x5000: // SE Vx, Vy
            if ((state->registers[(opcode & 0x0F00) >> 8]) ==
                    (state->registers[(opcode & 0x00F0) >> 4])) {
                state->pc += 2;
            }
            break;
        case 0x6000: // LD Vx, byte,
            state->registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        case 0x7000: // ADD Vx, byte
            state->registers[(opcode & 0x0F00) >> 8] += opcode * 0x00FF;
            break;
        case 0x8000: // LD Vx, byte OR 
            // case OR, XOR, etc...
            unimplemented_instruction(opcode);
            break;
        case 0x9000: // SNE, Vx, Vy
            unimplemented_instruction(opcode);
            break;
        case 0xA000: // LD I, addr
            unimplemented_instruction(opcode);
            break;
        case 0xB000: // JP V0, addr
            unimplemented_instruction(opcode);
            break;
        case 0xC000: // RND Vx, byte
            unimplemented_instruction(opcode);
            break;
        case 0xD000: // DRW Vx, Vy, nibble
            unimplemented_instruction(opcode);
            break;
        case 0xE000: // idk
            unimplemented_instruction(opcode);
            break;
        case 0xF000: // idk
            unimplemented_instruction(opcode);
            break;
    }
}


void load_rom(Chip8 *state, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Failed to open ROM");
        exit(-1);
    }

    // find file size
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size + PROGRAM_START > RAM_SIZE) {
        fprintf(stderr, "ROM too large to fit in memory\n");
        fclose(f);
        exit(-1);
    }

    // read file into memory starting at 0x200
    size_t bytesRead = fread(&state->ram[PROGRAM_START], 1, size, f);
    fclose(f);

    if (bytesRead != (size_t)size) {
        fprintf(stderr, "Failed to read full ROM\n");
        exit(-1);
    }

    // set program counter
    state->pc = PROGRAM_START;
}

void init_chip8(Chip8 *state) {
    // load fontset
    for (int i = 0; i < FONTSET_SIZE; i++) {
        state->ram[i] = FONTSET[i];
    }
}


int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
        exit(1);
    }


    Chip8 *chip8 = calloc(1, sizeof(Chip8));

    // Init display
    SDL_Window *chip8_display;
    SDL_Renderer *chip8_renderer;
    SDL_Texture *chip8_texture;
    uint32_t *pixel_buffer = malloc(
        (DISPLAY_HEIGHT * DISPLAY_WIDTH) * sizeof(uint32_t)
    );
    SDL_Init(SDL_INIT_EVERYTHING);
    init_window(&chip8_display, &chip8_renderer, &chip8_texture);


    // initialize and load rom to emulator
    init_chip8(chip8);
    load_rom(chip8, argv[1]);

    int running = 1;
    while (running) {
        emulate_chip8(chip8);

        // update_timers(chip8);

        handle_input(chip8, &running);

        buffer_display(chip8, pixel_buffer);

        draw_display(pixel_buffer, chip8_renderer, chip8_texture);

        usleep(15000); // sleep for 15 ms
    }

    free(pixel_buffer);
    free(chip8);

    return 0;
}
