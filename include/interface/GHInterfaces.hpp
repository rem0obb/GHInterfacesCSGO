#pragma once

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <dlfcn.h>

class GHInterfaces
{
private:
    u_int64_t m_vtable_address;
    const std::string m_file;
    void *m_dl;
    Dl_info m_info;

    bool Memcmp(const uint8_t *data, const std::vector<uint8_t>& pattern, const std::vector<uint8_t>& mask);

public:
    GHInterfaces(const std::string _file, u_int64_t _vtable_address);
    ~GHInterfaces();

    u_int64_t IndexFunctionOffsetVtable64(u_int64_t _function);
    u_int64_t IndexFunctionOffsetVtable32(u_int64_t _function);
    void *CreateInterfaceFN(const std::string _class);
    uintptr_t FindPattern(const std::vector<uint8_t> &_pattern, const std::vector<uint8_t> &_mask, size_t _size);
};