//
//  vm.h
//  clox_xcode
//
//  Created by Bruno Paulino on 26.04.24.
//

#ifndef vm_h
#define vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    // Instruction Pointer:
    // location of the currently executing function
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM(void);
void freeVM(void);
void push(Value value);
Value pop(void);
InterpretResult interpret(const char* chunk);

#endif /* vm_h */
