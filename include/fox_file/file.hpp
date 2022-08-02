#ifndef FOX__FILE__H
#define FOX__FILE__H
#include "fox/fox.hpp"
#include <fstream>
#include <string>
#include <vector>
namespace FoxVm{
class FoxFile{
    std::string m_filename="";
    FoxType* m_labels;
    size_t m_labels_count=0;
    FoxType* m_code=nullptr;
    size_t m_size=0;
    std::ofstream m_output_file;
    std::ifstream m_read_file;
    std::vector<FoxType**> to_clear;
    std::pair<std::pair<FoxType*,size_t>,FoxType*> read(std::string);
    void write(FoxType*,size_t);
    void read(FoxType*,size_t);
    void write(FoxType*,size_t,std::string);
    public:
    FoxFile(FoxType*,size_t,FoxType*,size_t,std::string);
    FoxFile(std::string);
    void write();
    void clean_up();
    std::pair<std::pair<FoxType*,size_t>,FoxType*> read();
};
}
#endif