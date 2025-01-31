#include "parser.h"

Token currentToken_Parser(Parser* parser){
    return parser->currentToken;
}

Token peekToken_Parser(Parser* parser){
    return parser->nextToken;
}

void advance_token_Parser(Parser* parser){
    parser->currentToken = parser->nextToken;
    parser->nextToken = nextToken(parser->lexer);
}

void parser_throw_error(Parser* parser, const char* expected){
    printf("Error at [%d:%d]: Expected '%s', got '%s'\n",
           parser->currentToken.line_number, parser->currentToken.column_number,
           expected, parser->currentToken.literal);
}

int is_infix_operator(Token token) {
    //    +, -, *, /, %, >=, <=, <, >, !=, ==, ->
    return token.type == PLUS_OPERATOR || token.type == MINUS_OPERATOR ||
           token.type == ASTERISK || token.type == FORWARD_SLASH || token.type == MODULUS_OPERATOR ||
           token.type == GREATER_THAN || token.type == LESSER_THAN ||
           token.type == GREATER_EQUAL || token.type == LESSER_EQUAL ||
           token.type == EQUALITY_OPERATOR || token.type == NOT_EQUAL_OPERATOR ||
           token.type == ARROW_OPERATOR;
}

int get_precedence(Token token) {
    switch (token.type) {
        case ARROW_OPERATOR:
            return 30;
        case PLUS_OPERATOR:
        case MINUS_OPERATOR:
            return 10;
        case ASTERISK:
        case FORWARD_SLASH:
        case MODULUS_OPERATOR:
            return 20;
        case GREATER_THAN:
        case LESSER_THAN:
        case GREATER_EQUAL:
        case LESSER_EQUAL:
            return 5;
        case EQUALITY_OPERATOR:
        case NOT_EQUAL_OPERATOR:
            return 3;
        default:  // non operators, invalid tokens and the kind
            return 0;
    }
}

Expression* parse_expression(Parser* parser) {
    Expression* left = parse_prefix_expression(parser);
    return parse_infix_expression(parser, left, 0);
}

Expression* parse_prefix_operator_expression(Parser* parser){
    Token currentToken = parser->currentToken;  // get the prefix operator
    advance_token_Parser(parser);
    Expression* operand = parse_prefix_expression(parser);

    if (operand == NULL) {
        parser_throw_error(parser, "expression after prefix operator");
        return NULL;
    }
    PrefixExpression* prefixExpr = (PrefixExpression*)malloc(sizeof(PrefixExpression));
    prefixExpr->expr.node.type = NODE_EXPR;
    prefixExpr->expr.expr_type = EXPR_PREFIX;
    prefixExpr->prefix_operator_token = currentToken;
    prefixExpr->right = operand;
    return (Expression*)prefixExpr;
}

Expression* parse_prefix_expression(Parser* parser) {
    Token currentToken = parser->currentToken;
    switch (currentToken.type) {
        case IDENTIFIER:
            return parse_identifier_expression(parser);
        case INTEGER_LITERAL:
            return parse_integer_literal_expression(parser);
        case FLOAT_LITERAL:
            return parse_float_literal_expression(parser);
        case STRING_LITERAL:
            return parse_string_literal_expression(parser);
        case LEFT_PARENTHESIS:  // sub expressions
            return parse_grouped_expression(parser);
        case MINUS_OPERATOR:
        case BANG:
        case LESSER_EQUAL:
        case LESSER_THAN:
        case GREATER_EQUAL:
        case GREATER_THAN:
            return parse_prefix_operator_expression(parser);
        default:
            parser_throw_error(parser, "Unexpected token in expression");
            return NULL;
    }
}

Expression* parse_identifier_expression(Parser* parser) {
    Token identifierToken = parser->currentToken;
    advance_token_Parser(parser);
    if (parser->currentToken.type == INCREMENT_OPERATOR || parser->currentToken.type == DECREMENT_OPERATOR) {
        PostfixExpression* postfixExpr = (PostfixExpression*)malloc(sizeof(PostfixExpression));
        postfixExpr->expr.node.type = NODE_EXPR;
        postfixExpr->expr.expr_type = EXPR_POSTFIX;
        postfixExpr->left = (Expression*)malloc(sizeof(IdentifierExpression));
        ((IdentifierExpression*)postfixExpr->left)->identifier_token = identifierToken;
        postfixExpr->postfix_operator_token = parser->currentToken;
        advance_token_Parser(parser);
        return (Expression*)postfixExpr;
    }
    IdentifierExpression* identifierExpr = (IdentifierExpression*)malloc(sizeof(IdentifierExpression));
    identifierExpr->expr.node.type = NODE_EXPR;
    identifierExpr->expr.expr_type = EXPR_IDENTIFIER;
    identifierExpr->identifier_token = identifierToken;
    return (Expression*)identifierExpr;
}

