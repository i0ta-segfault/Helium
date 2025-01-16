#include "helium.h"

void print_expression(Expression* expr) {
    switch (expr->expr_type) {
        case EXPR_IDENTIFIER: {
            IdentifierExpression* idExpr = (IdentifierExpression*)expr;
            printf("Identifier: %s\n", idExpr->identifier_token.literal);
            break;
        }
        case EXPR_INTEGER: {
            IntegerLiteral* intLiteral = (IntegerLiteral*)expr;
            printf("Integer Literal: %s\n", intLiteral->integer_token.literal);
            break;
        }
        case EXPR_INFIX: {
            InfixExpression* infixExpr = (InfixExpression*)expr;
            printf("Infix Expression: (");
            print_expression(infixExpr->left);
            printf(" %s ", infixExpr->operator_token.literal);
            print_expression(infixExpr->right);
            printf(")");
            break;
        }
        case EXPR_PREFIX: {
            PrefixExpression* prefixExpr = (PrefixExpression*)expr;
            printf("Prefix Expression: ");
            printf("%s(\n\t", prefixExpr->prefix_operator_token.literal);
            print_expression(prefixExpr->right);
            printf(")");
            break;
        }
        case EXPR_POSTFIX: {
            PostfixExpression* postfixExpr = (PostfixExpression*)expr;
            printf("Postfix Expression: ");
            printf("( ");
            print_expression(postfixExpr->left);
            printf(")%s",postfixExpr->postfix_operator_token.literal);
            break;
        }
        default:
            printf("Unknown Expression Type\n");
            break;
    }
}

void print_statement(Statement* stmt) {
    switch (stmt->stmt_type) {
        case STMT_LET: {
            LetStatement* letStmt = (LetStatement*)stmt;
            printf("Let Statement: %s\n", letStmt->identifier.identifier_token.literal);
            if (letStmt->value != NULL) {
                printf("  Value: ");
                print_expression(letStmt->value);
            }
            else {
                printf("  Uninitialized\n");
            }
            break;
        }
        case STMT_ASSIGNMENT: {
            AssignmentStatement* assignStmt = (AssignmentStatement*)stmt;
            printf("Assignment Statement: %s = ", assignStmt->identifier_token.literal);
            print_expression(assignStmt->value);
            break;
        }
        case STMT_RETURN: {
            ReturnStatement* returnStmt = (ReturnStatement*)stmt;
            printf("Return Statement: ");
            print_expression(returnStmt->returnValue);
            break;
        }
        case STMT_EXPR: {
            ExpressionStatement* expressionStmt = (ExpressionStatement*)stmt;
            printf("Expression Statement: ");
            print_expression(expressionStmt->expr);
            printf("\n");
            break;
        }
        case STMT_IF: {
            IfStatement* ifStmt = (IfStatement*)stmt;
            printf("If Statement:\n");
            printf("  Condition: ");
            print_expression(ifStmt->condition); // the expression after if keyword
            printf("\n  If Block:\n");
            for (int i = 0; i < ifStmt->ifBlockCount; i++) {
                printf("    Statement %d:\n", i + 1);
                print_statement(ifStmt->ifBlock[i]);
                printf("\n");
            }
            break;
        }
        default:
            printf("Unknown Statement Type\n");
            break;
    }
}

int main(int argc, char** argv) {
    
    if (argc != 2) {
        printf("Usage: ./main filename.he\n");
        exit(EXIT_FAILURE);
    }

    char* filePath = argv[1];
    FILE* fptr = fopen(filePath, "r");
    if (!fptr) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(fptr, 0, SEEK_END);
    long int fileSize = ftell(fptr);
    rewind(fptr);

    char* sourcecode = (char*)malloc(fileSize + 1);
    if (!sourcecode) {
        perror("Error allocating memory for source code");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }

    fread(sourcecode, 1, fileSize, fptr);
    sourcecode[fileSize] = '\0';

    fclose(fptr);

    Token* tokensArray = NULL;
    int tokensCount = 0;
    getAllTokens(sourcecode, &tokensArray, &tokensCount);

    for (int i = 0; i < 50; i++)
        printf("-");
    printf("\n");
    for (int i = 0; i < tokensCount; i++) {
        printf("Token Type: %-30s | Literal: %-20s | Line : %d   Column : %d\n", getTokenTypeName(tokensArray[i]), tokensArray[i].literal, (tokensArray[i].line_number), (tokensArray[i].column_number));
    }
    for (int i = 0; i < 50; i++)
        printf("-");
    printf("\n");

    Lexer lexer = initLexer(sourcecode);
    Parser parser = initParser(&lexer);
    Program* program = parseProgram(&parser);
    printf("\nParser output:\n");
    if(program == NULL){
        printf("Parse program function returned nothing - NULL\n");
    }
    else{
        for (int i = 0; i < program->stmtsCount; i++) {
            printf("Statement %d:\n", i + 1);
            print_statement(program->stmts[i]);
            printf("\n");
        }
    }

    return 0;
}
