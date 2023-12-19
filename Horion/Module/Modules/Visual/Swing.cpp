#include "Swing.h"
#include "../../../../Utils/Logger.h"
#include "../../../../Utils/Utils.h"

Swing::Swing() : IModule(0, Category::VISUAL, "Swing animations") {
	registerEnumSetting("Swings", &selectedSwing, 0);
	selectedSwing.addEntry(EnumEntry("Flux", 0));
	selectedSwing.addEntry(EnumEntry("Punch", 1));
}

Swing::~Swing() {
}

const char* Swing::getModuleName() {
	return ("Swing");
}

void* fluxTargetAddress = (void*)(FindSignature("E8 ? ? ? ? F3 44 0F 59 35 ? ? ? ? F3 0F 10 15"));
char ogFlux[5] = {};

void* punchTargetAddress = (void*)(FindSignature("48 89 43 ? 48 8B 5C 24 ? 48 83 C4 ? 5F C3 CC CC CC CC 48 89 5C 24 ? 57 48 83 EC ? 48 8B 42 ? 48 8B DA 0F 29 74 24 ? 48 8B F9 0F 29 7C 24 ? 0F 57 FF 48 85 C0 74 ? F3 0F 10 70"));
char ogPunch[5] = {};

void Swing::onEnable() {
	Utils::copyBytes(fluxTargetAddress, ogFlux, 5);
	Utils::copyBytes(punchTargetAddress, ogPunch, 5);
}

void Swing::onTick(GameMode* gm) {
	if (!fluxTargetAddress || !punchTargetAddress) return;

	switch (selectedSwing.selected) {
	case 0:
		Utils::nopBytes(fluxTargetAddress, 5);
		Utils::patchBytes(punchTargetAddress, ogPunch, 4);
		break;
	case 1:
		Utils::nopBytes(punchTargetAddress, 4);
		Utils::patchBytes(fluxTargetAddress, ogFlux, 5);
		break;
	}
}

void Swing::onDisable() {
	Utils::patchBytes(fluxTargetAddress, ogFlux, 5);
	Utils::patchBytes(punchTargetAddress, ogPunch, 4);
}