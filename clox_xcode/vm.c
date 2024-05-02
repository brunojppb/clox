//
//  vm.c
//  clox_xcode
//
//  Created by Bruno Paulino on 26.04.24.
//

#include <stdio.h>
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "vm.h"

// static VM to ease things out during the implementation
// but certainly not recommend on a production-grade lang
VM vm;

static void resetStack(void) {
    vm.stackTop = vm.stack;
}


static InterpretResult run(void) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while(false)

    for (;;) {
        
#ifdef DEBUG_TRACE_EXECUTION
        printf("            ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;
            case OP_NEGATE:
                push(-pop());
                break;
                // TODO: Once we have functions in clox
                // we will reporpose this to return out of the function instead.
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);
    
    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;
    
    InterpretResult result = run();
    
    freeChunk(&chunk);
    return result;
}

void initVM(void) {
    resetStack();
}
void freeVM(void) {
    
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop(void) {
    vm.stackTop--;
    return *vm.stackTop;
}
