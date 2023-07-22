#include "trpHooks/TRPHooks.hpp"

#include <stdexcept>
#include <stdint.h>

TRPHooks::TRPHooks()
{
}
TRPHooks::~TRPHooks()
{
}

bool TRPHooks::TRP_Function(off_t *_address_function, off_t *_address_function_hook, size_t _len)
{
    if (mprotect(_address_function, _len, PROT_WRITE | PROT_EXEC | PROT_READ) < 0)
    {
        throw std::runtime_error("mprotect failed with error");
        return false;
    }
    else
    {
        uintptr_t relative_function = _address_function - _address_function_hook - 5; // get relative function address
        *_address_function = 0xE9; // opcode jmp
        *(uintptr_t*)(_address_function + 1) = relative_function; // jmp <address_relative>

        return true;
    }
}