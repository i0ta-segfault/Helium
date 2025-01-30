#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "parser.h"
    #include "hashmap.h"

    char* evaluate_escape_sequences(char*);
    float evaluate_infix_expression(InfixExpression*, HashMap*);
    void evaluate(Program*);

#endif