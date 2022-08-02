#include "fox_file/file.hpp"
#define HEADER_TEXT "foxvm"
#include <iostream>
#include <cstdint>
#include <string.h>
namespace FoxVm{
inline num handle_endian( const double inFloat )
{
    #if BYTE_ORDER == LITTLE_ENDIAN
    double retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;
    returnFloat[0] = floatToConvert[7];
    returnFloat[1] = floatToConvert[6];
    returnFloat[2] = floatToConvert[5];
    returnFloat[3] = floatToConvert[4];
    returnFloat[4] = floatToConvert[3];
    returnFloat[5] = floatToConvert[2];
    returnFloat[6] = floatToConvert[1];
    returnFloat[7] = floatToConvert[0];
    return retVal;
    #else
    return inFloat;
    #endif
}
inline num read_num(num x){
    # if BYTE_ORDER == __LITTLE_ENDIAN
    int num_byte=sizeof(num)/sizeof(char);
    num returnFloat =0;
    char* inputVal=reinterpret_cast<char*>(&x);
    char* returnVal=reinterpret_cast<char*>(&returnFloat);
    for(int i=num_byte;i>0;i++){     
        returnVal[i]=inputVal[num_byte-i];
    }
    return returnFloat;
    #else 
    return x;
    #endif
}
FoxFile::FoxFile(FoxType* code ,size_t size,FoxType* labels,size_t labels_count,std::string filename){
    m_filename=filename;
    m_code=code;
    m_size=size;
    m_labels=labels;
    m_labels_count=labels_count;
}
FoxFile::FoxFile(std::string filename){
    m_filename=filename;
}
void FoxFile::write(){
    write(m_code,m_size,m_filename);
}
std::pair<std::pair<FoxType*,size_t>,FoxType*> FoxFile::read(){
    return read(m_filename);
}
void FoxFile::write(FoxType* code,size_t size,std::string filename){
    m_output_file=std::ofstream(filename, std::ios::binary);
    if(!m_output_file){
        std::cout<<"Error opening file: "<<filename<<"\n";
        exit(1);
    }
    m_output_file.write(HEADER_TEXT,sizeof(char)*5);
    {
        auto l_size=htobe64(m_labels_count);
        m_output_file.write(reinterpret_cast<char*>(&l_size), sizeof(l_size));
        for (size_t i = 0; i < m_labels_count; i++) {
            auto b_size=htobe64(m_labels[i].size);
            m_output_file.write(reinterpret_cast<char*>(&b_size), sizeof(b_size));
            write(m_labels[i].ptr,m_labels[i].size);
        }
    }
    auto b_size=htobe64(size);
    m_output_file.write(reinterpret_cast<char*>(&b_size), sizeof(b_size));
    write(code,size);
    m_output_file.close();
}
void FoxFile::write(FoxType* code,size_t size){
    for(size_t i=0;i<size;i++){
        char type=code[i].type;
        m_output_file.write(reinterpret_cast<char*>(&type), sizeof(type));
        if(code[i].type==TYPE_NUM){
            auto byte_num=handle_endian(code[i].number);
            m_output_file.write(reinterpret_cast<char*>(&byte_num), sizeof(code[i].number));
        }
        else if(code[i].type==TYPE_OP){
            char op= (char)code[i].number;
            m_output_file.write(reinterpret_cast<char*>(&op), sizeof(op));
        }
        else if(code[i].type==TYPE_REGISTER){
            uint16_t op= htobe16((uint16_t)code[i].number);
            m_output_file.write(reinterpret_cast<char*>(&op), sizeof(op));
        }
        else if(code[i].type==TYPE_STR){
            auto b_size=htobe64(code[i].size);
            m_output_file.write(reinterpret_cast<char*>(&b_size), sizeof(b_size));
            for(size_t j=0;j<code[i].size;++j){
                char res=code[i].ptr[j].number;
                m_output_file.write(reinterpret_cast<char*>(&res), sizeof(res));
            }
        }
        else if(code[i].type==TYPE_ARRAY){
            auto b_size=htobe64(code[i].size);
            m_output_file.write(reinterpret_cast<char*>(&b_size), sizeof(b_size));
            write(code[i].ptr,code[i].size);
        }
    }
}
std::pair<std::pair<FoxType*,size_t>,FoxType*> FoxFile::read(std::string filename){ 
    m_read_file=std::ifstream(filename, std::ios::binary);
    if(!m_read_file){
        std::cout<<"Error opening file: "<<filename<<"\n";
        exit(1);
    }
    char header[5];
    m_read_file.read(header,sizeof(char)*5);
    for(size_t i=0;i<5;i++){
        if(header[i]!=HEADER_TEXT[i]){
            std::cout<<"File is not a FoxVM file\n";
            exit(1);
        }
    }
    {
        size_t l_size=0;
        m_read_file.read(reinterpret_cast<char*>(&l_size), sizeof(l_size));
        m_labels_count=be64toh(l_size);
        m_labels=new FoxType[m_labels_count];
        for (size_t i = 0; i < m_labels_count; i++) {
            size_t b_size=0;
            m_read_file.read(reinterpret_cast<char*>(&b_size), sizeof(b_size));
            auto l_size=be64toh(b_size);
            m_labels[i].ptr=new FoxType[l_size];
            m_labels[i].size=l_size;
            read(m_labels[i].ptr,l_size);
        }
    }
    m_read_file.read(reinterpret_cast<char*>(&m_size), sizeof(m_size));
    m_size=be64toh(m_size);
    m_code=NULL;
    m_code=(FoxType*)malloc(m_size*sizeof(FoxType));
    read(m_code,m_size);
    m_read_file.close(); 
    return std::make_pair(std::make_pair(m_labels,m_labels_count),m_code);
}
void FoxFile::read(FoxType* code,size_t size){
    for(size_t i=0;i<size;i++){
        char type=0;
        m_read_file.read(reinterpret_cast<char*>(&type), sizeof(type));
        code[i].type=(curr_type)type;
        if(code[i].type==TYPE_NUM){
            m_read_file.read(reinterpret_cast<char*>(&code[i].number), sizeof(code[i].number));
            code[i].number=handle_endian(code[i].number);
        }
        else if(code[i].type==TYPE_OP){
            char op=0;
            m_read_file.read(reinterpret_cast<char*>(&op), sizeof(op));
            code[i].number=op;
        }
        else if(code[i].type==TYPE_REGISTER){
            uint16_t op=0;
            m_read_file.read(reinterpret_cast<char*>(&op), sizeof(op));
            code[i].number=be16toh(op);
        }
        else if(code[i].type==TYPE_STR){
            m_read_file.read(reinterpret_cast<char*>(&code[i].size), sizeof(code[i].size));
            code[i].size=be64toh(code[i].size);
            code[i].ptr=(FoxType*)malloc(code[i].size*sizeof(FoxType));
            for(size_t j=0;j<code[i].size;++j){
                char res=0;
                m_read_file.read(reinterpret_cast<char*>(&res), sizeof(res));
                code[i].ptr[j]=(FoxType){.type=TYPE_NUM ,.number=(num)res};
            }
        }
        else if(code[i].type==TYPE_ARRAY){
            m_read_file.read(reinterpret_cast<char*>(&code[i].size), sizeof(code[i].size));
            code[i].size=be64toh(code[i].size);
            code[i].ptr=(FoxType*)malloc(code[i].size*sizeof(FoxType));
            read(code[i].ptr,code[i].size);
            to_clear.push_back(&code[i].ptr);
        }
    }
}
void FoxFile::clean_up(){
    if(m_code!=NULL){
        for(auto&i:to_clear){
            if((*i)!=NULL) {
                free(*i);
                *i=NULL;
            }
        }
        free(m_code);
        m_code=NULL;
    }
}
}