Expression* parse_integer_literal_expression(Parser* parser) {
    Token integerToken = parser->currentToken;
    advance_token_Parser(parser);
    IntegerLiteral* integerLiteral = (IntegerLiteral*)malloc(sizeof(IntegerLiteral));
    integerLiteral->expr.node.type = NODE_EXPR;
    integerLiteral->expr.expr_type = EXPR_INTEGER;
    integerLiteral->integer_token = integerToken;
    return (Expression*)integerLiteral;
}

Expression* parse_float_literal_expression(Parser* parser) {
    Token floatToken = parser->currentToken;
    advance_token_Parser(parser);
    FloatingLiteral* floatingLiteral = (FloatingLiteral*)malloc(sizeof(FloatingLiteral));
    floatingLiteral->expr.node.type = NODE_EXPR;
    floatingLiteral->expr.expr_type = EXPR_FLOAT;
    floatingLiteral->float_token = floatToken;
    return (Expression*)floatingLiteral;
}

Expression* parse_string_literal_expression(Parser* parser) {
    Token stringToken = parser->currentToken;
    advance_token_Parser(parser);
    StringLiteral* stringLiteral = (StringLiteral*)malloc(sizeof(StringLiteral));
    stringLiteral->expr.node.type = NODE_EXPR;
    stringLiteral->expr.expr_type = EXPR_STRING;
    stringLiteral->string_token = stringToken;
    return (Expression*)stringLiteral;
}

// Parses grouped expressions inside parentheses (e.g., (5 + 3))
Expression* parse_grouped_expression(Parser* parser) {
    advance_token_Parser(parser);  // Consume '('
    Expression* expr = parse_expression(parser);  // Parse the inner expression
    if (parser->currentToken.type != RIGHT_PARENTHESIS) {
        parser_throw_error(parser, "Expected closing parenthesis ')'");
        return NULL;
    }
    advance_token_Parser(parser);  // Consume ')'
    return expr;
}

// (5 + 3) or (x * y) kinda expressions
Expression* parse_infix_expression(Parser* parser, Expression* left, int precedence) {
    Token currentToken = parser->currentToken;
    while (is_infix_operator(currentToken) && get_precedence(currentToken) > precedence) {
        advance_token_Parser(parser);
        Expression* right = parse_prefix_expression(parser);
        InfixExpression* infixExpr = (InfixExpression*)malloc(sizeof(InfixExpression));
        infixExpr->expr.node.type = NODE_EXPR;
        infixExpr->expr.expr_type = EXPR_INFIX;
        infixExpr->left = left;
        infixExpr->operator_token = currentToken;
        infixExpr->right = right;
        left = (Expression*)infixExpr;

        currentToken = parser->currentToken;
    }
    return left;
}

AssignmentStatement* parse_assignment_statement(Parser* parser) {
    Token identifierToken = parser->currentToken;
    advance_token_Parser(parser);
    if (parser->currentToken.type != ASSIGNMENT_OPERATOR) {
        parser_throw_error(parser, "assignment operator '='");
        synchronize(parser);
        return NULL;
    }
    advance_token_Parser(parser);
    Expression* value = parse_expression(parser);
    if (value == NULL) {
        parser_throw_error(parser, "expression after '='");
        synchronize(parser);
        return NULL;
    }
    AssignmentStatement* assignmentStmt = (AssignmentStatement*)malloc(sizeof(AssignmentStatement));
    assignmentStmt->stmt = (Statement){.node = {.type = NODE_STMT}, .stmt_type = STMT_ASSIGNMENT};
    assignmentStmt->identifier_token = identifierToken;
    assignmentStmt->value = value;
    if (parser->currentToken.type != SEMICOLON) {
        parser_throw_error(parser, "semicolon ';' after assignment");
        synchronize(parser);
        return NULL;
    }
    advance_token_Parser(parser);
    return assignmentStmt;
}

