#include "emulator_c.h" // Include the C header
#include "emulator.hpp" // Include the C++ header
#include "common.hpp"   // For NES types

#include <string>
#include <vector> // Potentially needed by Cartridge or other components
#include <new>    // For std::nothrow

// Define the opaque struct type just to satisfy the compiler for casting
// The actual C++ class definition is hidden from C users.
struct Emulator_C {
    NES::Emulator* instance;
};

// Helper function to get the C++ instance pointer safely
static inline NES::Emulator* get_instance(Emulator_Handle handle) {
    if (!handle) {
        return nullptr;
    }
    return handle->instance;
}


extern "C" {

Emulator_Handle emulator_create(const char* rom_path) {
    // Use std::nothrow to prevent exceptions from crashing C code
    Emulator_C* handle = new (std::nothrow) Emulator_C;
    if (!handle) {
        return NULL; // Allocation failed
    }

    try {
        handle->instance = new NES::Emulator(std::string(rom_path));
        // Check if constants match (optional sanity check)
        // static_assert(EMULATOR_WIDTH == NES::Emulator::WIDTH, "Width mismatch");
        // static_assert(EMULATOR_HEIGHT == NES::Emulator::HEIGHT, "Height mismatch");
        return handle;
    } catch (...) { // Catch potential exceptions from Emulator constructor (e.g., file not found)
        delete handle; // Clean up the allocated handle struct
        return NULL;   // Indicate failure
    }
}

void emulator_destroy(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (instance) {
        delete instance; // Call C++ destructor
    }
    delete handle; // Free the handle struct itself
}

NES_Pixel_C* emulator_get_screen_buffer(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (!instance) {
        return NULL;
    }
    // NES_Pixel and NES_Pixel_C are typedefs for uint32_t, so cast is safe
    return reinterpret_cast<NES_Pixel_C*>(instance->get_screen_buffer());
}

NES_Byte_C* emulator_get_memory_buffer(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (!instance) {
        return NULL;
    }
    // NES_Byte and NES_Byte_C are typedefs for uint8_t, so cast is safe
    return reinterpret_cast<NES_Byte_C*>(instance->get_memory_buffer());
}

NES_Byte_C* emulator_get_controller(Emulator_Handle handle, int port) {
    NES::Emulator* instance = get_instance(handle);
    // Check for valid instance and port range (assuming 2 controllers)
    if (!instance || (port != 0 && port != 1)) {
        return NULL;
    }
    // NES_Byte and NES_Byte_C are typedefs for uint8_t, so cast is safe
    return reinterpret_cast<NES_Byte_C*>(instance->get_controller(port));
}

void emulator_reset(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (instance) {
        instance->reset();
    }
}

void emulator_step(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (instance) {
        instance->step();
    }
}

void emulator_backup(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (instance) {
        instance->backup();
    }
}

void emulator_restore(Emulator_Handle handle) {
    NES::Emulator* instance = get_instance(handle);
    if (instance) {
        instance->restore();
    }
}

// Retrieve constants via C++ static members
int emulator_get_width() {
    return NES::Emulator::WIDTH;
}

int emulator_get_height() {
    return NES::Emulator::HEIGHT;
}

} // extern "C"