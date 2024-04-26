//
//  debug.h
//  clox_xcode
//
//  Created by Bruno Paulino on 24.04.24.
//

#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk *chunk, const char* name);
int disassembleInstruction(Chunk *chunk, int offset);

#endif /* clox_debug_h */
