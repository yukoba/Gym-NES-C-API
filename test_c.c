#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "emulator_c.h"

static int sdlKeySymToKeyNo(int sym) {
    switch (sym) {
        case 13: return 3; // ENTER = START
        case 97: return 0; // A
        case 98: return 1; // B
        case 115: return 2; // S = SELECT
        case 1073741903: return 7; // RIGHT
        case 1073741904: return 6; // LEFT
        case 1073741905: return 5; // DOWN
        case 1073741906: return 4; // UP
        default: return -1;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    Emulator_Handle emulator = emulator_create(argv[1]);
    emulator_reset(emulator);

    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;

    int IMAGE_WIDTH = emulator_get_width();
    int IMAGE_HEIGHT = emulator_get_height();

    NES_Pixel_C* screen = emulator_get_screen_buffer(emulator);
    NES_Byte_C* controller = emulator_get_controller(emulator, 0);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "NES",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "Cannot create a window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "Cannot create a renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        IMAGE_WIDTH,
        IMAGE_HEIGHT
    );
    if (texture == NULL) {
        fprintf(stderr, "Cannot create a texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                int keyNo = sdlKeySymToKeyNo(event.key.keysym.sym);
                if (keyNo >= 0) {
                    if (event.key.state == 0) {
                        *controller &= ~(1 << keyNo);
                    } else {
                        *controller |= (1 << keyNo);
                    }
                }
            }
        }

        emulator_step(emulator);

        if (SDL_UpdateTexture(texture, NULL, screen, IMAGE_WIDTH * sizeof(uint32_t)) < 0) {
            fprintf(stderr, "Failed to update texture: %s\n", SDL_GetError());
            break;
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    emulator_destroy(emulator);

    return 0;
}
