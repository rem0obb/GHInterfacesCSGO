#include "ghooks/GHooks.hpp"
#include "interface/GHInterfaces.hpp"
#include "config/config.hpp"

#include <iostream>
#include <csignal>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>
#include <sys/mman.h>

// class
GHooks::ClientModeShared clientModeShared;
GHooks::CHLClient chlClient;

GHooks::GHooks()
{
}

GHooks::~GHooks()
{
}

// Função de tratamento do sinal
void GetClientModeSharedVTABLE(int sig, siginfo_t *info, void *ucontext)
{
	if (sig == SIGTRAP)
	{
		std::cout << "[*] Received SIGTRAP (INT3) signal." << std::endl;

		ucontext_t *context = static_cast<ucontext_t *>(ucontext);

		unsigned long long rax = context->uc_mcontext.gregs[REG_RAX];
		std::cout << "[*] RAX: 0x" << std::hex << rax << std::endl;

		clientModeShared.vTable = *(uint64_t **)rax; // get vtable class ClientModeShared

		long page_size = sysconf(_SC_PAGE_SIZE);
		if (mprotect(ALIGN_ADDR(clientModeShared.vTable), page_size, PROT_READ | PROT_WRITE | PROT_NONE) != 0)
		{
			perror("[*] mprotect error ");
		}

		// get methods
		clientModeShared.CreateMove = clientModeShared.vTable[25];

		strncpy((char *)chlClient.HudProcessInput + 35, "\xFF\xE2", 2); // recover bytes
	}
}

void GHooks::Start()
{
	GHInterfaces inter(CLIENT_CLIENT);
	void *vClient = inter.CreateInterfaceFN(CLIENT_DLL_INTERFACE_VERSION);


	chlClient.vTable = *(uint64_t **)(vClient + 0x0); // get vtable class CHLClient
	// get methods
	chlClient.HudProcessInput = chlClient.vTable[10]; // get pointer for function CHLClient::HudProcessInput

	// adjustment permission in memory for write
	long page_size = sysconf(_SC_PAGE_SIZE);
	if (mprotect(ALIGN_ADDR(chlClient.HudProcessInput), page_size, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_NONE) != 0)
	{
		perror("[*] mprotect error ");
	}

	// Get Pointer pClientMode
	{
		struct sigaction sa;
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = GetClientModeSharedVTABLE;
		sigemptyset(&sa.sa_mask);

		if (sigaction(SIGTRAP, &sa, nullptr) == -1)
		{
			perror("[*] sigaction error ");
			return;
		}
	}

	// insert signal int3 (breakpoint)
	strncpy((char *)chlClient.HudProcessInput + 35, "\xCC\x00", 2);
}

GHooks::ClientModeShared GHooks::getClassClientModeShared()
{
	return clientModeShared;
}

GHooks::CHLClient GHooks::getClassCHLClient()
{
	return chlClient;
}