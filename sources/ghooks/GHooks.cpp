#include "ghooks/GHooks.hpp"
#include "interface/GHInterfaces.hpp"
#include "config/config.hpp"

#include <iostream>

GHooks::GHooks()
{
}

GHooks::~GHooks()
{
}

void GHooks::install()
{

	GHInterfaces inter(CLIENT_CLIENT, 0x010104C4);

	std::vector<uint8_t> pattern = {0x55, 0x89, 0xE5};
	std::vector<uint8_t> mask = {'?','?', '?'};
	inter.FindPattern(pattern, mask, 0x40000);

	void *ClientShared = inter.CreateInterfaceFN("16ClientModeShared");
}
