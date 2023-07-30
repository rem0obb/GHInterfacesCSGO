#pragma once

#include <stdint.h>
#include <cstddef>

class VMTHook
{
public:
    VMTHook(uint64_t *_vtable_address, int _size);
    ~VMTHook();

    void VMTInstallHook(unsigned int _index_function, uintptr_t address_function_hook);
    void VMTUninstall(unsigned int _index_function);
    uint64_t *VMTGetVTableCopy();

private:
    uint64_t *m_vtable;
    uint64_t *m_vtable_copy;
    size_t m_vtable_size;

    inline void Memcpy(uint64_t *_dst, uint64_t *_src, int _size);
};