#ifndef CONSTRUCT__FOX__H
#define CONSTRUCT__FOX__H
#include "fox/fox.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#define $ _reg
namespace FoxVm{  
struct FoxOperation;
FoxType $(uint16_t);
FoxType str(std::string);
FoxType number(num);
FoxType array(std::vector<FoxType>);
FoxType array(std::vector<std::string>);
FoxType array(std::vector<num>);
FoxType label(std::vector<FoxType>);
FoxOperation operator +(FoxType, FoxType);//OP_ADD
FoxOperation operator -(FoxType);//OP_NEG
FoxOperation operator -(FoxType, FoxType);//OP_SUB
FoxOperation operator *(FoxType, FoxType);//OP_MUL
FoxOperation operator /(FoxType, FoxType);//OP_DIV
FoxOperation operator %(FoxType, FoxType);//OP_MOD
FoxOperation operator ^(FoxType, FoxType);//OP_BIT_XOR
FoxOperation operator ==(FoxType, FoxType);//OP_EQ
FoxOperation operator !=(FoxType, FoxType);//OP_NEQ
FoxOperation operator <(FoxType, FoxType);//OP_LT
FoxOperation operator >(FoxType, FoxType);//OP_GT
FoxOperation operator <=(FoxType, FoxType);//OP_LE
FoxOperation operator >=(FoxType, FoxType);//OP_GE
FoxOperation operator &&(FoxType, FoxType);//OP_AND
FoxOperation operator ||(FoxType, FoxType);//OP_OR
FoxOperation operator !(FoxType);//OP_NOT
FoxOperation operator &(FoxType, FoxType);//OP_BIT_AND
FoxOperation operator |(FoxType, FoxType);//OP_BIT_OR
FoxOperation operator ~(FoxType);//OP_BIT_NOT
FoxOperation operator <<(FoxType, FoxType);//OP_SHL
FoxOperation operator >>(FoxType, FoxType);//OP_SHR
FoxOperation ptr(FoxType);
FoxOperation operator *(FoxType);
FoxOperation array_eq(FoxType);
FoxOperation append(FoxType);
FoxOperation insert(FoxType,FoxType);
FoxOperation at(FoxType,FoxType);
FoxOperation allocate(FoxType);
FoxOperation reallocate(FoxType);
FoxOperation len(FoxType);
FoxOperation dlopen(FoxType);
FoxOperation back(FoxType);
class FoxConstruct{
    FoxType* code=nullptr;
    std::vector<FoxType> m_code;
    public:
    FoxConstruct()=default;
    FoxType* get_code();
    size_t get_code_size();
    void clean_up();
    void operation(FoxType,FoxOperation);
    void load_ptr_val(FoxType,FoxType);
    void load(FoxType,FoxType);
    void mov(FoxType,FoxType);
    void jmp(FoxType);
    void ret();
    void jmp_if(FoxType,FoxType);
    void jmp_if(FoxOperation,FoxType);
    void jmp_else(FoxType);
    void destroy(FoxType);
    void dlclose(FoxType);
    void pop(FoxType);
    void dlcall(FoxType,FoxType,FoxType);
};
struct FoxOperation{
    FoxType left;
    FoxType right;
    opcode op;
    bool is_uni=false;
};
}
#endif