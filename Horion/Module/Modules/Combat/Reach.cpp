#include "Reach.h"

#include "../../../../Utils/Logger.h"
#include "../../../../Utils/Utils.h"

Reach::Reach() : IModule(0, Category::COMBAT, "Increases your reach") {
	this->registerFloatSetting("Reach Value", &this->reachValue, this->reachValue, 3.f, 7.f);
}

Reach::~Reach() {
}

const char* Reach::getModuleName() {
	return ("Reach");
}

std::string Reach::getModeText() {
	char reachChar[50];
	sprintf_s(reachChar, "%.1f Blocks", reachValue);
	return reachChar;
}

void Reach::onTick(GameMode* gm) {
	if (reachPtr ) {
		*reachPtr = reachValue;
	}
}

void Reach::onEnable() {
	static uintptr_t sigOffset = 0x0;
	if (sigOffset == 0x0) {
		sigOffset = FindSignature("F3 0F 10 15 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 4C 24 ? 48");

		if (sigOffset ) {
			int offset = *reinterpret_cast<int*>((sigOffset + 4));  // Get Offset from code
			reachPtr = reinterpret_cast<float*>(sigOffset + offset + 8);
			originalReach = *reachPtr;
		}
	}
	if (!VirtualProtect(reachPtr, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef HORION_DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
}

void Reach::onDisable() {
	*reachPtr = originalReach;
	if (reachPtr )
		VirtualProtect(reachPtr, sizeof(float), oldProtect, &oldProtect);
}