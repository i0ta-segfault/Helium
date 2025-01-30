#include "evaluator.h"

char* evaluate_escape_sequences(char* str) {
    int length = strlen(str);
    char* newStr = malloc(length + 1);
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (str[i] == '\\' && (str[i + 1] == 'n' || str[i + 1] == 't')) {
            if (str[i + 1] == 'n') {
                newStr[j++] = '\n';
            } 
            else if (str[i + 1] == 't') {
                newStr[j++] = '\t';
            }
            i++;
        } 
        else {
            newStr[j++] = str[i];
        }
    }

    newStr[j] = '\0';
    return newStr;
}

float evaluate_infix_expression(InfixExpression* infixExpr, HashMap* map) {
    float left_operand_result = 0.0f, right_operand_result = 0.0f, total_result = 0.0f;
    Expression* left_operand = infixExpr->left;
    Expression* right_operand = infixExpr->right;
    if (left_operand == NULL) {
        printf("Error: Left operand is NULL.\n");
        return 0;
    }
    else if (right_operand == NULL) {
        printf("Error: Right operand is NULL.\n");
        return 0;
    }

    // Evaluate the left operand
    switch(left_operand->expr_type){
        case EXPR_INTEGER:
            printf("Left operand result : %d\n", atoi(((IntegerLiteral*)left_operand)->integer_token.literal));
            left_operand_result = atoi(((IntegerLiteral*)left_operand)->integer_token.literal);
            break;
        case EXPR_FLOAT:
            printf("Left operand result : %f\n", atof(((FloatingLiteral*)left_operand)->float_token.literal));
            left_operand_result = atof(((FloatingLiteral*)left_operand)->float_token.literal);
            break;
        case EXPR_IDENTIFIER: {
            IdentifierExpression* idExpr = (IdentifierExpression*)left_operand;
            EntryMetaData metadata = getFromHashMap(map, idExpr->identifier_token.literal);
            if (metadata.data_type != NULL && metadata.value != NULL) {
                if (strcmp(metadata.data_type, "int") == 0) {
                    left_operand_result = *((int*)metadata.value);
                } 
                else if (strcmp(metadata.data_type, "float") == 0) {
                    left_operand_result = *((float*)metadata.value);
                }
            }
            break;
        }
        case EXPR_INFIX: {
            InfixExpression* left_operand_infix = (InfixExpression*)left_operand;
            left_operand_result = evaluate_infix_expression(left_operand_infix, map);
            break;
        }
        // case EXPR_STRING: {
        //     StringLiteral* str = (StringLiteral*)left_operand;
        //     char* stringValue = str->string_token.literal;
        //     stringValue = evaluate_escape_sequences(stringValue);
        //     printf("%s", stringValue);
        //     break;
        // }
        default:
            printf("Unsupported left operand in infix expression.\n");
            break;
    }

    // Evaluate the right operand
    switch(right_operand->expr_type){
        case EXPR_INTEGER:
            printf("Right operand result : %d\n", atoi(((IntegerLiteral*)right_operand)->integer_token.literal));
            right_operand_result = atoi(((IntegerLiteral*)right_operand)->integer_token.literal);
            break;
        case EXPR_FLOAT:
            printf("Right operand result : %f\n", atof(((FloatingLiteral*)right_operand)->float_token.literal));
            right_operand_result = atof(((FloatingLiteral*)right_operand)->float_token.literal);
            break;
        case EXPR_IDENTIFIER: {
            IdentifierExpression* idExpr = (IdentifierExpression*)right_operand;
            EntryMetaData metadata = getFromHashMap(map, idExpr->identifier_token.literal);
            if (metadata.data_type != NULL && metadata.value != NULL) {
                if (strcmp(metadata.data_type, "int") == 0) {
                    right_operand_result = *((int*)metadata.value);
                    printf("Right operand result for int identifier : %d\n", *((int*)metadata.value));
                } 
                else if (strcmp(metadata.data_type, "float") == 0) {
                    right_operand_result = *((float*)metadata.value);
                    printf("Right operand result for float identifier: %f\n", *((float*)metadata.value));
                }
            }
            else if (metadata.data_type == NULL || metadata.value == NULL) {
                printf("\nError: Variable %s not found\n", idExpr->identifier_token.literal);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case EXPR_INFIX: {
            InfixExpression* right_operand_infix = (InfixExpression*)right_operand;
            right_operand_result = evaluate_infix_expression(right_operand_infix, map);
            break;
        }
        // case EXPR_STRING: {
        //     StringLiteral* str = (StringLiteral*)right_operand;
        //     char* stringValue = str->string_token.literal;
        //     stringValue = evaluate_escape_sequences(stringValue);
        //     printf("%s", stringValue);
        //     break;
        // }
        default:
            printf("Unsupported right operand in infix expression.\n");
            break;
    }

    // Perform the operation based on the operator
    switch(infixExpr->operator_token.type){
        case PLUS_OPERATOR:
            total_result = left_operand_result + right_operand_result;
            break;
        case MINUS_OPERATOR:
            total_result = left_operand_result - right_operand_result;
            break; 
        case ASTERISK:
            total_result = left_operand_result * right_operand_result;
            break; 
        case FORWARD_SLASH:
            if(right_operand_result != 0)
                total_result = left_operand_result / right_operand_result;
            else{
                printf("Division by 0 not permitted\n");
            }
            break; 
        case MODULUS_OPERATOR:
            if(right_operand_result != 0)
                total_result = (int)left_operand_result % (int)right_operand_result;
            else{
                printf("Division by 0 not permitted\n");
            }
            break; 
        case GREATER_EQUAL:
            total_result = left_operand_result >= right_operand_result;
            break; 
        case LESSER_EQUAL:
            total_result = left_operand_result <= right_operand_result;
            break;
        case GREATER_THAN:
            total_result = left_operand_result > right_operand_result;
            break; 
        case LESSER_THAN:
            total_result = left_operand_result < right_operand_result;
            break;
        case EQUALITY_OPERATOR:
            total_result = left_operand_result == right_operand_result;
            break;
        case NOT_EQUAL_OPERATOR:
            total_result = left_operand_result != right_operand_result;
            break;
        case ARROW_OPERATOR:
            break;
        default:
            printf("Unsupported infix operator.\n");
            break;
    }

    return total_result;
}

void evaluate(Program* program) {
    if(program == NULL){
        printf("Program ast was NULL\n");
        return;
    }
    HashMap* map = initHashMap(5);
    // printf("Program stmts count : %d\n",program->stmtsCount);
    for (int i = 0; i < program->stmtsCount; i++) {
        switch (program->stmts[i]->stmt_type) {
            case STMT_LET: {
                LetStatement* stmt = (LetStatement*)program->stmts[i];
                if (stmt->value != NULL && stmt->value->expr_type == EXPR_INFIX) { // let x : int = 10 + 5;
                    InfixExpression* infixExpr = (InfixExpression*)stmt->value;
                    double result = evaluate_infix_expression(infixExpr, map);
                    // Initialize based on type
                    if (strcmp(stmt->data_type.literal, "int") == 0) {
                        int* initial_value = malloc(sizeof(int));
                        *initial_value = (int)result;
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "int", initial_value);
                    } 
                    else if (strcmp(stmt->data_type.literal, "float") == 0) {
                        float* initial_value = malloc(sizeof(float));
                        *initial_value = (float)result;
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "float", initial_value);
                    } 
                    else {
                        printf("Error: Unsupported data type '%s' at [%d:%d]\n", stmt->data_type.literal, 
                                stmt->identifier.identifier_token.line_number, 
                                stmt->identifier.identifier_token.column_number);
                        return;
                    }
                }
                else if (stmt->value != NULL && stmt->value->expr_type != EXPR_INFIX) { // let x : int = 10;
                    // Handle declaration stuff like let x:int = 10 or let y:float = 10.0
                    if (strcmp(stmt->data_type.literal, "int") == 0) {
                        IntegerLiteral* value = (IntegerLiteral*)stmt->value;
                        int* assigned_value = malloc(sizeof(int));
                        *assigned_value = atoi(value->integer_token.literal);
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "int", assigned_value);
                    }
                    else if (strcmp(stmt->data_type.literal, "float") == 0) {
                        FloatingLiteral* value = (FloatingLiteral*)stmt->value;
                        float* assigned_value = malloc(sizeof(float));
                        *assigned_value = atof(value->float_token.literal);
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "float", assigned_value);
                    } 
                    else if (strcmp(stmt->data_type.literal, "string") == 0) {
                        StringLiteral* value = (StringLiteral*)stmt->value;
                        char* assigned_value = strdup(value->string_token.literal);
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "string", assigned_value);
                    } 
                    else {
                        printf("Error: Unsupported data type '%s' at [%d:%d]\n", stmt->data_type.literal, 
                                stmt->identifier.identifier_token.line_number, 
                                stmt->identifier.identifier_token.column_number);
                        return;
                    }
                }
                else if (stmt->value == NULL) {  // empty initializations like let x : int;
                    if (strcmp(stmt->data_type.literal, "int") == 0) {
                        int* assigned_value = malloc(sizeof(int)); // garbage value, no default initialisation
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "int", assigned_value);
                    }
                    else if (strcmp(stmt->data_type.literal, "float") == 0) {
                        float* assigned_value = malloc(sizeof(float));
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "float", assigned_value);
                    } 
                    else if (strcmp(stmt->data_type.literal, "string") == 0) {
                        char* assigned_value = malloc(sizeof(char));
                        insertToHashMap(map, stmt->identifier.identifier_token.literal, "string", assigned_value);
                    } 
                    else {
                        printf("Error: Unsupported data type '%s' at [%d:%d]\n", stmt->data_type.literal, 
                                stmt->identifier.identifier_token.line_number, 
                                stmt->identifier.identifier_token.column_number);
                        return;
                    }
                }
                break;
            }
            case STMT_ASSIGNMENT: {
                // printf("Assignment statement encountered\n");
                AssignmentStatement* stmt = (AssignmentStatement*)program->stmts[i];
                EntryMetaData metadata = getFromHashMap(map, stmt->identifier_token.literal);
                if (metadata.data_type == NULL || metadata.value == NULL) {  // variable has not been initialized yet
                    printf("Error at [%d:%d]: Variable %s not found or uninitialized\n", 
                        stmt->identifier_token.line_number, 
                        stmt->identifier_token.column_number, 
                        stmt->identifier_token.literal);
                    return;
                } 
                else if (metadata.data_type != NULL && metadata.value != NULL) { // variable was found

                    if (stmt->value->expr_type == EXPR_INFIX) {  // stuff like blue_bear = a + 9;
                        printf("Assignment statement has infix expression\n");
                        InfixExpression* infixExpr = (InfixExpression*)stmt->value;
                        double result = evaluate_infix_expression(infixExpr, map);
                        printf("The result of the infix expr : %d\n", result);
                        if (strcmp(metadata.data_type, "int") == 0) {
                            int* assigned_value = (int*)metadata.value;
                            *assigned_value = (int)result;
                        }
                        else if (strcmp(metadata.data_type, "float") == 0) {
                            float* assigned_value = (float*)metadata.value;
                            *assigned_value = (float)result;
                        }
                    } 
                    else {  // simple declaration like blue_bear = 9;
                        if (strcmp(metadata.data_type, "int") == 0) {
                            IntegerLiteral* value = (IntegerLiteral*)stmt->value;
                            int* assigned_value = malloc(sizeof(int));
                            *assigned_value = atoi(value->integer_token.literal);
                            insertToHashMap(map, stmt->identifier_token.literal, metadata.data_type, assigned_value);
                        }
                        else if (strcmp(metadata.data_type, "float") == 0) {
                            FloatingLiteral* value = (FloatingLiteral*)stmt->value;
                            float* assigned_value = malloc(sizeof(float));
                            *assigned_value = atof(value->float_token.literal);
                            insertToHashMap(map, stmt->identifier_token.literal, metadata.data_type, assigned_value);
                        }
                        else if (strcmp(metadata.data_type, "string") == 0) {
                            StringLiteral* value = (StringLiteral*)stmt->value;
                            char* assigned_value = strdup(value->string_token.literal);
                            insertToHashMap(map, stmt->identifier_token.literal, metadata.data_type, assigned_value);
                        }
                    }
                } 
                else {
                    printf("Error at [%d:%d] : No variable with %s name found or it is uninitialized\n", 
                        stmt->identifier_token.line_number, 
                        stmt->identifier_token.column_number, 
                        stmt->identifier_token.literal);
                    return;
                }
                break;
            }
            case STMT_PRINT: {
                PrintStatement* stmt = (PrintStatement*)program->stmts[i];
                PrintStatement* currentStmt = stmt;
                while (currentStmt != NULL) {
                    if (currentStmt->left != NULL) {
                        Expression* left_side = currentStmt->left;
                        switch (left_side->expr_type) {
                            case EXPR_STRING: {
                                StringLiteral* str = (StringLiteral*)left_side;
                                char* stringValue = str->string_token.literal;
                                stringValue = evaluate_escape_sequences(stringValue);
                                printf("%s", stringValue);
                                break;
                            }
                            case EXPR_IDENTIFIER: {
                                IdentifierExpression* idnexpr = (IdentifierExpression*)left_side;
                                EntryMetaData metadata = getFromHashMap(map, idnexpr->identifier_token.literal);
                                if (metadata.data_type != NULL && metadata.value != NULL) {
                                    if (strcmp(metadata.data_type, "int") == 0)
                                        printf("%d", *((int*)metadata.value));
                                    else if (strcmp(metadata.data_type, "float") == 0)
                                        printf("%f", *((float*)metadata.value));
                                    else if (strcmp(metadata.data_type, "string") == 0)
                                        printf("%s", *((char*)metadata.value));
                                }
                                break;
                            }
                            case EXPR_INTEGER: {
                                IntegerLiteral* intExpr = (IntegerLiteral*)left_side;
                                printf("%d", atoi(intExpr->integer_token.literal));
                                break;
                            }
                            case EXPR_FLOAT: {
                                FloatingLiteral* floatExpr = (FloatingLiteral*)left_side;
                                printf("%f", atof(floatExpr->float_token.literal));
                                break;
                            }
                            case EXPR_INFIX: {
                                InfixExpression* infixExpr = (InfixExpression*)left_side;
                                double result = evaluate_infix_expression(infixExpr, map);
                                printf("%f", result);
                                break;
                            }
                            default:
                                printf("Unsupported expression type.\n");
                                break;
                        }
                    }
                    currentStmt = (PrintStatement*)currentStmt->right;
                }
                break;
            }
            default: {
                printf("Unknown statement encountered\n"); break;
            }
        }
    }
    freeHashMap(map);
}