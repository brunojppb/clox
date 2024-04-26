//
//  vm.h
//  clox_xcode
//
//  Created by Bruno Paulino on 26.04.24.
//

#ifndef vm_h
#define vm_h

#include "chunk.h"

typedef struct {
    Chunk* chunk;
    // Instruction Pointer:
    // location of the currently executing function
    uint8_t* ip;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM(void);
void freeVM(void);
InterpretResult interpret(Chunk *chunk);

#endif /* vm_h */
