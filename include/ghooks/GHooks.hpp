#pragma once

#include <cstdint>

#define CLIENT_DLL_INTERFACE_VERSION "VClient018" // EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CHLClient, IBaseClientDLL, "VClient018", gHLClient );
#define ALIGN_ADDR(addr) ((void *)((size_t)(addr) & ~(page_size - 1)))

class GHooks
{
public:
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

    ClientModeShared getClassClientModeShared();
    CHLClient getClassCHLClient();
};
