#include <dlfcn.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "config/config.hpp"
#include "ghooks/GHooks.hpp"

extern "C"
{
    static void (*LauncherMain_o)(int argc, const char **argv);

    void hooks_teste()
    {
        sleep(60);
        std::cout << "[*] Instaling Hooks ..." << std::endl;
        GHooks hooks;
        hooks.install();
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
                std::thread first(hooks_teste);
                LauncherMain_o(argc, argv);
            }
            dlclose(dl);
        }
    }
}