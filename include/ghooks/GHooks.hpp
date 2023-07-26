#pragma once

#include <cstdint>
#include <csignal>

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
        uintptr_t CreateMove;
    };

    struct CHLClient
    {
        uint64_t *vTable;
        uintptr_t HudProcessInput;
    };

    GHooks();
    ~GHooks();

    void Start();
    void ReadjustPages();

    ClientModeShared getClassClientModeShared();
    CHLClient getClassCHLClient();
};
