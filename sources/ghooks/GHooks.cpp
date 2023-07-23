#include "ghooks/GHooks.hpp"
#include "interface/GHInterfaces.hpp"
#include "config/config.hpp"

#include <iostream>
#include <malloc.h>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include <ucontext.h>
#include <libunwind.h>

GHooks::ClientModeShared clientModeShared;
GHooks::CHLClient chlClient;

GHooks::GHooks()
{
}

GHooks::~GHooks()
{
}

// Função de tratamento do sinal
void GetPointerClientModeSharedVTABLE(int sig, siginfo_t *info, void *ucontext)
{
	if (sig == SIGTRAP)
	{
		std::cout << "[*] Received SIGTRAP (INT3) signal." << std::endl;

		ucontext_t *context = static_cast<ucontext_t *>(ucontext);

		unsigned long long rax = context->uc_mcontext.gregs[REG_RAX];
		std::cout << "[*] RAX: 0x" << std::hex << rax << std::endl;

		uint64_t *vTable = *(uint64_t **)rax;  // get vtable class ClientModeShared

		std::cout << "[*] pClientMode: " << std::hex << vTable << std::endl;

		clientModeShared.CreateMove = vTable[25];

		strncpy((char *)chlClient.HudProcessInput + 35, "\xFF\xE2", 2); // recover bytes
	}
}

void GHooks::Interface_VClient()
{
	GHInterfaces inter(CLIENT_CLIENT);
	void *vClient = inter.CreateInterfaceFN(CLIENT_DLL_INTERFACE_VERSION);

	uint64_t *vTable = *(uint64_t **)(vClient + 0x0); // get vtable class CHLClient

	chlClient.HudProcessInput = vTable[10]; // get pointer for function CHLClient::HudProcessInput

	long page_size = sysconf(_SC_PAGE_SIZE);
	if (mprotect(ALIGN_ADDR(chlClient.HudProcessInput), page_size, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_NONE) != 0)
	{
		perror("[*] mprotect error ");
	}

	// Get Pointer pClientMode
	{
		struct sigaction sa;
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = GetPointerClientModeSharedVTABLE;
		sigemptyset(&sa.sa_mask);

		if (sigaction(SIGTRAP, &sa, nullptr) == -1)
		{
			perror("sigaction");
			return;
		}
	}

	strncpy((char *)chlClient.HudProcessInput + 35, "\xCC\x00", 2); // insert signal int3 (breakpoint)
}