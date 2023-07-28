#pragma once

#include <cstdint>
#include <csignal>
#include "vmt/VMThook.hpp"

#define CLIENT_DLL_INTERFACE_VERSION "VClient018" // EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CHLClient, IBaseClientDLL, "VClient018", gHLClient );
#define ALIGN_ADDR(addr) ((void *)((size_t)(addr) & ~(m_page_size - 1)))

class GHooks
{
private:
    static size_t m_page_size;

public:
    static void GetClientModeSharedVTABLE(int sig, siginfo_t *info, void *ucontext);

    // class
    struct ClientModeShared
    {
        uint64_t *vTable;
        uint64_t *g_pClientMode;
        size_t vTableSize;
    };

    struct CHLClient
    {
        uint64_t *vTable;
        size_t vTableSize;
    };

    GHooks();
    ~GHooks();

    void Start();
    void ReadjustPagesVTABLE();

    ClientModeShared getClassClientModeShared();
    CHLClient getClassCHLClient();
};
