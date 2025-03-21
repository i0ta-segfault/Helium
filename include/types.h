#ifndef _TYPES_H_
#define _TYPES_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef enum DataType {
        integer,
        floating,
        string,
        DataTypeCount
    } DataType;

    typedef enum TokenType {
        IDENTIFIER,    // x, y, add, subtract, ...
        KEYWORDS,      // func, if, let, ...

        COLON,
        Data_Type,     // integer, boolean
        
        INTEGER_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        
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
        
        ARROW_OPERATOR,

        SEMICOLON,
        COMMA,
        DOUBLE_QUOTES,
        SINGLE_QUOTES,
        
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        LEFT_CURLY_BRACES,
        RIGHT_CURLY_BRACES,
        LEFT_SQUARE_BRACKET,
        RIGHT_SQUARE_BRACKET,

        BUILT_IN_FUNCTION,
        
        ILLEGAL,
        ENDOFFILE,
        TokenType_Count
    } TokenType;

    typedef enum Keywords {
        let,
        fn,
        print,
        Kreturn,
        Kif,
        Kelse,
        Ktrue,
        Kfalse,
        Kint,
        Kfloat,
        Kstring,
        Keywords_Count
    } Keywords;

    typedef struct Token {
        TokenType type;
        char* literal;
        int line_number;
        int column_number;
    } Token;

    // an array of keywords
    extern char* keywordsArray[Keywords_Count];
    // an array of data types
    extern char* datatypesArray[DataTypeCount];

#endif