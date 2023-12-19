#include "FullBright.h"

FullBright::FullBright() : IModule(0, Category::VISUAL, "Puts your gamma to max") {
	registerFloatSetting("Intensity", &this->intensity, this->intensity, 0.f, 12.f);
}

FullBright::~FullBright() {
}

float originalGamma = -1;

const char* FullBright::getModuleName() {
	return "Fullbright";
}

void FullBright::onTick(GameMode* gm) {
	if (gammaPtr && *gammaPtr != 10)
		*gammaPtr = 10;
}

void FullBright::onEnable() {
	if (gammaPtr) {
		originalGamma = *gammaPtr;
		*gammaPtr = 10;
	}
}

void FullBright::onDisable() {
	if (gammaPtr) {
		if (originalGamma >= 0 && originalGamma <= 1)
			*gammaPtr = originalGamma;
		else
			*gammaPtr = 0.5f;
	}
}