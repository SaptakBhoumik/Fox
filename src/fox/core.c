#include "fox/fox_opcode.h"
#include <stdio.h>
#include <stdlib.h>
#if defined _WIN32 || defined __CYGWIN__
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#define bin_op(op_arg)  {\
                    code++;\
                    num r1=m_memory[(long long)code->number].number;\
                    code++;\
                    num r2=m_memory[(long long)code->number].number;\
                    code++;\
                    m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=r1 op_arg r2};\
                    }

#define if_bin_op(curr_operator)code++;\
        FoxType r1=m_memory[(long long)code->number];\
        code++;\
        FoxType r2=m_memory[(long long)code->number];\
        code++;\
        if((int)(r1.number curr_operator r2.number)){\
            FoxType func = get_function(code->number);\
            __execute(func.ptr,m_memory);\
            if((opcode)(*(code+1)).number==OP_ELSE){\
                code++;\
                code++;\
            }\
        }\
        else{\
            if((opcode)(*(code+1)).number==OP_ELSE){\
                code++;\
                code++;\
                FoxType func = get_function(code->number);\
                __execute(func.ptr,m_memory);\
            }\
        }

#define unary_op(op_arg) code++;\
                        num r1=m_memory[(long long)code->number].number;\
                        code++;\
                        m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=op_arg r1};

#define unary_int_op(op_arg) code++;\
                        long long r1=(long long)m_memory[(long long)code->number].number;\
                        code++;\
                        m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=op_arg r1};


#define bin_int_op(op_arg)  {\
                    code++;\
                    long long r1=(long long)m_memory[(long long)code->number].number;\
                    code++;\
                    long long r2=(long long)m_memory[(long long)code->number].number;\
                    code++;\
                    m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=r1 op_arg r2};\
                    }

#define advance()  code++;
#define DISPATCH()  {\
                    code++;\
                    goto *dispatch[(opcode)code->number];\
                    }
#define insert(op) [OP_##op] = &&_OP_##op


typedef struct FoxType FoxType;
extern FoxType get_function(num name);
typedef void (*ext_func)(FoxType ,FoxType*);
static inline int equality_array(FoxType* array1,num size1,FoxType* array2,num size){
    int res=0;
    for(long long i=0;i<size1;i++){
        if(array2[i].type==TYPE_ARRAY||array2[i].type==TYPE_STR){
           res=equality_array(array1[i].ptr,array1[i].size,array2[i].ptr,array2[i].size);
           if(!res){
                res = 0;
                break;
           } 
        }
        else if(array2[i].type==TYPE_PTR){
            if(array1[i].ptr!=array2[i].ptr){
                res=0;
                break;
            }
            else{
                res=1;
            }
        }
        else if(array1[i].number!=array2[i].number){
            res=0;
            break;
        }
        else{
            res=1;
        }
    }
    return res;
}
static inline char* to_str(FoxType item){
    char* res=malloc(sizeof(char)*(item.size+1));
    for(long long i=0;i<item.size;i++){
        if(item.ptr[i].type!=TYPE_NUM){break;}
        res[i]=item.ptr[i].number;
    }
    res[(long long)item.size]='\0';
    return res;
}


