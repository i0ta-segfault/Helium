#ifndef _LEXER_H_
#define _LEXER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    typedef enum TokenType {
        IDENTIFIER,    // x, y, add, subtract, ...

        KEYWORDS,      // func, if, let, ...

        INTEGER,

        ASSIGNMENT_OPERATOR,
        PLUS_OPERATOR,
        MINUS_OPERATOR,
        MODULUS_OPERATOR,
        ASTERISK,
        FORWARD_SLASH,
        LESSER_THAN,
        LESSER_EQUAL,
        GREATER_THAN,
        GREATER_EQUAL,
        BANG,
        EQUALITY_OPERATOR,
        NOT_EQUAL_OPERATOR,
        INCREMENT_OPERATOR,
        DECREMENT_OPERATOR,
        
        SEMICOLON,
        COMMA,

        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        LEFT_CURLY_BRACES,
        RIGHT_CURLY_BRACES,
        LEFT_SQUARE_BRACKET,
        RIGHT_SQUARE_BRACKET,

        ILLEGAL,
        ENDOFFILE,

        TokenType_Count
    } TokenType;

    typedef enum Keywords{
        let,
        fn,
        Kreturn,
        Kif,
        Kelse,
        Ktrue,
        Kfalse,
        Keywords_Count
    } Keywords;

    typedef struct Token {
        TokenType type;
        char* literal;
        int line_number;
        int column_number;
    } Token;


    typedef struct Lexer {
        char* input;
        int previousPosition;   // position last read from
        int readPosition;       // position to read from next
        char characterRead;
        int currentLine;
        int currentColumn;
    } Lexer;

    // an array of keywords
    extern char* keywordsArray[Keywords_Count];

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