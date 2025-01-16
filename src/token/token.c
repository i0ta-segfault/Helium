#include "lexer.h"

char* getKeywordEnumName(Keywords keyword) {
    if (keyword >= 0 && keyword < Keywords_Count) {
        return keywordsArray[keyword];
    }
    return NULL;
}

char* getTokenTypeName(Token token){
    switch(token.type){
        case IDENTIFIER : return "IDENTIFIER"; 
        case KEYWORDS : return "KEYWORDS"; 
        case INTEGER : return "INTEGER";
        case ASSIGNMENT_OPERATOR : return "ASSIGNMENT_OPERATOR"; 
        case PLUS_OPERATOR : return "PLUS_OPERATOR"; 
        case MINUS_OPERATOR : return "MINUS_OPERATOR"; 
        case LESSER_THAN : return "LESSER_THAN";
        case LESSER_EQUAL : return "LESSER_EQUAL";
        case GREATER_THAN : return "GREATER_THAN";
        case GREATER_EQUAL : return "GREATER_EQUAL";
        case EQUALITY_OPERATOR : return "EQUALITY_OPERATOR";
        case NOT_EQUAL_OPERATOR : return "NOT_EQUAL_OPERATOR";
        case ASTERISK : return "ASTERISK"; 
        case FORWARD_SLASH : return "FORWARD_SLASH"; 
        case MODULUS_OPERATOR : return "MODULUS_OPERATOR"; 
        case SEMICOLON : return "SEMICOLON"; 
        case COMMA : return "COMMA"; 
        case BANG : return "BANG"; 
        case INCREMENT_OPERATOR: return "INCREMENT_OPERATOR";
        case DECREMENT_OPERATOR: return "DECREMENT_OPERATOR";
        case LEFT_PARENTHESIS : return "LEFT_PARENTHESIS"; 
        case RIGHT_PARENTHESIS : return "RIGHT_PARENTHESIS"; 
        case LEFT_CURLY_BRACES : return "LEFT_CURLY_BRACES"; 
        case RIGHT_CURLY_BRACES : return "RIGHT_CURLY_BRACES"; 
        case LEFT_SQUARE_BRACKET : return "LEFT_SQUARE_BRACKET"; 
        case RIGHT_SQUARE_BRACKET : return "RIGHT_SQUARE_BRACKET";
        case ILLEGAL : return "ILLEGAL";
        case ENDOFFILE : return "ENDOFFILE"; 
    }
}

int isKeyword(char* string) {
    if (string == NULL) {
        return 0; // Null safety check
    }

    for (int i = 0; i < Keywords_Count; i++) {
        if (strcmp(string, getKeywordEnumName(i)) == 0) {
            return 1;
        }
    }
    return 0;
}

int isDigit(char character){
    return ((character >= '0' && character <= '9'));
}