#ifndef _LEXER_H_
#define _LEXER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "types.h"
    #include "token.h"

    typedef struct Lexer {
        char* input;
        int previousPosition;   // position last read from
        int readPosition;       // position to read from next
        char characterRead;
        int currentLine;
        int currentColumn;
    } Lexer;

    Lexer initLexer(char*);

    // helper function to log out token type name
    char* enumNametoString(TokenType);
    void skipWhiteSpace(Lexer*);
    int isLetter(char);
    int isDigit(char);
    char* readIdentifier(Lexer*);
    char* readNumber(Lexer*);

    // helper function to look ahead
    char peekChar(Lexer*);

    // this function will take in the source code and an array of type Token that will be filled
    // and dynamically resized with all the tokens in the source code
    void getAllTokens(char*, Token**, int*);
    Token nextToken(Lexer*);
    void readCharacter(Lexer*);

    // helper function to check if identifier is keyword and get keyowrd enum name
    char* getKeywordEnumName(Keywords);
    int isKeyword(char*);

    // helper function to get token type name
    char* getTokenTypeName(Token);

#endif