// target.h - define things expected to be implemented by a target

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void sys_init();

// this is actually mostly a dummy to make sure the compilation unit doesn't get nuked by the linker
void register_interrupt_handlers();

#ifdef __cplusplus
}
#endif
