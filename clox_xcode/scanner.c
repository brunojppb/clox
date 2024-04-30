//
//  scanner.c
//  clox_xcode
//
//  Created by Bruno Paulino on 29.04.24.
//

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

static bool isAtEnd(void) {
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static char advance(void) {
    scanner.current++;
    return scanner.current[-1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    // match found. Consume the next token
    // and advance the current pointer
    scanner.current++;
    return true;
}

// get the current char, but do not consume it
static char peek(void) {
    return *scanner.current;
}

static char peekNext(void) {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static void skipWhitespace(void) {
    for(;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    // comments go all the way to the end of the line
                    // let the newline be handled by the next round
                    // around the loop so lines are incremented
                    while(peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
                
            default:
                return;
        }
    }
}

static Token string(void) {
    while(peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    // Consume the closing double quotes
    advance();
    return makeToken(TOKEN_STRING);
}

static Token number(void) {
    while(isDigit(peek())) {
        advance();
    }
    
    // Floating point numbers
    if (peek() == '.' && isDigit(peekNext())) {
        // consume the '.'
        advance();
        // consume the mantissa
        while(isDigit(peek())) {
            advance();
        }
    }
    
    return makeToken(TOKEN_NUMBER);
}

static TokenType identifierType(void) {
    return TOKEN_IDENTIFIER;
}

static Token identifier(void) {
    while(isAlpha(peek()) || isDigit(peek())) {
        advance();
    }
    return makeToken(identifierType());
}



Token scanToken(void) {
    skipWhitespace();
    scanner.start = scanner.current;
    
    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }
    
    char c = advance();
    
    if (isAlpha(c)) {
        return identifier();
    }
    
    if (isDigit(c)) {
        return number();
    }
    
    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
            if (match('=')) {
                return makeToken(TOKEN_BANG_EQUAL);
            }
            return makeToken(TOKEN_BANG);
        case '=':
            if (match('=')) {
                return makeToken(TOKEN_EQUAL_EQUAL);
            }
            return makeToken(TOKEN_EQUAL);
        case '<':
            if (match('=')) {
                return makeToken(TOKEN_LESS_EQUAL);
            }
            return makeToken(TOKEN_LESS);
        case '>':
            if (match('=')) {
                return makeToken(TOKEN_GREATER_EQUAL);
            }
            return makeToken(TOKEN_GREATER);
        
        case '"': return string();
    }
    
    return errorToken("Unexpected character");
}
