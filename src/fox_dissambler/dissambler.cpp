#include "fox_dissambler/dissambler.hpp"
#include <iostream>
#define advance() code++;
#define op_print(text) std::cout<<"\e[1m\e[91m"<<text<<"\e[0m\e[0m";
#define reg_print(text) std::cout<<"\e[94m$"<<text<<"\e[0m";
#define num_print(text) std::cout<<"\e[36m"<<text<<"\e[0m";
#define comment_print(text) std::cout<<"\e[90m"<<text<<"\e[0m";
#define bin_op(text,op) {\
                        op_print(text);\
                        std::cout<<" ";\
                        code++;\
                        auto r1=*code;\
                        code++;\
                        auto r2=*code;\
                        code++;\
                        auto r3=*code;\
                        reg_print(r1.number)\
                        std::cout<<" ";\
                        reg_print(r2.number)\
                        std::cout<<" ";\
                        reg_print(r3.number)\
                        std::cout<<"\t";\
                        comment_print(";$"<<r3.number<<"=$"<<r1.number<<op<<"$"<<r2.number<<"\n");\
                        }

#define prefix_op(text,op) {\
                        op_print(text);\
                        std::cout<<" ";\
                        code++;\
                        auto r1=*code;\
                        code++;\
                        auto r2=*code;\
                        reg_print(r1.number)\
                        std::cout<<" ";\
                        reg_print(r2.number)\
                        std::cout<<"\t";\
                        comment_print(";$"<<r2.number<<"="<<op<<"$"<<r1.number<<"\n");\
                        }

#define if_op(text,op) {\
                        op_print(text);\
                        std::cout<<" ";\
                        code++;\
                        auto r1=*code;\
                        code++;\
                        auto r2=*code;\
                        code++;\
                        auto r3=*code;\
                        reg_print(r1.number)\
                        std::cout<<" ";\
                        reg_print(r2.number)\
                        std::cout<<" ";\
                        reg_print(r3.number)\
                        std::cout<<"\t";\
                        comment_print(";if($"<<r1.number<<op<<"$"<<r2.number<<"):"<<"jmp "<<r3.number<<"\n");\
                    }

#define function(op) {\
                        op_print(op);\
                        std::cout<<" ";\
                        advance();\
                        auto r1=*code;\
                        reg_print(code->number);\
                        std::cout<<" ";\
                        advance();\
                        auto r2=*code;\
                        reg_print(code->number);\
                        std::cout<<"\t";\
                        comment_print(";$"<<r2.number<<"="<<op<<"($"<<r1.number<<")");\
                        std::cout<<"\n";\
                        break;\
                    }
    
#define methord(op) {\
                        op_print(op);\
                        std::cout<<" ";\
                        advance();\
                        auto r1=*code;\
                        reg_print(code->number);\
                        std::cout<<" ";\
                        advance();\
                        auto r2=*code;\
                        reg_print(code->number);\
                        std::cout<<" ";\
                        advance();\
                        auto r3=*code;\
                        reg_print(code->number);\
                        std::cout<<"\t";\
                        comment_print(";$"<<r3.number<<"="<<"$"<<r2.number<<"."<<op<<"($"<<r1.number<<")");\
                        std::cout<<"\n";\
                        break;\
                    }
       
