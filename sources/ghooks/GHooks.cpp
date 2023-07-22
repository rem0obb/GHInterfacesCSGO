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

GHooks::GHooks()
{
}

GHooks::~GHooks()
{
}

void *hook()
{
}

// Função de tratamento do sinal
void sigtrap_handler(int signal)
{
	if (signal == SIGTRAP)
	{
		std::cout << "[*] Received SIGTRAP (INT3) signal." << std::endl;

		uint64_t rax;
		asm("mov %%rax, %0"
			: "=a"(rax) // Lê o valor do registrador rax e atribui a g_pClientMode
			:
			:);

		void *g_pClientMode = (void*)rax;
		std::cout << "[*] Register RAX/g_pClientMode = " << std::hex << g_pClientMode << std::endl;

		uint64_t *vTable = *(uint64_t **)(g_pClientMode + 0x0); // get vtable class CHLClient
		uintptr_t CreateMove = vTable[24];						// get pointer for function CreateMove

		for (int i = 0; i < 10; i++)
		{
			printf("address=%p / %x\n", (CreateMove + i), *(unsigned char *)((CreateMove + i)));
		}

		while (true)
		{
		}
	}
}

void GHooks::Interface_VClient()
{
	GHInterfaces inter(CLIENT_CLIENT);
	void *vClient = inter.CreateInterfaceFN(CLIENT_DLL_INTERFACE_VERSION);

	uint64_t *vTable = *(uint64_t **)(vClient + 0x0); // get vtable class CHLClient
	uintptr_t HudProcessInput = vTable[10];			  // get pointer for function CHLClient::HudProcessInput

	long page_size = sysconf(_SC_PAGE_SIZE);

	if (mprotect(ALIGN_ADDR(HudProcessInput), page_size, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_NONE) != 0)
	{
		perror("Falha ao definir permissões de leitura, escrita é execução");
	}

	for (int i = 0; i < 3; i++)
	{
		printf("address=%p / %x\n", (HudProcessInput + 16) + i, *(unsigned char *)((HudProcessInput + 16) + i));
	}

	std::signal(SIGTRAP, sigtrap_handler);

	strncpy((char *)HudProcessInput + 16, "\xCC\x00\x00", 3); // insert signal int3 (breakpoint)
}