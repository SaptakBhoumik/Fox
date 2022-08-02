#include "fox_construct/construct.hpp"
#include <iostream>
namespace FoxVm{
std::vector<FoxType**> to_clean_up;

FoxType $(uint16_t number){
    FoxType reg;
    reg.type=TYPE_REGISTER;
    reg.number=number;
    return reg;
}
FoxType number(num number){
    FoxType reg;
    reg.type=TYPE_NUM;
    reg.number=number;
    return reg;
}
FoxType get_op(opcode op){
    FoxType reg;
    reg.type=TYPE_OP;
    reg.number=op;
    return reg;
}
FoxType str(std::string string){
    FoxType reg;
    reg.type=TYPE_STR;
    reg.size=string.size();
    reg.ptr=(FoxType*)malloc(sizeof(FoxType)*reg.size);
    to_clean_up.push_back(&reg.ptr);
    for(int i=0;i<reg.size;i++){
        reg.ptr[i].type=TYPE_NUM;
        reg.ptr[i].number=string[i];
    }
    return reg;
}
FoxType array(std::vector<FoxType> array){
    FoxType reg;
    reg.type=TYPE_ARRAY;
    reg.size=array.size();
    reg.ptr=(FoxType*)malloc(sizeof(FoxType)*reg.size);
    to_clean_up.push_back(&reg.ptr);
    for(int i=0;i<reg.size;i++){
        reg.ptr[i]=array[i];
    }
    return reg;
}
FoxType array(std::vector<std::string> array){
    FoxType reg;
    reg.type=TYPE_ARRAY;
    reg.size=array.size();
    reg.ptr=(FoxType*)malloc(sizeof(FoxType)*reg.size);
    to_clean_up.push_back(&reg.ptr);
    for(int i=0;i<reg.size;i++){
        reg.ptr[i]=str(array[i]);
    }
    return reg;
}
FoxType array(std::vector<num> array){
    FoxType reg;
    reg.type=TYPE_ARRAY;
    reg.size=array.size();
    reg.ptr=(FoxType*)malloc(sizeof(FoxType)*reg.size);
    to_clean_up.push_back(&reg.ptr);
    for(int i=0;i<reg.size;i++){
        reg.ptr[i]=number(array[i]);
    }
    return reg;
}
FoxType label(std::vector<FoxType> label){
    FoxType reg;
    reg.type=TYPE_LABEL;
    reg.size=label.size();
    reg.ptr=(FoxType*)malloc(sizeof(FoxType)*reg.size);
    to_clean_up.push_back(&reg.ptr);
    for(int i=0;i<reg.size;i++){
        reg.ptr[i]=label[i];
    }
    return reg;
}
FoxOperation operator +(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_ADD;
    return op;
}
FoxOperation operator -(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_SUB;
    return op;
}
FoxOperation operator *(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_MUL;
    return op;
}
FoxOperation operator /(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_DIV;
    return op;
}
FoxOperation operator %(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_MOD;
    return op;
}
FoxOperation operator ^(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_BIT_XOR;
    return op;
}
FoxOperation ptr(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_BIT_AND;
    return op;
}
FoxOperation operator |(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_BIT_OR;
    return op;
}
FoxOperation operator <<(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_SHL;
    return op;
}
FoxOperation operator >>(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_SHR;
    return op;
}
FoxOperation operator ==(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_EQ;
    return op;
}
FoxOperation operator !=(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_NEQ;
    return op;
}
FoxOperation operator >(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_GT;
    return op;
}
FoxOperation operator <(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_LT;
    return op;
}
FoxOperation operator >=(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_GE;
    return op;
}
FoxOperation operator <=(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_LE;
    return op;
}
FoxOperation operator &&(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_AND;
    return op;
}
FoxOperation operator ||(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_OR;
    return op;
}
FoxOperation operator !(FoxType left){
    FoxOperation op;
    op.left=left;
    op.op=OP_NOT;
    return op;
}
FoxOperation operator -(FoxType left){
    FoxOperation op;
    op.left=left;
    op.op=OP_NEG;
    op.is_uni=true;
    return op;
}
FoxOperation operator ~(FoxType left){
    FoxOperation op;
    op.left=left;
    op.op=OP_BIT_NOT;
    op.is_uni=true;
    return op;
}
FoxOperation operator &(FoxType left){
    FoxOperation op;
    op.left=left;
    op.op=OP_GETPTR;
    op.is_uni=true;
    return op;
}
FoxOperation operator *(FoxType left){
    FoxOperation op;
    op.left=left;
    op.op=OP_LDPTR_VAL;
    op.is_uni=true;
    return op;
}
FoxOperation array_eq(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_EQ_ARRAY;
    return op;
}
FoxOperation append(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;    
    op.op=OP_APPEND;
    return op;
}
FoxOperation insert(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_INSERT;
    return op;
}
FoxOperation at(FoxType left,FoxType right){
    FoxOperation op;
    op.left=left;
    op.right=right;
    op.op=OP_AT;
    return op;
}
FoxOperation allocate(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;
    op.op=OP_MALLOC;
    return op;
}
FoxOperation reallocate(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;
    op.op=OP_REALLOC;
    return op;
}
FoxOperation len(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;
    op.op=OP_LEN;
    return op;
}
FoxOperation dlopen(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;
    op.op=OP_DL_OPEN;
    return op;
}
FoxOperation back(FoxType left){
    FoxOperation op;
    op.left=left;
    op.is_uni=true;
    op.op=OP_BACK;
    return op;
}
FoxType* FoxConstruct::get_code(){
    m_code.push_back(get_op(OP_RET));
    code=(FoxType*)malloc(sizeof(FoxType)*m_code.size());
    for(int i=0;i<m_code.size();i++){
        code[i]=m_code[i];
    }
    return code;
}
size_t FoxConstruct::get_code_size(){
    return m_code.size();
}
void FoxConstruct::clean_up(){
    m_code.clear();
    if(code!=NULL){
        for(auto&i:to_clean_up){
            if((*i)!=NULL) {
                free(*i);
                *i=NULL;
            }
        }
        free(code);
        code=NULL;
    }
}
void FoxConstruct::load_ptr_val(FoxType reg1,FoxType reg2){
    m_code.push_back(get_op(OP_LDPTR_VAL));
    m_code.push_back(reg1);
    m_code.push_back(reg2);
}
void FoxConstruct::operation(FoxType reg,FoxOperation op){
    m_code.push_back(get_op(op.op));
    m_code.push_back(op.left);
    if(op.is_uni!=true){
        m_code.push_back(op.right);
    }
    m_code.push_back(reg);
}
void FoxConstruct::load(FoxType reg,FoxType value){
    m_code.push_back(get_op(OP_LOAD));
    m_code.push_back(value);
    m_code.push_back(reg);
}
void FoxConstruct::mov(FoxType reg1,FoxType reg2){
    m_code.push_back(get_op(OP_MOV));
    m_code.push_back(reg1);
    m_code.push_back(reg2);
}
void FoxConstruct::jmp(FoxType loc){
    m_code.push_back(get_op(OP_JMP));
    m_code.push_back(loc);
}
void FoxConstruct::ret(){
    m_code.push_back(get_op(OP_RET));
}
void FoxConstruct::jmp_if(FoxType reg,FoxType loc){
    m_code.push_back(get_op(OP_IF));
    m_code.push_back(reg);
    m_code.push_back(loc);
}
void FoxConstruct::jmp_else(FoxType loc){
    m_code.push_back(get_op(OP_ELSE));
    m_code.push_back(loc);
}
void FoxConstruct::destroy(FoxType loc){
    m_code.push_back(get_op(OP_FREE));
    m_code.push_back(loc);
}
void FoxConstruct::dlclose(FoxType reg){
    m_code.push_back(get_op(OP_DL_CLOSE));
    m_code.push_back(reg);
}
void FoxConstruct::pop(FoxType reg){
    m_code.push_back(get_op(OP_POP));
    m_code.push_back(reg);
}
void FoxConstruct::dlcall(FoxType r1,FoxType r2,FoxType r3){
    m_code.push_back(get_op(OP_DL_CALL));
    m_code.push_back(r1);
    m_code.push_back(r2);
    m_code.push_back(r3);
}
void FoxConstruct::jmp_if(FoxOperation op,FoxType reg){
    switch (op.op){
        case OP_NEQ:{
            m_code.push_back(get_op(OP_IF_NEQ));
            break;
        }
        case OP_EQ:{
            m_code.push_back(get_op(OP_IF_EQ));
            break;
        }
        case OP_LT:{
            m_code.push_back(get_op(OP_IF_LT));
            break;
        }
        case OP_GT:{
            m_code.push_back(get_op(OP_IF_GT));
            break;
        }
        case OP_LE:{
            m_code.push_back(get_op(OP_IF_LE));
            break;
        }
        case OP_GE:{
            m_code.push_back(get_op(OP_IF_GE));
            break;
        }
        case OP_AND:{
            m_code.push_back(get_op(OP_IF_AND));
            break;
        }
        case OP_OR:{
            m_code.push_back(get_op(OP_IF_OR));
            break;
        }
        default:{
            std::cout << "Opcode " <<op.op << "cant be in a if branch"<< std::endl;
        }
    }
    m_code.push_back(op.left);
    m_code.push_back(op.right);
    m_code.push_back(reg);
}
}