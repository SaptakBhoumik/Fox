#ifndef FOX__DIS__H
#define FOX__DIS__H
#include "fox/fox.hpp"
#include <string>
#include <vector>
#include <unordered_map>
namespace FoxVm{
class FoxDis{
    FoxType* m_code=nullptr, *m_labels=nullptr;
    size_t m_label_count=0;
    void disassemble(FoxType*,std::string) noexcept;
    public:
    FoxDis(FoxType*,FoxType*,size_t) noexcept;
    void disassemble() noexcept;
};
}
#endif