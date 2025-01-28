#ifndef _PARSER_H_
#define _PARSER_H_

    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "lexer.h"
    #include "ast.h"

    enum Precedence{
        LOWEST = 1,
        ASSIGN,
        EQUALS,
        LESSGREATER,
        SUM,
        PRODUCT,
        CALL,           // fn()
        INDEX,          // array[i]
    };

    typedef struct Parser {
        Lexer* lexer;
        Token currentToken;
        Token nextToken;
    } Parser;

    Parser initParser(Lexer*);
    void parser_throw_error(Parser*, const char*);
    void advance_token_Parser(Parser*);
    Program* parseProgram(Parser*);
    Token currentToken_Parser(Parser*);
    Token peekToken_Parser(Parser*);

    void synchronize(Parser*);

    Statement* parse_statement(Parser*);
    AssignmentStatement* parse_assignment_statement(Parser*);
    LetStatement* parse_let_statement(Parser*);
    ReturnStatement* parse_return_statement(Parser*);
    PrintStatement* parse_print_statement(Parser*);

    int is_infix_operator(Token);
    int get_precedence(Token);
    Expression* parse_expression(Parser*);
    Expression* parse_prefix_expression(Parser*);
    Expression* parse_infix_expression(Parser*, Expression*, int);
    Expression* parse_identifier_expression(Parser*);
    Expression* parse_integer_literal_expression(Parser*);
    Expression* parse_float_literal_expression(Parser*);
    Expression* parse_string_literal_expression(Parser*);
    Expression* parse_grouped_expression(Parser*);

    ExpressionStatement* parse_expression_statement(Parser*);

#endif