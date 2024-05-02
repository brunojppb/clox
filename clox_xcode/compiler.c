//
//  compiler.c
//  clox_xcode
//
//  Created by Bruno Paulino on 29.04.24.
//

#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "common.h"
#include "scanner.h"
#include "vm.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;
Chunk* compilingChunk;

static Chunk* currentChunk(void) {
    return compilingChunk;
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    // To control whether our we will execute any bytecode
    // at a later step. We keep following along the code
    // as usual, but if panicMode is active, we don't run.
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);
    
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if(token->type == TOKEN_ERROR) {
        // nothing here for now
    } else {
        // Show the lexeme based on the length of the token
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }
    
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char *message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}


static void advance(void) {
    parser.previous = parser.current;
    
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        
        errorAtCurrent(parser.current.start);
    }
}

// Consumes the next token and advance
// handy for jumping to the one dfter the next token
// and validating whether the given toke type matches with the very next one.
static void consume(TokenType type, const char* message) {
    if(parser.current.type == type) {
        advance();
        return;
    }
    
    errorAtCurrent(message);
}

static void emitReturn(void) {
    emitByte(OP_RETURN);
}

static void endCompiler(void) {
    emitReturn();
}

static void expression(void) {
    // let's see what goes here.
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    compilingChunk = chunk;
    parser.hadError = false;
    parser.panicMode = false;
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
    endCompiler();
    return !parser.hadError;
}
