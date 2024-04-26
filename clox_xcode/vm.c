//
//  vm.c
//  clox_xcode
//
//  Created by Bruno Paulino on 26.04.24.
//

#include <stdio.h>
#include "common.h"
#include "debug.h"
#include "vm.h"

// static VM to ease things out during the implementation
// but certainly not recommend on a production-grade lang
VM vm;

void initVM(void) {
    
}
void freeVM(void) {
    
}


static InterpretResult run(void) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
        
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
                // TODO: Once we have functions in clox
                // we will reporpose this to return out of the function instead.
            case OP_RETURN: {
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
                break;
            }
        }
    }

#undef READ_BYTE
}

InterpretResult interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

