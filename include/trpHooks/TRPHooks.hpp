#pragma once

#include <sys/mman.h>
#include <sys/errno.h>

class TRPHooks 
{
    private:    

    public:
        TRPHooks();
        ~TRPHooks();

        bool TRP_Function(off_t* _address_function, off_t* _address_function_hook, size_t _len);
};