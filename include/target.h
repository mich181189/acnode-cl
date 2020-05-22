// target.h - define things expected to be implemented by a target

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void target_init();

// this is actually mostly a dummy to make sure the compilation unit doesn't get nuked by the linker
void register_interrupt_handlers();

#ifdef __cplusplus
}

// This stuff is only available in C++ but that's fine

// Get the sytem console - either a debug UART or stdio
class Console;
Console* getConsole();

#endif
