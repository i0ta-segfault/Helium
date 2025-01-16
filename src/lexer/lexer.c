#include "lexer.h"

// init keywords hash map
char* keywordsArray[Keywords_Count] = {
    "let",
    "fn",
    "return",
    "if",
    "else",
    "true",
    "false"
};

void readCharacter(Lexer* lexer) {
    if (lexer->readPosition >= strlen(lexer->input)) {
        lexer->characterRead = 0; // End of input
    } else {
        lexer->characterRead = lexer->input[lexer->readPosition];
        if (lexer->characterRead == '\n') {
            lexer->currentLine++;
            lexer->currentColumn = 1;
        }
        else {
            lexer->currentColumn++;
        }
    }
    lexer->previousPosition = lexer->readPosition;
    lexer->readPosition++;
}

int isLetter(char character){
    // identifiers like blue_bear will work because of the '_' check
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character == '_'));
}

void skipWhiteSpace(Lexer* lexer){
    while(lexer->characterRead == ' ' || lexer->characterRead == '\n' || lexer->characterRead == '\t' || lexer->characterRead == '\r'){
        readCharacter(lexer);
    }
}

char* readIdentifier(Lexer* lexer){
    int start = lexer->previousPosition;
    while(isLetter(lexer->characterRead)){
        readCharacter(lexer);
    }
    int length = lexer->readPosition - start;
    return strndup(&lexer->input[start], length - 1);
}

char* readNumber(Lexer* lexer) {
    int start = lexer->previousPosition;
    while (isDigit(lexer->characterRead)) {
        readCharacter(lexer);
    }
    int length = lexer->readPosition - start;
    return strndup(&lexer->input[start], length - 1);
}

char peekChar(Lexer* lexer){
    if(lexer->readPosition >= strlen(lexer->input)){
        return 0;
    }
    else{
        return lexer->input[lexer->readPosition];
    }
}

Token nextToken(Lexer* lexer){
    Token token;
    skipWhiteSpace(lexer);

    token.line_number = lexer->currentLine;
    token.column_number = lexer->currentColumn;

    switch(lexer->characterRead){
        case '=' :
            if(peekChar(lexer) == '='){
                readCharacter(lexer);
                token.type = EQUALITY_OPERATOR;
                token.literal = strdup("==");
            }
            else{
            token.type = ASSIGNMENT_OPERATOR;
            token.literal = strdup("=");
            }
            break;
        case '+' : 
            if(peekChar(lexer) == '+'){
                readCharacter(lexer);
                token.type = INCREMENT_OPERATOR;
                token.literal = strdup("++");
            }
            else{
                token.type = PLUS_OPERATOR;
                token.literal = strdup("+");
            }
            break;
        case '-' : 
            if(peekChar(lexer) == '-'){
                readCharacter(lexer);
                token.type = DECREMENT_OPERATOR;
                token.literal = strdup("--");
            }
            else{
                token.type = MINUS_OPERATOR;
                token.literal = strdup("-");
            }
            break;
        case '*' : 
            token.type = ASTERISK;
            token.literal = strdup("*");
            break;
        case '/' :
            if(peekChar(lexer) == '/'){
                while(lexer->characterRead != '\n' && lexer->characterRead != 0){
                    readCharacter(lexer);
                }
                return nextToken(lexer);
            }
            else{
                token.type = FORWARD_SLASH;
                token.literal = strdup("/");
            }
            break;
        case '!' : 
            if(peekChar(lexer) == '='){
                readCharacter(lexer);
                token.type = NOT_EQUAL_OPERATOR;
                token.literal = strdup("!=");
            }
            else{
                token.type = BANG;
                token.literal = strdup("!");
            }
            break;
        case '<' : 
            if(peekChar(lexer) == '='){
                readCharacter(lexer);
                token.type = LESSER_EQUAL;
                token.literal = strdup("<=");
            }
            else{
                token.type = LESSER_THAN;
                token.literal = strdup("<");
            }    
            break;
        case '>' : 
            if(peekChar(lexer) == '='){
                readCharacter(lexer);
                token.type = GREATER_EQUAL;
                token.literal = strdup(">=");
            }
            else{
                token.type = GREATER_THAN;
                token.literal = strdup(">");
            }    
            break;
        case '(' : 
            token.type = LEFT_PARENTHESIS;
            token.literal = strdup("(");
            break;
        case ')' : 
            token.type = RIGHT_PARENTHESIS;
            token.literal = strdup(")");
            break;
        case '{' : 
            token.type = LEFT_CURLY_BRACES;
            token.literal = strdup("{");
            break;
        case '}' : 
            token.type = RIGHT_CURLY_BRACES;
            token.literal = strdup("}");
            break;
        case ',' : 
            token.type = COMMA;
            token.literal = strdup(",");
            break;
        case ';' : 
            token.type = SEMICOLON;
            token.literal = strdup(";");
            break;
        case 0 : 
            token.type = ENDOFFILE;
            token.literal = strdup("");
            break;
        default : 
            if(isLetter(lexer->characterRead)){
                char* identifier = readIdentifier(lexer);
                if(isKeyword(identifier)){
                    token.type = KEYWORDS;
                }
                else{
                    token.type = IDENTIFIER;
                }
                token.literal = identifier;
                return token;
            }
            else if(isDigit(lexer->characterRead)){
                token.type = INTEGER;
                token.literal = readNumber(lexer);\
                return token;
            }
            else{
                token.type = ILLEGAL;
                token.literal = (char*)malloc(2 * sizeof(char));
                token.literal[0] = lexer->characterRead;
                token.literal[1] = '\0';
                // Error reporting
                fprintf(stderr, "[%d : %d] Error encountered - Illegal token : %c found\n",
                        token.line_number,
                        token.column_number,
                        lexer->characterRead);
            }
    }
    readCharacter(lexer);
    return token;
}

Lexer initLexer(char* sourceCode){
    Lexer lexer = {
        .input = sourceCode,
        .previousPosition = 0,
        .readPosition = 0,
        .currentLine = 1,
        .currentColumn = 1
    };
    readCharacter(&lexer);
    return lexer;
}

void getAllTokens(char* sourceCode, Token** tokens, int* tokenCount) {

    // Initialize lexer struct
    Lexer lexer = initLexer(sourceCode);

    *tokens = (Token*)malloc(sizeof(Token));
    if (!*tokens) {
        fprintf(stderr, "Failed to allocate memory for tokens\n");
        exit(EXIT_FAILURE);
    }
    *tokenCount = 0;

    while (1) {
        Token token = nextToken(&lexer);
        *tokens = (Token*)realloc(*tokens, (*tokenCount + 1) * sizeof(Token));
        if (!*tokens) {
            fprintf(stderr, "Failed to reallocate memory for tokens\n");
            exit(EXIT_FAILURE);
        }
        (*tokens)[*tokenCount] = token;
        (*tokenCount)++;

        if (token.type == ENDOFFILE) {
            break;
        }
    }
}