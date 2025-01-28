#include "evaluator.h"

void evaluate(Program* program) {
    if(program == NULL){
        printf("Program ast was NULL");
        return;
    }
    HashMap* map = initHashMap(5);
    for (int i = 0; i < program->stmtsCount; i++) {
        switch (program->stmts[i]->stmt_type) {
            case STMT_LET: {
                LetStatement* stmt = program->stmts[i];
                if (strcmp(stmt->data_type.literal, "int") == 0) { 
                    int* initial_value = malloc(sizeof(int));
                    *initial_value = 0;  // Default to 0 for an int
                    insertToHashMap(map, stmt->identifier.identifier_token.literal, "int", initial_value);
                    printf("Inserted: %s (type: int, value: %d)\n", stmt->identifier.identifier_token.literal, *initial_value);
                } else if (strcmp(stmt->data_type.literal, "float") == 0) {
                    float* initial_value = malloc(sizeof(float));
                    *initial_value = 0.0f;  // Default to 0.0f for a float
                    insertToHashMap(map, stmt->identifier.identifier_token.literal, "float", initial_value);
                } else if (strcmp(stmt->data_type.literal, "string") == 0) {
                    insertToHashMap(map, stmt->identifier.identifier_token.literal, "string", NULL);  // Initialize to NULL
                } else {
                    printf("Error: Unsupported data type '%s' at [%d:%d]\n", stmt->data_type.literal, stmt->identifier.identifier_token.line_number, stmt->identifier.identifier_token.column_number);
                    return;
                }
                break;
            }
            case STMT_ASSIGNMENT: {
                AssignmentStatement* stmt = program->stmts[i];
                EntryMetaData metadata = getFromHashMap(map, stmt->identifier_token.literal);
                if (metadata.data_type == NULL || metadata.value == NULL) {
                    printf("Error: Variable %s not found or uninitialized\n", stmt->identifier_token.literal);
                }
                if (metadata.data_type != NULL && metadata.value != NULL) { 
                    insertToHashMap(map, stmt->identifier_token.literal, metadata.data_type, stmt->value);
                } else {
                    printf("Error at [%d:%d] : No variable with %s name found or it is uninitialized\n", 
                        stmt->identifier_token.line_number, stmt->identifier_token.column_number, 
                        stmt->identifier_token.literal);
                    return;
                }
                break;
            }
            case STMT_PRINT: {
                PrintStatement* stmt = program->stmts[i];
                for (int j = 0; j < stmt->exprCount; j++) {
                    Expression* curr_expr = stmt->expressions[j];
                    switch (curr_expr->expr_type) {
                        case EXPR_STRING: { 
                            StringLiteral* str = (StringLiteral*)curr_expr;
                            printf("%s", str->string_token.literal);
                            break;
                        }
                        case EXPR_IDENTIFIER: {
                            IdentifierExpression* idnexpr = (IdentifierExpression*)curr_expr;
                            EntryMetaData metadata = getFromHashMap(map, idnexpr->identifier_token.literal);
                            if (metadata.data_type != 0 && metadata.value != NULL) {
                                if (strcmp(metadata.data_type, "int") == 0)
                                    printf("%d", *((int*)metadata.value)); // Evaluate integer value
                                else if (strcmp(metadata.data_type, "float") == 0)
                                    printf("%f", *((float*)metadata.value)); // Evaluate float value
                            }
                            break;
                        }
                        case EXPR_INTEGER: {
                            IntegerLiteral* intExpr = (IntegerLiteral*)curr_expr;
                            printf("%d", atoi(intExpr->integer_token.literal)); // Print integer value
                            break;
                        }
                        case EXPR_FLOAT: {
                            FloatingLiteral* floatExpr = (FloatingLiteral*)curr_expr;
                            printf("%f", atof(floatExpr->float_token.literal)); // Print float value
                            break;
                        }
                        case EXPR_INFIX: {
                            InfixExpression* infixExpr = (InfixExpression*)curr_expr;
                            // Evaluate and print the result of the infix expression here (not implemented)
                            break;
                        }
                        default:
                            printf("Unsupported expression type.\n");
                            break;
                    }
                    if (j < stmt->exprCount - 1) {
                        printf(", ");
                    }
                }
                printf("\n");
                break;
            }
        }
    }
    freeHashMap(map);
}