#include "vmt/VMThook.hpp"

#include <stdexcept>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

VMTHook::VMTHook(uint64_t *_vtable_address, int _size) : m_vtable(_vtable_address),
                                                         m_vtable_size(_size)

{
    m_vtable_copy = new uint64_t[_size]; // first allocate array of row pointer

    Memcpy(m_vtable_copy, _vtable_address, _size);

    std::cout << "[*] VTable [" << _vtable_address << "] successfully copied" << std::endl;
}

VMTHook::~VMTHook()
{
}

void VMTHook::VMTInstallHook(uint _index_function, uintptr_t address_function_hook)
{
    if (_index_function > m_vtable_size)
        throw std::runtime_error("[!] Index exceeded VTable");

    m_vtable_copy[_index_function] = address_function_hook;
}

void VMTHook::VMTUninstall(uint _index_function)
{
    if (_index_function > m_vtable_size)
        throw std::runtime_error("[!] Index exceeded VTable");

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
