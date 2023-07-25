#include <dlfcn.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "config/config.hpp"
#include "ghooks/GHooks.hpp"

extern "C"
{
    static void (*LauncherMain_o)(int argc, const char **argv);
    static bool (*CreateMove)(void *_this, float flInputSampleTime, void *cmd);
    static bool CreateMoveHook(void *_this, float flInputSampleTime, void *cmd);

    void ThreadHooks()
    {
        sleep(TIMEOUT); // wait csgo load is libraries

        std::cout << "[*] Instaling Hooks ..." << std::endl;
        GHooks hooks;
        hooks.Start();

        sleep(10);
        CreateMove = reinterpret_cast<bool (*)(void *_this, float flInputSampleTime, void *cmd)>(hooks.getClassClientModeShared().CreateMove);
        hooks.getClassClientModeShared().vTable[25] = (uint64_t)&CreateMoveHook;
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
                std::thread first(ThreadHooks);

                LauncherMain_o(argc, argv);
            }
            dlclose(dl);
        }
    }

    bool CreateMoveHook(void *_this, float flInputSampleTime, void *cmd)
    {
        std::cout << "[*] Hooked Function CreateMove" << std::endl;
        return CreateMove(_this, flInputSampleTime, cmd);
    }
}