LetStatement* parse_let_statement(Parser* parser) {   // handle stuff like let result:int = 10; let ans:bool;
    Token letToken = {.literal = "let", .type = KEYWORDS, 
                      .line_number = parser->currentToken.line_number,
                      .column_number = parser->currentToken.column_number};
    Token data_type;
    advance_token_Parser(parser);

    printf("Parsing let\n");

    if (parser->currentToken.type != IDENTIFIER) {
        parser_throw_error(parser, "identifier after 'let'");
        return NULL;
    }

    IdentifierExpression identifier = {
        .expr = { 
            .node = { .type = NODE_EXPR },
            .expr_type = EXPR_IDENTIFIER
        },
        .identifier_token = parser->currentToken
    };
    advance_token_Parser(parser);

    if(parser->currentToken.type != COLON) {
        parser_throw_error(parser, "expected data type after identifier");
        return NULL;
    }
    if(parser->currentToken.type == COLON){
        printf("Colon was encountered, %s\n", parser->currentToken.literal);
    } 
    advance_token_Parser(parser);

    if(parser->currentToken.type != Data_Type){
        parser_throw_error(parser, "invalid data type");
        return NULL;
    } 
    else {
        int dataTypeEnum = getDataTypeFromEnum(parser->currentToken.literal);
        if (dataTypeEnum == -1) {
            parser_throw_error(parser, "unrecognized data type");
            return NULL;
        }
        data_type.type = Data_Type;
        data_type.literal = getDataTypeEnumName(dataTypeEnum);
        printf("Program flow reached here, checking the data type, it was : %s\n", data_type.literal);
    }
    advance_token_Parser(parser);

    if (parser->currentToken.type == ASSIGNMENT_OPERATOR) {
        advance_token_Parser(parser);
        Expression* value = parse_expression(parser);
        if (value == NULL) {
            parser_throw_error(parser, "expression after '='");
            return NULL;
        }

        printf("Program flow reached here, checking the assignment operator\n");

        LetStatement* letStmt = (LetStatement*)malloc(sizeof(LetStatement));
        letStmt->stmt = (Statement) { .node = { .type = NODE_STMT }, .stmt_type = STMT_LET };
        letStmt->token = letToken;
        letStmt->data_type = data_type;
        letStmt->identifier = identifier;
        letStmt->value = value;
        if(letStmt == NULL) printf("Let stmt is null\n");
        else{
            printf("Let stmt was not null\n");
            IntegerLiteral* printstuff = (IntegerLiteral*)letStmt->value;
            printf("The value assigned to the let stmt identifier %s is %s\n",letStmt->identifier.identifier_token.literal, printstuff->integer_token.literal);
        }

        if(parser->currentToken.type != SEMICOLON){
            parser_throw_error(parser, "expected semicolon ';' after statement");
            return NULL;
        }
        advance_token_Parser(parser);

        printf("Program flow reached here ---- let statement parsing end\n");
        return letStmt;
    } 
    else {   // handle a simple declaration (let result:int;)
        LetStatement* letStmt = (LetStatement*)malloc(sizeof(LetStatement));
        letStmt->stmt = (Statement) { .node = { .type = NODE_STMT }, .stmt_type = STMT_LET };
        letStmt->token = letToken;
        letStmt->data_type = data_type;
        letStmt->identifier = identifier;
        letStmt->value = NULL;  // Uninitialized variable
        if(parser->currentToken.type != SEMICOLON){
            parser_throw_error(parser, "expected semicolon ';' after statement");
            return NULL;
        }
        advance_token_Parser(parser);
        return letStmt;
    }
}

ReturnStatement* parse_return_statement(Parser* parser) {
    Token returnToken = parser->currentToken;
    advance_token_Parser(parser);
    Expression* returnValue = parse_expression(parser);
    if (returnValue == NULL) {
        parser_throw_error(parser, "expression after 'return'");
        synchronize(parser);
        return NULL;
    }
    if (parser->currentToken.type != SEMICOLON) {
        parser_throw_error(parser, "semicolon ';' after 'return' statement");
        synchronize(parser);
        return NULL;
    }
    advance_token_Parser(parser);
    ReturnStatement* returnStmt = (ReturnStatement*)malloc(sizeof(ReturnStatement));
    returnStmt->stmt = (Statement){.node = {.type = NODE_STMT}, .stmt_type = STMT_RETURN};
    returnStmt->token = returnToken;
    returnStmt->returnValue = returnValue;
    return returnStmt;
}

