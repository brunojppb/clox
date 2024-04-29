#include <stdio.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char *argv[])
{
    initVM();
    
    Chunk chunk;
    initChunk(&chunk);
    
    // MARK: Arithmatic OP
    // hand-rolled AST for arithmetic operation:
    // -((1.2 + 3.4) / 5.6)
    
    // add the constant value itself to the chunk’s constant pool.
    // That returns the index of the constant in the array.
    // Then we write the constant instruction, starting with its opcode.
    // After that, we write the one-byte constant index operand.
    // Note that writeChunk() can write opcodes or operands.
    // It’s all raw bytes as far as that function is concerned.
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    
    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    
    writeChunk(&chunk, OP_ADD, 123);
    
    constant = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    
    writeChunk(&chunk, OP_DIVIDE, 123);
    
    // END: Arithmatic OP
    
    writeChunk(&chunk, OP_NEGATE, 123);
    
    writeChunk(&chunk, OP_RETURN, 123);
    
    disassembleChunk(&chunk, "Test chunk");
    interpret(&chunk);
    
    freeVM();
    
    freeChunk(&chunk);
    return 0;
}
