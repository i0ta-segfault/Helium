#ifndef _TOKEN_H_
    #define _TOKEN_H_
    #define INITIAL_TOKEN_CAPACITY 10
    #include "types.h"

    char* getKeywordEnumName(Keywords);
    char* getDataTypeEnumName(DataType);
    int getDataTypeFromEnum(char*);
    char* getTokenTypeName(Token);
    int isKeyword(char*);
    int isDataType(char*);
    int isBuiltInFunction(char*);
    int isDigit(char);

#endif