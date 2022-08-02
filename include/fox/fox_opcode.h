#ifndef FOX_OPT_CODE__H
#define FOX_OPT_CODE__H
#include <stdint.h>
#include <stddef.h>
typedef double num;
#define MAX_LIMIT 65535
enum curr_type{
    TYPE_NUM,
    TYPE_LABEL,
    TYPE_ARRAY,
    TYPE_STR,
    TYPE_PTR,
    TYPE_OP,
    TYPE_REGISTER
};

struct FoxType{
    enum curr_type type;
    union{
        num number;
        struct{ 
            struct FoxType* ptr;
            size_t size;
        };
    };
};
typedef enum {
    OP_LOAD,
    OP_MOV,
    OP_ADD,
    OP_SUB,
    OP_NEG,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_BIT_XOR,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_BIT_AND,
    OP_BIT_OR,
    OP_BIT_NOT,
    OP_SHL,
    OP_SHR,
    OP_JMP,
    OP_RET,
    OP_IF,
    OP_ELSE,
    OP_IF_NEQ,
    OP_IF_EQ,
    OP_IF_LT,
    OP_IF_GT,
    OP_IF_LE,
    OP_IF_GE,
    OP_IF_AND,
    OP_IF_OR,
    OP_EQ_ARRAY,
    OP_INSERT,
    OP_APPEND,
    OP_MALLOC,
    OP_FREE,
    OP_REALLOC,
    OP_LEN,
    OP_GETPTR,
    OP_LDPTR_VAL,
    OP_ASPTR_VAL,
    OP_AT,
    OP_DL_OPEN,
    OP_DL_CLOSE,
    OP_DL_CALL,
    OP_POP,
    OP_BACK,
}opcode;
#endif