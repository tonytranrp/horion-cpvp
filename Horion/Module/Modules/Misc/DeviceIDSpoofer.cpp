#include "DeviceIDSpoofer.h"

DeviceIDSpoofer::DeviceIDSpoofer() : IModule(0, Category::MISC, "Generates random device and client IDs!") {}

const char* DeviceIDSpoofer::getModuleName() {
	return "DeviceIDSpoofer";
}

void DeviceIDSpoofer::onEnable() {
	if (Game.getLocalPlayer()) { // don't want this to print when joining a game (modules default enable on join)
		this->clientMessageF("%sDevice ID queued to randomly generate, relog to apply!", GREEN);
	}
}