ExpressionStatement* parse_expression_statement(Parser* parser) {
    Expression* expr = parse_expression(parser);
    if (expr == NULL) {
        parser_throw_error(parser, "valid expression");
        return NULL;
    }
    if (parser->currentToken.type != SEMICOLON) {
        parser_throw_error(parser, "semicolon ';' after expression");
        return NULL;
    }
    advance_token_Parser(parser);
    ExpressionStatement* exprStmt = (ExpressionStatement*)malloc(sizeof(ExpressionStatement));
    exprStmt->stmt.node.type = NODE_STMT;
    exprStmt->stmt.stmt_type = STMT_EXPR;
    exprStmt->expr = expr;
    return exprStmt;
}

IfStatement* parse_if_statement(Parser* parser) {
    /*
     * Expected syntax:
     *   if (condition) {
     *     // statements
     *   }
     */

    // Ensure the current token is 'if'
    Token ifToken = parser->currentToken;
    if (ifToken.type != KEYWORDS) {
        parser_throw_error(parser, "Expected 'if' keyword.");
        return NULL;
    }
    advance_token_Parser(parser);

    if (parser->currentToken.type != LEFT_PARENTHESIS) {
        parser_throw_error(parser, "Expected '(' after 'if'.");
        return NULL;
    }
    advance_token_Parser(parser);
    
    Expression* condition = parse_expression(parser);
    if (condition == NULL) {
        parser_throw_error(parser, "Invalid condition expression in 'if' statement.");
        return NULL;
    }

    if (parser->currentToken.type != RIGHT_PARENTHESIS) {
        parser_throw_error(parser, "Expected ')' after condition in 'if' statement.");
        return NULL;
    }
    advance_token_Parser(parser);

    if (parser->currentToken.type != LEFT_CURLY_BRACES) {
        parser_throw_error(parser, "Expected '{' to start the 'if' block.");
        return NULL;
    }
    advance_token_Parser(parser);

    Statement** ifBlock = NULL;
    int ifBlockCount = 0;
    while (parser->currentToken.type != RIGHT_CURLY_BRACES) {
        Statement* stmt = parse_statement(parser);
        if (stmt == NULL) {
            parser_throw_error(parser, "Invalid statement in 'if' block.");
            free(ifBlock);
            return NULL;
        }
        ifBlock = (Statement**)realloc(ifBlock, sizeof(Statement*) * (ifBlockCount + 1));
        if (ifBlock == NULL) {
            parser_throw_error(parser, "Memory allocation failed for 'if' block.");
            return NULL;
        }
        ifBlock[ifBlockCount++] = stmt;
    }

    if (parser->currentToken.type != RIGHT_CURLY_BRACES) {
        parser_throw_error(parser, "Expected '}' to close the 'if' block.");
        free(ifBlock);
        return NULL;
    }
    advance_token_Parser(parser);

    IfStatement* ifStmt = (IfStatement*)malloc(sizeof(IfStatement));
    if (ifStmt == NULL) {
        parser_throw_error(parser, "Memory allocation failed for IfStatement.");
        free(ifBlock);
        return NULL;
    }
    ifStmt->stmt = (Statement){.node = {.type = NODE_STMT}, .stmt_type = STMT_IF};
    ifStmt->condition = condition;
    ifStmt->ifBlock = ifBlock;
    ifStmt->ifBlockCount = ifBlockCount;
    return ifStmt;
}

PrintStatement* parse_print_statement(Parser* parser){
    // print(">>> " -> a -> " a");
    // parser needs to handle stuff like this
    PrintStatement* printStmt = (PrintStatement*)malloc(sizeof(PrintStatement));
    printStmt->stmt.stmt_type = STMT_PRINT;
    printStmt->left = NULL;
    printStmt->right = NULL;
    Token printToken = parser->currentToken;
    advance_token_Parser(parser);

    if (parser->currentToken.type != LEFT_PARENTHESIS) {
        parser_throw_error(parser, "Expected '(' after 'print'.");
        free(printStmt);
        return NULL;
    }
    advance_token_Parser(parser);

    printStmt->left = parse_expression(parser);
    if (printStmt->left == NULL) {
        free(printStmt);
        return NULL;
    }

    // handle multiple ->s
    PrintStatement* currentPrintStmt = printStmt;
    while (parser->currentToken.type == ARROW_OPERATOR) {
        advance_token_Parser(parser);

        PrintStatement* newPrintStmt = (PrintStatement*)malloc(sizeof(PrintStatement));
        newPrintStmt->stmt.stmt_type = STMT_PRINT;
        newPrintStmt->left = currentPrintStmt->right;
        newPrintStmt->right = parse_expression(parser);

        if (newPrintStmt->right == NULL) {
            free(newPrintStmt);
            PrintStatement* temp = printStmt;
            while (temp) {
                PrintStatement* next = (PrintStatement*)temp->right;
                free(temp);
                temp = next;
            }
            return NULL;
        }

        currentPrintStmt->right = (Expression*)newPrintStmt;
        currentPrintStmt = newPrintStmt;
    }

    if (parser->currentToken.type != RIGHT_PARENTHESIS) {
        parser_throw_error(parser, "Expected ')' after 'print' arguments.");
        free(printStmt->left);
        free(printStmt->right);
        free(printStmt);
        return NULL;
    }
    advance_token_Parser(parser);

    if (parser->currentToken.type != SEMICOLON) {
        parser_throw_error(parser, "Expected ';' after 'print' statement.");
        free(printStmt->left);
        free(printStmt->right);
        free(printStmt);
        return NULL;
    }
    advance_token_Parser(parser);
    return printStmt;
}

