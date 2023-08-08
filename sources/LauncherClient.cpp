#include <dlfcn.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/mman.h>
#include "config/config.hpp"
#include "ghooks/GHooks.hpp"
#include "internals/usercmd.hpp"
#include "internals/icliententitylist.hpp"

extern "C"
{

    CSGO::IClientEntityList *ClientEntityList;

    static bool (*CreateMove)(void *_this, float flInputSampleTime, CSGO::CUserCmd *cmd);
    bool CreateMoveHook(void *_this, float flInputSampleTime, CSGO::CUserCmd *cmd)
    {
        std::cout << "[*] Hooked Function CreateMove" << std::endl
                  << "Cmd.buttons = " << cmd->buttons << std::endl;

        for (int i = 0; i < 64; i++)
        {
            auto player = ClientEntityList->GetClientEntity(i);
            if (player)
                std::cout << "Player " << std::dec << i << std::endl;
        }

        return CreateMove(_this, flInputSampleTime, cmd);
    }

    void ClientModeSharedHook()
    {
        GHooks hooks;

        hooks.Start();

        sleep(10);

        // Hooks
        std::cout << "[*] Hooking Methods Class ClientModeShared" << std::endl;

        VMTHook vmt_hook_client(hooks.getClassClientModeShared().vTable, hooks.getClassClientModeShared().vTableSize);

        ClientEntityList = hooks.getClassClientEntityList();

        CreateMove = reinterpret_cast<bool (*)(void *_this, float flInputSampleTime, CSGO::CUserCmd *cmd)>(hooks.getClassClientModeShared().vTable[25]);

        vmt_hook_client.VMTInstallHook(25, (uint64_t)&CreateMoveHook);

        *(uint64_t *)hooks.getClassClientModeShared().g_pClientMode = (uint64_t)vmt_hook_client.VMTGetVTableCopy();
    }

    void ThreadHooks()
    {
        sleep(TIMEOUT); // wait csgo load is libraries

        std::cout << "[*] Starting Hooks" << std::endl;

        ClientModeSharedHook();
    }

    void LauncherMain(int argc, const char **argv)
    {
        std::cout << "[*] Loading LauncherClient.so" << std::endl;

        void *dl = dlopen(BACKUP_LAUNCHER, RTLD_NOW);
        if (dl)
        {
            void *LauncherMain = dlsym(dl, "LauncherMain");
            if (LauncherMain)
            {
                static void (*LauncherMain_o)(int argc, const char **argv) = reinterpret_cast<void (*)(int argc, const char **argv)>(LauncherMain);
                std::thread hooks_thread(ThreadHooks);

                LauncherMain_o(argc, argv);
            }

            dlclose(dl);
        }
        else
        {
            throw std::runtime_error("[!] Erro in open library : " + std::string(BACKUP_LAUNCHER));
        }
    }
}