namespace FoxVm{
void print_array(FoxType* arr,size_t size,bool is_comment=false) noexcept{
    std::cout<<"[";
    for(size_t i=0;i<size;++i){
        if(arr[i].type==TYPE_NUM){
            if(is_comment){
                comment_print(arr[i].number);
            }
            else{
                num_print(arr[i].number);
            }
        }
        else if(arr[i].type==TYPE_ARRAY){
            print_array(arr[i].ptr,arr[i].size,is_comment);
        }
        else if(arr[i].type==TYPE_STR){
            if (!is_comment) {
                std::cout<<"\e[33m";
            }
            std::cout<<"\'";
            for(size_t j=0;j<arr[i].size;++j){
                if(is_comment){
                    comment_print(arr[i].ptr[j].number);
                }
                else{
                    printf("%c",(char)arr[i].ptr[j].number);
                }
            }
            std::cout<<"\'";
            if (!is_comment) {
                std::cout<<"\e[0m";
            }
        }
        else if(arr[i].type==TYPE_PTR){
            if(is_comment){
                comment_print(arr[i].ptr);
            }
            else{
                num_print(arr[i].ptr);
            }
        }
        if(i<size-1){
            std::cout<<",";
        }
    }
    std::cout<<"]";
}
FoxDis::FoxDis(FoxType* code,FoxType* labels,size_t label_count) noexcept{
    m_code=code;
    m_labels=labels;
    m_label_count=label_count;
}
void FoxDis::disassemble() noexcept{
    for(size_t i=0;i<m_label_count;++i){
        reg_print(i<<":\n");
        disassemble(m_labels[i].ptr,"\t");
    }
    disassemble(m_code,"");
}
void FoxDis::disassemble(FoxType* code,std::string spacing) noexcept{
    while(true){
        std::cout<<spacing;
        switch((opcode)code->number){
            case OP_LOAD:{
                if((code+1)->type!=TYPE_LABEL){
                    op_print("load");
                    std::cout<<" ";
                }
                else{
                    reg_print((code+2)->number);
                }
                advance(); 
                auto val=*code;       
                if(code->type==TYPE_NUM){
                    num_print(code->number);
                }
                else if(code->type==TYPE_ARRAY){
                    print_array(code->ptr,code->size);
                }
                else if(code->type==TYPE_STR){
                    std::cout<<"\e[33m\'";
                    for(size_t i=0;i<code->size;++i){
                        printf("%c",(char)code->ptr[i].number);
                    }
                    std::cout<<"\'\e[0m";
                }
                else if(code->type==TYPE_PTR){
                    num_print(code->ptr);
                }
                advance();
                if(val.type!=TYPE_LABEL){
                    std::cout<<" ";
                    auto reg=*code;
                    reg_print(code->number);
                    std::cout<<"\t";
                    comment_print(";$"<<reg.number<<"=");
                    if(val.type==TYPE_NUM){
                        comment_print(val.number);
                    }
                    else if(val.type==TYPE_ARRAY){
                        print_array(val.ptr,val.size,true);
                    }
                    else if(val.type==TYPE_STR){
                        comment_print("\'");
                        for(size_t i=0;i<val.size;++i){
                            comment_print((char)val.ptr[i].number);
                        }
                        comment_print("\'");
                    }
                    else if(val.type==TYPE_PTR){
                        comment_print(val.ptr);
                    }
                }
                std::cout<<"\n";
                break;
            }
            case OP_MOV:{
                op_print("mov");
                std::cout<<" ";
                advance();
                auto r1=*code;
                reg_print(code->number);
                std::cout<<" ";
                advance();
                auto r2=*code;
                reg_print(code->number);
                std::cout<<"\t";
                comment_print(";$"<<r2.number<<"=$"<<r1.number<<"\n");
                break;
            }
            case OP_RET:{
                op_print("ret");
                std::cout<<"\n";
                goto end;
            }
            case OP_ADD:{
                bin_op("add ","+");
                break;
            }
            case OP_SUB:{
                bin_op("add ","+");
                break;
            }
            case OP_MUL:{
                bin_op("mul ","*");
                break;
            }
            case OP_DIV:{
                bin_op("div ","/");
                break;
            }
            case OP_MOD:{
                bin_op("mod ","%");
                break;
            }
            case OP_BIT_XOR:{
                bin_op("bit_xor","^");
                break;
            }
            case OP_EQ:{
                bin_op("eq ","==");
                break;
            }
            case OP_NEQ:{
                bin_op("neq ","!=");
                break;
            }
            case OP_LT:{
                bin_op("lt ","<");
                break;
            }
            case OP_GT:{
                bin_op("gt ",">");
                break;
            }
            case OP_LE:{
                bin_op("le ","<=");
                break;
            }
            case OP_GE:{
                bin_op("ge ",">=");
                break;
            }
            case OP_AND:{
                bin_op("and ","&&");
                break;
            }
            case OP_OR:{
                bin_op("or ","||");
                break;
            }
            case OP_BIT_AND:{
                bin_op("bit_and","&");
                break;
            }
            case OP_BIT_OR:{
                bin_op("bit_or","|");
                break;
            }
            case OP_SHL:{
                bin_op("shl ","<<");
                break;
            }
            case OP_SHR:{
                bin_op("shr ",">>");
                break;
            }
            case OP_IF:{
                op_print("if");
                std::cout<<" ";
                advance();
                auto r1=*code;
                reg_print(code->number);
                std::cout<<" ";
                advance();
                auto r2=*code;
                reg_print(code->number);
                std::cout<<" ";
                std::cout<<"\t";
                comment_print(";if($"<<r1.number<<"):jmp $"<<r2.number);
                std::cout<<"\n";
                break;
            }
            case OP_ELSE:{
                op_print("else ");
                advance();
                reg_print(code->number);
                std::cout<<"\t\t";
                comment_print(";else:jmp $"<<code->number<<"\n");
            }
            case OP_IF_EQ:{
                if_op("if_eq ","==");
                break;
            }
            case OP_IF_NEQ:{
                if_op("if_neq ","!=");
                break;
            }
            case OP_IF_LT:{
                if_op("if_lt ","<");
                break;
            }
            case OP_IF_GT:{
                if_op("if_gt ",">");
                break;
            }
            case OP_IF_LE:{
                if_op("if_le ","<=");
                break;
            }
            case OP_IF_GE:{
                if_op("if_ge ",">=");
                break;
            }
            case OP_IF_OR:{
                if_op("if_or ","||");
                break;
            }
            case OP_IF_AND:{
                if_op("if_and ","&&");
                break;
            }
            case OP_EQ_ARRAY:{
                if_op("eq_array ","==");
                break;
            }
            case OP_JMP:{
                op_print("jmp");
                std::cout<<" ";
                advance();
                reg_print(code->number);
                std::cout<<"\t\t";
                comment_print(";jmp $"<<code->number);
                std::cout<<"\n";
                break;
            }
            case OP_DL_OPEN:{
                function("dl_open");
                break;
            }
            case OP_DL_CLOSE:{
                op_print("dl_close");
                std::cout<<" ";
                advance();
                reg_print(code->number);
                std::cout<<"\t\t";
                comment_print(";dl_close($"<<code->number<<")");
                std::cout<<"\n";
                break;
            }
            case OP_FREE:{
                op_print("free");
                std::cout<<" ";
                advance();
                reg_print(code->number);
                std::cout<<"\t\t";
                comment_print(";free($"<<code->number<<")");
                std::cout<<"\n";
                break;
            }
            case OP_MALLOC:{
                function("malloc")
                break;
            }
            case OP_REALLOC:{
                function("realloc");
                break;
            }
            case OP_NEG:{
                prefix_op("neg","-")
                break;
            }
            case OP_BIT_NOT:{
                prefix_op("bit_not","~")
                break;
            }
            case OP_NOT:{
                prefix_op("not","!")
                break;
            }
            case OP_LEN:{
                function("len");
                break;
            }
            case OP_ASPTR_VAL:{
                op_print("asptr_val");
                std::cout<<" ";
                code++;
                auto r1=*code;
                code++;
                auto r2=*code;
                reg_print(r1.number)
                std::cout<<" ";
                reg_print(r2.number)
                comment_print(";*$"<<r2.number<<"="<<"$"<<r1.number<<"\n");
                break;
            }
            case OP_GETPTR:{
                prefix_op("getptr","&");
                break;
            }
            case OP_LDPTR_VAL:{
                prefix_op("ldptr_val","*");
                break;
            }
            case OP_INSERT:{
                methord("insert");
                break;
            }
            case OP_APPEND:{
                methord("append");
                break;
            }
            case OP_AT:{
                methord("at");
                break;
            }
            case OP_POP:{
                op_print("pop");
                std::cout<<" ";
                advance();
                reg_print(code->number);
                std::cout<<"\t\t";
                comment_print(";$"<<code->number<<".pop()");
                std::cout<<"\n";
                break;
            }
            case OP_BACK:{
                op_print("back");
                std::cout<<" ";
                advance();
                auto r1=*code;
                reg_print(code->number);
                std::cout<<" ";
                advance();
                auto r2=*code;
                reg_print(code->number);
                std::cout<<"\t";
                comment_print(";$"<<r2.number<<"=$"<<r1.number<<".back()");
                std::cout<<"\n";
                break;
            }
            case OP_DL_CALL:{
                op_print("dl_call");
                std::cout<<" ";
                advance();
                auto r1=*code;
                reg_print(code->number);
                std::cout<<" ";
                advance();
                auto r2=*code;
                reg_print(code->number);
                std::cout<<" ";
                advance();
                auto r3=*code;
                reg_print(code->number);
                std::cout<<"\t";
                comment_print(";$"<<r2.number<<".dl_call"<<"($"<<r1.number<<",arg=$"<<r3.number<<")");
                std::cout<<"\n";
                break;
            }
            default:{
                std::cout<<"Invalid opcode "<<(opcode)code->number<<std::endl;
                exit(1);
            }
        }
        code++;
    }
    end:{}
}
}