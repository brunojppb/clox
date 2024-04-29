//
//  compiler.c
//  clox_xcode
//
//  Created by Bruno Paulino on 29.04.24.
//

#include <stdio.h>
#include "compiler.h"
#include "common.h"
#include "scanner.h"

void compile(const char* source) {
    initScanner(source);
    int line = -1;
    for(;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        // Using * instead lets you pass the precision as an argument.
        // So that printf() call prints the first token.length characters
        // of the string at token.start
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        
        if (token.type == TOKEN_EOF) {
            break;
        }
    }
}