void synchronize(Parser* parser) {
    while (parser->currentToken.type != SEMICOLON &&
           parser->currentToken.type != ENDOFFILE) {
        advance_token_Parser(parser);
    }
    if (parser->currentToken.type == SEMICOLON)
        advance_token_Parser(parser);
}

Statement* parse_statement(Parser* parser) {
    Statement* stmt = NULL;
    switch (parser->currentToken.type) {
        case IDENTIFIER:
            printf("In parse_statement, identifier switch case\n");
            if (parser->nextToken.type == ASSIGNMENT_OPERATOR)
                stmt = (Statement*)parse_assignment_statement(parser);
            else
                stmt = (Statement*)parse_expression_statement(parser);
            break;
        case KEYWORDS:
            if (strcmp(parser->currentToken.literal, "let") == 0){
                printf("In parse_statement, let keyword's switch case\n");
                stmt = (Statement*)parse_let_statement(parser);
                if(stmt == NULL) printf("NULL let stmt returned\n");
            }
            else if (strcmp(parser->currentToken.literal, "return") == 0){
                printf("In parse_statement, return keyword's switch case\n");
                stmt = (Statement*)parse_return_statement(parser);
            }
            else if (strcmp(parser->currentToken.literal, "if") == 0){
                printf("In parse_statement, if keyword's switch case\n");
                stmt = (Statement*)parse_if_statement(parser);
            }
        case BUILT_IN_FUNCTION:
            printf("In parse_statement, built in function switch case\n");
            if (strcmp(parser->currentToken.literal, "print") == 0)
                stmt = (Statement*)parse_print_statement(parser);
            break;
        case GREATER_EQUAL:
        case GREATER_THAN:
        case LESSER_EQUAL:
        case LESSER_THAN:
        case EQUALITY_OPERATOR:
        case NOT_EQUAL_OPERATOR:
        case INCREMENT_OPERATOR:
        case DECREMENT_OPERATOR:
            stmt = (Statement*)parse_expression_statement(parser);
            break;
        default:
            parser_throw_error(parser, "valid statement");
            synchronize(parser);
            return NULL;
    }
    if (stmt == NULL)
        synchronize(parser);
    return stmt;
}

Program* parseProgram(Parser* parser) {
    Program* program = (Program*)malloc(sizeof(Program));
    program->stmts = NULL;
    program->stmtsCount = 0;

    while (parser->currentToken.type != ENDOFFILE) {
        printf("Token being parsed : %s and the next token : %s\n", parser->currentToken.literal, parser->nextToken.literal);
        Statement* stmt = parse_statement(parser);
        if (stmt != NULL) {
            if(stmt->stmt_type == STMT_LET){
                printf("Let statement returned to parse program in while loop\n");
            }
            program->stmtsCount++;
            printf("The program statement count after incrementing it : %d\n",program->stmtsCount);
            program->stmts = (Statement**)realloc(program->stmts, program->stmtsCount * sizeof(Statement*));
            program->stmts[program->stmtsCount - 1] = stmt;
        }
        else{
            synchronize(parser);
        }
    }
    printf("Program statements' count %d after parseProgram fn\n",program->stmtsCount);
    return program;
}

Parser initParser(Lexer* lexer){
    Parser parser = {
        .lexer = lexer
    };
    // read 2 next tokens to set currenttoken and nexttoken
    advance_token_Parser(&parser);
    advance_token_Parser(&parser);
    return parser;
}