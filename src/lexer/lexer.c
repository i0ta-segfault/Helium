#include "lexer.h"

// init keywords hash map
char* keywordsArray[Keywords_Count] = {
    "let",
    "fn",
    "return",
    "if",
    "else",
    "true",
    "false",
    "int",
    "float"
};

char* datatypesArray[DataTypeCount] = {
    "int",
    "float"
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

int isDigit(char character){
    return ((character >= '0' && character <= '9'));
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

char* readNumber(Lexer* lexer) { // numbers like 29_000 are valid
    int start = lexer->previousPosition;
    int numberIsDecimal = 0;
    while (isDigit(lexer->characterRead) || lexer->characterRead == '.' || lexer->characterRead == '_') {
        if (lexer->characterRead == '.') {
            if (numberIsDecimal) // deal with multiple decimal points, like 3.1.4
                break;
            numberIsDecimal = 1;
        } 
        else if (lexer->characterRead == '_') {
            readCharacter(lexer);
            continue;
        }
        readCharacter(lexer);
    }

    int length = lexer->readPosition - start;
    char* rawNumber = strndup(&lexer->input[start], length - 1);
    char* cleanNumber = (char*)malloc(length);
    int j = 0;
    for (int i = 0; i < strlen(rawNumber); i++) {
        if (rawNumber[i] != '_') {
            cleanNumber[j++] = rawNumber[i];
        }
    }
    cleanNumber[j] = '\0';
    free(rawNumber);
    return cleanNumber;
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
    Token token = {0};
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
        case ':' :
            token.type = COLON;
            token.literal = strdup(":");
            break;
        case 0 : 
            token.type = ENDOFFILE;
            token.literal = strdup("");
            break;
        default : 
            if(isLetter(lexer->characterRead)){
                char* identifier = readIdentifier(lexer);
                if(isKeyword(identifier)){
                    if(isDataType(identifier))
                        token.type = Data_Type;
                    else
                        token.type = KEYWORDS;
                }
                else{
                    token.type = IDENTIFIER;
                }
                token.literal = identifier;
                return token;
            }
            else if(isDigit(lexer->characterRead)){
                char* numberLiteral = readNumber(lexer);
                if(strchr(numberLiteral, '.')){
                    token.type = FLOAT_LITERAL;
                } else {
                    token.type = INTEGER_LITERAL;
                }
                token.literal = numberLiteral;
                return token;
            }
            else{
                token.type = ILLEGAL;
                token.literal = (char*)malloc(2 * sizeof(char));
                if (!token.literal) {
                    fprintf(stderr, "Memory allocation failed for illegal token literal\n");
                    exit(EXIT_FAILURE);
                }
                token.literal[0] = lexer->characterRead;
                token.literal[1] = '\0';
                // error reporting
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
    Lexer lexer = initLexer(sourceCode);
    int capacity = INITIAL_TOKEN_CAPACITY;
    *tokens = (Token*)malloc(capacity * sizeof(Token));
    if (!*tokens) {
        fprintf(stderr, "Failed to allocate memory for tokens\n");
        exit(EXIT_FAILURE);
    }
    *tokenCount = 0;
    while (1) {
        Token token = nextToken(&lexer);
        if (*tokenCount >= capacity) {
            capacity *= 2;
            *tokens = (Token*)realloc(*tokens, capacity * sizeof(Token));
            if (!*tokens) {
                fprintf(stderr, "Failed to reallocate memory for tokens\n");
                exit(EXIT_FAILURE);
            }
        }
        (*tokens)[*tokenCount] = token;
        (*tokenCount)++;
        if (token.type == ENDOFFILE) {
            break;
        }
    }
}