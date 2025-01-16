#ifndef _AST_H_
#define _AST_H_

    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "lexer.h"

    typedef enum {
        NODE_STMT, NODE_EXPR
    } NodeType;

    typedef enum {
        STMT_LET, STMT_RETURN, STMT_ASSIGNMENT, STMT_EXPR, STMT_IF, STMT_ELSE
    } StatementType;

    typedef enum {
        EXPR_IDENTIFIER, EXPR_INTEGER, EXPR_PREFIX, EXPR_INFIX, EXPR_POSTFIX
    } ExpressionType;

    typedef struct Node {
        NodeType type; // could be node_stmt or node_expr
    } Node;

    typedef struct Expression {
        Node node;
        ExpressionType expr_type;
    } Expression;

    typedef struct Statement {
        Node node;
        StatementType stmt_type;
    } Statement;

    typedef struct IdentifierExpression {
        Expression expr;
        Token identifier_token;   // will hold the identifier
    } IdentifierExpression;

    typedef struct PrefixExpression {
        Expression expr;
        Token prefix_operator_token;
        struct Expression* right;
    } PrefixExpression;
    
    typedef struct PostfixExpression {
        Expression expr;
        Token postfix_operator_token;
        struct Expression* left;
    } PostfixExpression;

    typedef struct InfixExpression {
        Expression expr;
        struct Expression* left;
        Token operator_token;
        struct Expression* right;
    } InfixExpression;

    typedef struct IntegerLiteral {
        Expression expr;
        Token integer_token;
    } IntegerLiteral;

    typedef struct LetStatement {
        Statement stmt;
        Token token;  // will hold the let keyword
        IdentifierExpression identifier;
        struct Expression* value;  // can be null if variable is just declared and not initialized
    } LetStatement;

    typedef struct AssignmentStatement {
        Statement stmt;
        Token identifier_token;
        struct Expression* value; // holds the expression being assigned
    } AssignmentStatement;

    typedef struct ReturnStatement {
        Statement stmt;
        Token token;  // will hold the return keyword
        struct Expression* returnValue;
    } ReturnStatement;

    typedef struct ExpressionStatement {
        Statement stmt;
        Expression* expr;
    } ExpressionStatement;

    typedef struct IfStatement {
        Statement stmt;
        Expression* condition;
        Statement** ifBlock;
        int ifBlockCount;
    } IfStatement;

    typedef struct ElseStatement {
        Statement stmt;
        Statement** elseBlock;
        int elseBlockCount;
    } ElseStatement;

    typedef struct Program {
        Statement** stmts;
        int stmtsCount;
    } Program;

#endif