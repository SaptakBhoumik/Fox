#include "fox/fox.hpp"
extern "C" void __execute(FoxType* code,FoxType* m_memory) noexcept;
namespace FoxVm {
thread_local FoxType* m_labels;
Fox::Fox(FoxType* code,FoxType* memory,FoxType* labels) noexcept{
    m_code=code;
    m_memory=memory;
    m_labels=labels;
}
void Fox::execute() noexcept{
    __execute(m_code,m_memory);
}
FoxType* Fox::getMemory() noexcept{
    return m_memory;
}
} // namespace FoxVm
extern "C" FoxType get_function(num name) noexcept{
    return FoxVm::m_labels[(long long)name];
}