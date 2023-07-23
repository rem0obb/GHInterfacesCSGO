#pragma once

#include "trpHooks/TRPHooks.hpp"

#include <cstdint>

#define CLIENT_DLL_INTERFACE_VERSION "VClient018" // EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CHLClient, IBaseClientDLL, "VClient018", gHLClient );
#define ALIGN_ADDR(addr) ((void *)((size_t)(addr) & ~(page_size - 1)))

class GHooks
{
public:
    struct ClientModeShared
    {
        uintptr_t CreateMove;
    };

    struct CHLClient
    {
        uintptr_t HudProcessInput;
    };

    GHooks();
    ~GHooks();

    // Função de tratamento do sinal
    void Start();
};
