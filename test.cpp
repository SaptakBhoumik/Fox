#include "fox/fox.hpp"
#include "fox_dissambler/dissambler.hpp"
#include "fox_file/file.hpp"
#include <stdio.h>
#include <sys/resource.h>
using namespace FoxVm;
int main(){
    struct rlimit R;
    getrlimit(RLIMIT_STACK, &R);
    R.rlim_cur = R.rlim_max;
    setrlimit(RLIMIT_STACK, &R);
    FoxType m_memory[MAX_LIMIT];

    FoxType code2[]={
        {.type=TYPE_OP,.number=OP_ADD},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_OP,.number=OP_MUL},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_OP,.number=OP_ADD},
        {.type=TYPE_NUM,.number='i'},
        {.type=TYPE_NUM,.number=100},
        {.type=TYPE_NUM,.number='i'},
        {.type=TYPE_OP,.number=OP_IF_NEQ},
        {.type=TYPE_NUM,.number='i'},
        {.type=TYPE_NUM,.number='l'},
        {.type=TYPE_NUM,.number=0},
        {.type=TYPE_OP,.number=OP_RET},
    };
    FoxType labels[1];
    labels[0]=(FoxType){.type=TYPE_LABEL};
    labels[0].ptr=code2;
    labels[0].size=17;
    FoxType code[15];
    code[0]=(FoxType){.type=TYPE_OP ,.number=OP_LOAD};
    code[1]=(FoxType){.type=TYPE_NUM ,.number=1000000};
    code[2]=(FoxType){.type=TYPE_NUM ,.number='l'};
    code[3]=(FoxType){.type=TYPE_OP ,.number=OP_LOAD};
    code[4]=(FoxType){.type=TYPE_NUM ,.number=0};
    code[5]=(FoxType){.type=TYPE_NUM ,.number='i'};
    code[6]=(FoxType){.type=TYPE_OP ,.number=OP_LOAD};
    code[7]=(FoxType){.type=TYPE_NUM ,.number=1};
    code[8]=(FoxType){.type=TYPE_NUM ,.number=0};
    code[9]=(FoxType){.type=TYPE_OP ,.number=OP_LOAD};
    code[10]=(FoxType){.type=TYPE_NUM ,.number=1};
    code[11]=(FoxType){.type=TYPE_NUM ,.number=100};
    code[12]=(FoxType){.type=TYPE_OP ,.number=OP_JMP};
    code[13]=(FoxType){.type=TYPE_NUM,.number=0};
    code[14]=(FoxType){.type=TYPE_OP ,.number=OP_RET};

    auto fox_file=FoxFile(code,15,labels,1,"test.fox");
    fox_file.write();
    fox_file=FoxFile("test.fox");
    auto code_read=fox_file.read();
    auto dis=FoxDis(code_read.second,code_read.first.first,code_read.first.second);
    dis.disassemble();

    
    auto x= Fox(code_read.second,m_memory,code_read.first.first);
    x.execute();
    // x.execute(code3,m_memory);
    // printf("%p\n",m_memory[0].PTR);
    // printf("%f\n",m_memory[100].ptr[1].number);
    // printf("%f\n",m_memory[0].ptr[1].number);
    return 0;
}