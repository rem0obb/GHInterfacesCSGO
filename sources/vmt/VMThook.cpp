#include "vmt/VMThook.hpp"
#include <stdio.h>

VMTHook::VMTHook(uint64_t *_vtable_address, int _size) : m_vtable(_vtable_address)
{
    m_vtable_copy = new uint64_t[_size]; // first allocate array of row pointer

    Memcpy(m_vtable_copy, _vtable_address, _size);
}

VMTHook::~VMTHook()
{
    delete m_vtable_copy;
}

bool VMTHook::VMTInstallHook(int _index_function, uintptr_t address_function_hook)
{
    m_vtable_copy[_index_function] = address_function_hook;
}

void VMTHook::VMTUninstall(int _index_function)
{
    m_vtable_copy[_index_function] = m_vtable[_index_function];
}

uint64_t *VMTHook::VMTGetVTableCopy()
{
    return m_vtable_copy;
}

void VMTHook::Memcpy(uint64_t *_dst, uint64_t *_src, int _size)
{
    for (int i = 0; i < _size; i++)
    {
        _dst[i] = _src[i];
    }
}
