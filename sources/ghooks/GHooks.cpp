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

uintptr_t HudProcessInput;

GHooks::GHooks()
{
}

GHooks::~GHooks()
{
}

// Função de tratamento do sinal
void sigtrap_handler(int sig, siginfo_t *info, void *ucontext)
{
	if (sig == SIGTRAP)
	{
		std::cout << "[*] Received SIGTRAP (INT3) signal." << std::endl;

		ucontext_t *context = static_cast<ucontext_t *>(ucontext);

		unsigned long long rax = context->uc_mcontext.gregs[REG_RAX];
		std::cout << "[*] RAX: 0x" << std::hex << rax << std::endl;

		uint64_t *vTable = *(uint64_t **)rax;

		std::cout << "[*] pClientMode: " << std::hex << vTable << std::endl;

		for (int i = 0; i < 10; i++)
		{
			printf("address=%p / %x\n", (vTable[25]) + i, *(unsigned char *)((vTable[25]) + i));
		}


		strncpy((char *)HudProcessInput + 35, "\xFF\xE2", 2); // recover bytes
	}
}

void GHooks::Interface_VClient()
{
	GHInterfaces inter(CLIENT_CLIENT);
	void *vClient = inter.CreateInterfaceFN(CLIENT_DLL_INTERFACE_VERSION);

	uint64_t *vTable = *(uint64_t **)(vClient + 0x0); // get vtable class CHLClient
	HudProcessInput = vTable[10];					  // get pointer for function CHLClient::HudProcessInput

	long page_size = sysconf(_SC_PAGE_SIZE);

	if (mprotect(ALIGN_ADDR(HudProcessInput), page_size, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_NONE) != 0)
	{
		perror("[*] mprotect error ");
	}

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigtrap_handler;
	sigemptyset(&sa.sa_mask);

	// Configura o tratador de sinal SIGTRAP
	if (sigaction(SIGTRAP, &sa, nullptr) == -1)
	{
		perror("sigaction");
		return;
	}

	strncpy((char *)HudProcessInput + 35, "\xCC\x00", 2); // insert signal int3 (breakpoint)
}