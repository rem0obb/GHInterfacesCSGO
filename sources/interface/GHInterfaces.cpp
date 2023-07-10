#include "interface/GHInterfaces.hpp"

#include <stdexcept>
#include <iostream>
#include <stdint.h>

GHInterfaces::GHInterfaces(const std::string _file, u_int64_t _vtable_address) : m_vtable_address(_vtable_address),
                                                                                 m_file(_file),
                                                                                 m_dl(dlopen(m_file.c_str(), RTLD_LAZY | RTLD_NOLOAD))
{
    dladdr(m_dl, &m_info);
}

GHInterfaces::~GHInterfaces()
{
}

u_int64_t GHInterfaces::IndexFunctionOffsetVtable64(u_int64_t _function)
{
    return (((_function - m_vtable_address) / 8) - 1);
}

u_int64_t GHInterfaces::IndexFunctionOffsetVtable32(u_int64_t _function)
{
    return (((_function - m_vtable_address) / 4) - 1);
}

bool GHInterfaces::Memcmp(const uint8_t *_data, const std::vector<uint8_t> &_pattern, const std::vector<uint8_t> &_mask)
{
    for (size_t i = 0; i < _pattern.size(); ++i)
    {
        if (*_data++ != _pattern[i] && _mask[i] != '?')
            return false;
    }
    return true;
}

uintptr_t GHInterfaces::FindPattern(const std::vector<uint8_t> &_pattern, const std::vector<uint8_t> &_mask, size_t _size)
{
    uintptr_t address = reinterpret_cast<uintptr_t>(m_info.dli_fbase);
    uintptr_t end_address = 0;

    std::cout << "[*] Scanning memory in base address : 0x" << std::hex << address << "\n";
    for (size_t i = 0; i < _size; i++)
    {
        end_address = address + i;
        if (GHInterfaces::Memcmp(reinterpret_cast<uint8_t *>(end_address), _pattern, _mask))
        {
            std::cout << "[*] Scan found memory in address : 0x" << end_address << "\n";
            return end_address;
        }
    }

    std::cout << "[*] Not found pattern, end memory : 0x" << end_address << "\n";

    return 0;
}

void *GHInterfaces::CreateInterfaceFN(const std::string _class)
{
    if (m_dl)
    {
        void *address = dlsym(m_dl, "CreateInterface");

        return nullptr;
    }
    else
    {
        throw std::runtime_error("Error in opened file dl " + std::string(dlerror()));
    }
}