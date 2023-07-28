#pragma once

#include <stdint.h>

class VMTHook
{
public:
    VMTHook(uint64_t *_vtable_address, int _size);
    ~VMTHook();

    bool VMTInstallHook(int _index_function, uintptr_t address_function_hook);
    void VMTUninstall(int _index_function);
    uint64_t *VMTGetVTableCopy();

private:
    uint64_t *m_vtable;
    uint64_t *m_vtable_copy;

    inline void Memcpy(uint64_t *_dst, uint64_t *_src, int _size);
};