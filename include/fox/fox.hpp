#ifndef MAIN__FOX__HPP
#define MAIN__FOX__HPP
extern "C" {
#include "fox_opcode.h"
}
#include <unordered_map>
namespace FoxVm {
class Fox{
    FoxType* m_code=nullptr,*m_memory=nullptr;
    public:
        Fox(FoxType* code,FoxType* memory,FoxType* labels) noexcept;
        void execute() noexcept;
        FoxType* getMemory() noexcept;
};
}
#endif
