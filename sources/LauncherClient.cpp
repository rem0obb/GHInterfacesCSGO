#include <dlfcn.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "config/config.hpp"
#include "ghooks/GHooks.hpp"
#include "internals/usercmd.hpp"
#include "internals/in_buttons.hpp"

extern "C"
{
    static void (*LauncherMain_o)(int argc, const char **argv);
    static bool (*CreateMove)(void *_this, float flInputSampleTime, CUserCmd *cmd);
    static bool CreateMoveHook(void *_this, float flInputSampleTime, CUserCmd *cmd);

    GHooks hooks;

    void ThreadHooks()
    {
        sleep(TIMEOUT); // wait csgo load is libraries

        std::cout << "[*] Installing Hooks ..." << std::endl;
        hooks.Start();

        sleep(10);

        // Hooks
        CreateMove = reinterpret_cast<bool (*)(void *_this, float flInputSampleTime, CUserCmd *cmd)>(hooks.getClassClientModeShared().CreateMove);
        hooks.getClassClientModeShared().vTable[25] = (uint64_t)&CreateMoveHook;
        
        // call only at the end of the inject hook
        hooks.ReadjustPages();
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
                LauncherMain_o = reinterpret_cast<void (*)(int argc, const char **argv)>(LauncherMain);
                std::thread hooks_thread(ThreadHooks);

                LauncherMain_o(argc, argv);
            }

            dlclose(dl);
        }else 
        {
            throw std::runtime_error("[*] Erro in open library : " + std::string(BACKUP_LAUNCHER));
        }
    }

    bool CreateMoveHook(void *_this, float flInputSampleTime, CUserCmd *cmd)
    {
        std::cout << "[*] Hooked Function CreateMove" << std::endl
                  << "Cmd.buttons = " << cmd->buttons << std::endl
                  << "Cmd.mousedy = " << cmd->mousedy << std::endl
                  << "Cmd.mousedx = " << cmd->mousedx << std::endl;

        return CreateMove(_this, flInputSampleTime, cmd);
    }
}