void __execute(FoxType* code,FoxType* m_memory){
    void* dispatch[]={
        insert(LOAD),
        insert(MOV),
        insert(ADD),
        insert(SUB),
        insert(NEG),
        insert(MUL),
        insert(DIV),
        insert(MOD),
        insert(BIT_XOR),
        insert(EQ),
        insert(NEQ),
        insert(LT),
        insert(GT),
        insert(LE),
        insert(GE),
        insert(AND),
        insert(OR),
        insert(NOT),
        insert(BIT_AND),
        insert(BIT_OR),
        insert(BIT_NOT),
        insert(SHL),
        insert(SHR),
        insert(JMP),
        insert(RET),
        insert(IF),
        insert(ELSE),
        insert(IF_NEQ),
        insert(IF_EQ),
        insert(IF_LT),
        insert(IF_GT),
        insert(IF_LE),
        insert(IF_GE),
        insert(IF_AND),
        insert(IF_OR),
        insert(EQ_ARRAY),
        insert(INSERT),
        insert(APPEND),
        insert(MALLOC),
        insert(FREE),
        insert(REALLOC),
        insert(LEN),
        insert(GETPTR),
        insert(LDPTR_VAL),
        insert(ASPTR_VAL),
        insert(AT),
        insert(DL_OPEN),
        insert(DL_CLOSE),
        insert(DL_CALL),
        insert(POP),
        insert(BACK),
    };
    goto *dispatch[(opcode)code->number];
    _OP_BACK:{
        //back <register1> <register2> ;$register2 = $register1.back()
        advance();
        FoxType r1=m_memory[(long long)code->number];
        advance();
        m_memory[(long long)code->number]=r1.ptr[(long long)r1.size-1];
        DISPATCH();
    }

    _OP_AT:{
        //at <register1> <register2> <destination>;$destination = $register2.at($register1)
        advance();
        FoxType r1=m_memory[(long long)code->number];
        advance();
        FoxType r2=m_memory[(long long)code->number];
        advance();
        m_memory[(long long)code->number]=r2.ptr[(long long)r1.number];
        DISPATCH();
    }
    _OP_ASPTR_VAL:{
        //asptr_val <register1> <destination>;*$destination = $register1
        advance();
        FoxType val=m_memory[(long long)code->number];
        advance();
        *m_memory[(long long)code->number].ptr=val;
        DISPATCH();
    }
    _OP_LDPTR_VAL:{
        advance();
        FoxType* ptr=m_memory[(long long)code->number].ptr;
        advance();
        m_memory[(long long)code->number]=*ptr;
        DISPATCH();
    }
    _OP_GETPTR:{
        advance();
        num index=m_memory[(long long)code->number].number;
        advance();
        m_memory[(long long)code->number].ptr=&m_memory[(long long)index];
        DISPATCH();
    }
    _OP_LEN:{
        advance();
        FoxType array=m_memory[(long long)code->number];
        advance();
        m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=array.size};
        DISPATCH();
    }
    _OP_FREE:{
        advance();
        free(m_memory[(long long)code->number].ptr);
        m_memory[(long long)code->number].ptr=NULL;
        m_memory[(long long)code->number].size=0;
        DISPATCH();        
    }
    _OP_MALLOC:{
        advance();
        num size=m_memory[(long long)code->number].number;
        advance();
        m_memory[(long long)code->number].ptr=malloc(size);
        DISPATCH();
    }
    _OP_REALLOC:{
        advance();
        num size=m_memory[(long long)code->number].number;
        advance();
        m_memory[(long long)code->number].ptr=realloc(m_memory[(long long)code->number].ptr,size);
        DISPATCH();
    }
       
    _OP_INSERT:{
        advance();
        FoxType r1=m_memory[(long long)code->number];
        advance();
        FoxType r2=m_memory[(long long)code->number];
        advance();
        m_memory[(long long)code->number].ptr[(long long)r2.number]=r1;
        DISPATCH();
    }
    _OP_APPEND:{
        advance();
        FoxType r1=m_memory[(long long)code->number];
        advance();
        FoxType* arr=&m_memory[(long long)code->number];
        arr->ptr[(long long)arr->size]=r1;
        arr->size++;
        DISPATCH();
    }
    _OP_POP:{
        advance();
        m_memory[(long long)code->number].size--;
        DISPATCH();
    }
    _OP_EQ_ARRAY:{
        advance();
        FoxType* array1=m_memory[(long long)code->number].ptr;
        num size1=m_memory[(long long)code->number].size;
        advance();
        FoxType* array2=m_memory[(long long)code->number].ptr;
        num size2=m_memory[(long long)code->number].size;
        advance();
        int res=0;
        if(size1==size2){
            res=equality_array(array1,size1,array2,size2);
        }
        m_memory[(long long)code->number]=(FoxType){.type=TYPE_NUM,.number=res};
        DISPATCH();
    }
    _OP_IF_NEQ:{
        if_bin_op(!=);
        DISPATCH();
    }
    _OP_IF_EQ:{
        if_bin_op(==);
        DISPATCH();
    }
    _OP_IF_LT:{
        if_bin_op(<);
        DISPATCH();
    }
    _OP_IF_GT:{
        if_bin_op(>);
        DISPATCH();
    }
    _OP_IF_LE:{
        if_bin_op(<=);
        DISPATCH();
    }
    _OP_IF_GE:{
        if_bin_op(>=);
        DISPATCH();
    }
    _OP_IF_AND:{
        if_bin_op(&&);
        DISPATCH();
    }
    _OP_IF_OR:{
        if_bin_op(||);
        DISPATCH();
    }
    
    _OP_IF:{
        //IF <address1> <address2>
        //If <address1> is true, jump to label <address2>
        advance();
        if((int)m_memory[(long long)code->number].number){
            advance();
            FoxType func = get_function(code->number);
            __execute(func.ptr,m_memory);
            if((opcode)(*(code+1)).number==OP_ELSE){
                advance();
                advance();
            }
        }
        else{
            advance();
            if((opcode)(*(code+1)).number==OP_ELSE){
                advance();
                advance();
                FoxType func = get_function(code->number);
                __execute(func.ptr,m_memory);
            }
        }
        DISPATCH();
    }
    
    _OP_ELSE:{
        printf("Error: else without a previous if op\n");
        exit(1);
    }
    
    _OP_LOAD:{
        //LOAD <data> <address>
        //Assign <data> to register <address>
        advance();
        FoxType value=(*code);
        advance();
        m_memory[(long long)code->number]=value;
        DISPATCH();
    }
    _OP_MOV:{
        //MOV <address> <address>
        //Assign register <address> to register <address>
        advance();
        FoxType address=(*code);
        advance();
        m_memory[(long long)code->number]=m_memory[(long long)address.number];
        DISPATCH();
    }    
    
    _OP_ADD:{
        //ADD <address> <address> <address>
        //Assign <address> to <address> + <address>
        bin_op(+);
        DISPATCH();
    }
    _OP_SUB:{
        //SUB <address> <address> <address>
        //Assign <address> to <address> - <address>
        bin_op(-);
        DISPATCH();
    }
    _OP_MUL:{
        //MUL <address> <address> <address>
        //Assign <address> to <address> * <address>
        bin_op(*);
        DISPATCH();
    }
    _OP_DIV:{
        //DIV <address> <address> <address>
        //Assign <address> to <address> / <address>
        bin_op(/);
        DISPATCH();
    }
    _OP_MOD:{
        //MOD <address> <address> <address>
        //Assign <address> to <address> % <address>
        bin_int_op(%);
        DISPATCH();
    }
    _OP_BIT_XOR:{
        //BIT_XOR <address> <address> <address>
        //Assign <address> to <address> ^ <address>
        bin_int_op(^);
        DISPATCH();
    }
    _OP_EQ:{
        //EQ <address> <address> <address>
        //Assign <address> to 1 if <address> == <address>
        bin_op(==);
        DISPATCH();
    }
    _OP_NEQ:{
        //NEQ <address> <address> <address>
        //Assign <address> to 1 if <address> != <address>
        bin_op(!=);
        DISPATCH();
    }
    _OP_LT:{
        //LT <address> <address> <address>
        //Assign <address> to 1 if <address> < <address>
        bin_op(<);
        DISPATCH();
    }
    _OP_GT:{
        //GT <address> <address> <address>
        //Assign <address> to 1 if <address> > <address>
        bin_op(>);
        DISPATCH();
    }
    _OP_LE:{
        //LE <address> <address> <address>
        //Assign <address> to 1 if <address> <= <address>
        bin_op(<=);
        DISPATCH();
    }
    _OP_GE:{
        //GE <address> <address> <address>
        //Assign <address> to 1 if <address> >= <address>
        bin_op(>=);
        DISPATCH();
    }
    _OP_AND:{
        //AND <address> <address> <address>
        //Assign <address> to 1 if <address> && <address>
        bin_op(&&);
        DISPATCH();
    }
    _OP_OR:{
        //OR <address> <address> <address>
        //Assign <address> to 1 if <address> || <address>
        bin_op(||);
        DISPATCH();
    }
    
    _OP_BIT_AND:{
        //BIT_AND <address> <address> <address>
        //Assign <address> to <address> & <address>
        bin_int_op(&);
        DISPATCH();
    }
    _OP_BIT_OR:{
        //BIT_OR <address> <address> <address>
        //Assign <address> to <address> | <address>
        bin_int_op(|);
        DISPATCH();
    }
    
    _OP_SHL:{
        //SHL <address> <address> <address>
        //Assign <address> to <address> << <address>
        bin_int_op(<<);
        DISPATCH();
    }
    _OP_SHR:{
        //SHR <address> <address> <address>
        //Assign <address> to <address> >> <address>
        bin_int_op(>>);
        DISPATCH();
    }
    _OP_NEG:{
        //NEG <address> <address>
        //Assign <address> to -<address>
        unary_op(-);
        DISPATCH();
    }
    _OP_NOT:{
        //NOT <address> <address>
        //Assign <address> to !<address>
        unary_op(!);
        DISPATCH();
    }
    _OP_BIT_NOT:{
        //BIT_NOT <address> <address>
        //Assign <address> to ~<address>
        unary_int_op(~);
        DISPATCH();
    }
    _OP_JMP:{
        //JMP <address>
        //Jump to <address>
        advance();
        FoxType func = get_function(code->number);
        __execute(func.ptr,m_memory);
        DISPATCH();
    }

    _OP_DL_OPEN:{
        advance();
        FoxType r1=m_memory[(long long)code->number];
        char* r1_str=to_str(r1);
        advance();
        #if defined _WIN32 || defined __CYGWIN__
        m_memory[(long long)code->number].so = LoadLibrary(r1_str);   
        #else
        m_memory[(long long)code->number].ptr = dlopen(r1_str, RTLD_NOW);   
        #endif  
        free(r1_str);
        r1_str=NULL;
        DISPATCH();
    }  
    _OP_DL_CLOSE:{
        advance();
        #if defined _WIN32 || defined __CYGWIN__
        FreeLibrary(m_memory[(long long)code->number].so);   
        #else
        dlclose(m_memory[(long long)code->number].ptr); 
        #endif    
        DISPATCH();
    }
    _OP_DL_CALL:{
        advance();
        char* r1=to_str(m_memory[(long long)code->number]);
        advance();
        #if defined _WIN32 || defined __CYGWIN__
        ext_func func = GetProcAddress(m_memory[(long long)code->number].so, r1);
        #else
        ext_func func = dlsym(m_memory[(long long)code->number].ptr, r1);
        #endif
        free(r1);
        r1=NULL;
        advance();
        (*func)(m_memory[(long long)code->number],m_memory);
        DISPATCH();
    }
    _OP_RET:{
        return;
    }
}