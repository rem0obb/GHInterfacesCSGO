#include <stdint.h>

#pragma once

namespace CSGO
{
    class IClientEntity
    {
    public:
        // Delete yourself.
        virtual void Release(void) = 0;

        // Network origin + angles
        virtual const void *&GetAbsOrigin(void) const = 0;
        virtual const void *&GetAbsAngles(void) const = 0;

        virtual void **GetMouth(void) = 0;

        // Retrieve sound spatialization info for the specified sound on this entity
        // Return false to indicate sound is not audible
        virtual bool GetSoundSpatialization(void *&info) = 0;

        virtual bool IsBlurred(void) = 0;
        virtual void* GetModelName(void) const = 0;
    };

    class IClientEntityList
    {
    public:
        // Get IClientNetworkable interface for specified entity
        virtual void *GetClientNetworkable(int entnum) = 0;
        virtual void *GetClientNetworkableFromHandle(void *hEnt) = 0;
        virtual void *GetClientUnknownFromHandle(void *hEnt) = 0;

        // NOTE: This function is only a convenience wrapper.
        // It returns GetClientNetworkable( entnum )->GetIClientEntity().
        virtual IClientEntity *GetClientEntity(int entnum) = 0;
        virtual IClientEntity *GetClientEntityFromHandle(unsigned long *hEnt) = 0;

        // Returns number of entities currently in use
        virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;

        // Returns highest index actually used
        virtual int GetHighestEntityIndex(void) = 0;

        // Sizes entity list to specified size
        virtual void SetMaxEntities(int maxents) = 0;
        virtual int GetMaxEntities() = 0;
        virtual void *GetClientNetworkableArray() = 0;
    };
}