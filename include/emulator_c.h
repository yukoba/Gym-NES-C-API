#ifndef EMULATOR_C_H
#define EMULATOR_C_H

#include <stdint.h> // For standard integer types

// Define C-compatible types based on common.hpp
typedef uint8_t NES_Byte_C;
typedef uint16_t NES_Address_C;
typedef uint32_t NES_Pixel_C;

// Opaque handle to the underlying C++ Emulator object
typedef struct Emulator_C* Emulator_Handle;

// Define constants (assuming values from common NES knowledge or ppu.hpp)
// If SCANLINE_VISIBLE_DOTS and VISIBLE_SCANLINES are macros, use them directly.
// Otherwise, these might need to be retrieved via functions if dynamic.
// Assuming standard NES resolution for this example.
#define EMULATOR_WIDTH  (256)
#define EMULATOR_HEIGHT (240)

#ifdef __cplusplus
extern "C" {
#endif

/// Creates a new NES Emulator instance.
///
/// @param rom_path Path to the NES ROM file.
/// @return A handle to the created emulator, or NULL on failure.
Emulator_Handle emulator_create(const char* rom_path);

/// Destroys an NES Emulator instance and frees associated resources.
///
/// @param emulator Handle to the emulator instance to destroy.
void emulator_destroy(Emulator_Handle emulator);

/// Gets a pointer to the emulator's screen buffer (framebuffer).
/// The buffer contains EMULATOR_WIDTH * EMULATOR_HEIGHT pixels.
/// Each pixel is a 32-bit value (e.g., ARGB or RGBA, depending on PPU implementation).
/// The pointer remains valid until the emulator is destroyed.
///
/// @param emulator Handle to the emulator instance.
/// @return Pointer to the screen buffer, or NULL if the handle is invalid.
NES_Pixel_C* emulator_get_screen_buffer(Emulator_Handle emulator);

/// Gets a pointer to the emulator's main RAM buffer (typically 2KB).
/// The pointer remains valid until the emulator is destroyed.
///
/// @param emulator Handle to the emulator instance.
/// @return Pointer to the RAM buffer, or NULL if the handle is invalid.
NES_Byte_C* emulator_get_memory_buffer(Emulator_Handle emulator);

/// Gets a pointer to the specified controller's state buffer.
/// This buffer typically holds 8 bits representing the state of the joypad buttons.
/// The pointer remains valid until the emulator is destroyed.
///
/// @param emulator Handle to the emulator instance.
/// @param port Controller port number (0 or 1).
/// @return Pointer to the controller state buffer, or NULL if the handle or port is invalid.
NES_Byte_C* emulator_get_controller(Emulator_Handle emulator, int port);

/// Resets the emulator to its initial state (like pressing the reset button).
/// Loads the ROM data and initializes CPU and PPU.
///
/// @param emulator Handle to the emulator instance.
void emulator_reset(Emulator_Handle emulator);

/// Executes the emulator for one full video frame.
///
/// @param emulator Handle to the emulator instance.
void emulator_step(Emulator_Handle emulator);

/// Saves the current state of the emulator (CPU, PPU, RAM, etc.).
/// Only one backup state is stored. Calling backup again overwrites the previous one.
///
/// @param emulator Handle to the emulator instance.
void emulator_backup(Emulator_Handle emulator);

/// Restores the emulator state from the previously saved backup.
/// If no backup was made, the behavior is undefined (likely does nothing).
///
/// @param emulator Handle to the emulator instance.
void emulator_restore(Emulator_Handle emulator);

/// Returns the width of the NES screen in pixels.
///
/// @return The screen width.
int emulator_get_width();

/// Returns the height of the NES screen in pixels.
///
/// @return The screen height.
int emulator_get_height();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // EMULATOR_C_H