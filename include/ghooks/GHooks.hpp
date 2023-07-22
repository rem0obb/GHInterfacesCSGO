#pragma once

#include "trpHooks/TRPHooks.hpp"

#define CLIENT_DLL_INTERFACE_VERSION "VClient018" // EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CHLClient, IBaseClientDLL, "VClient018", gHLClient );
#define ALIGN_ADDR(addr) ((void*)((size_t)(addr) & ~(page_size - 1)))

class GHooks
{
private:
    //TRPHooks m_Hook;
    
    //bool VClientHook_CreateMove(void *_this, float _flInputSampleTime, void *_cmd);
    //bool (*VClientHook_CreateMove)(void *_this, float _flInputSampleTime, void *_cmd);



public:
    GHooks();
    ~GHooks();

    void Interface_VClient();
};