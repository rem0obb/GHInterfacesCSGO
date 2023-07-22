#pragma once

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <dlfcn.h>

typedef void* (*InstantiateInterfaceFn)();

class InterfaceReg
{
public:
	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;
	InterfaceReg			*m_pNext; // For the global list.
	static InterfaceReg		*s_pInterfaceRegs;
};

// interface return status
enum 
{
	IFACE_OK = 0,
	IFACE_FAILED
};

class GHInterfaces
{
private:
    const std::string m_file;
    void *m_dl;
    Dl_info m_info;

    bool Memcmp(const uint8_t *data, const std::vector<uint8_t>& pattern, const std::vector<uint8_t>& mask);

public:
    GHInterfaces(const std::string _file);
    ~GHInterfaces();

    u_int64_t IndexFunctionOffsetVtable64(u_int64_t _function, off_t _vtable_address);
    u_int64_t IndexFunctionOffsetVtable32(u_int64_t _function, off_t _vtable_address);
    uintptr_t FindPattern(const std::vector<uint8_t> &_pattern, const std::vector<uint8_t> &_mask, size_t _size);
    void *CreateInterfaceFN(const std::string _class);
    void* GetBaseAddress();
};