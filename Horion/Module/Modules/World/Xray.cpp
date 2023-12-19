#include "Xray.h"

Xray::Xray() : IModule(0, Category::WORLD, "Allows you to see certain blocks easier") {
}

Xray::~Xray() {
}

const char* Xray::getModuleName() {
	return ("Xray");
}

void Xray::onEnable() {
	if (!Game.getLocalPlayer())
		this->setEnabled(false);
	else
		Game.getLevelRenderer()->reloadChunks();
}

void Xray::onTick(GameMode* gm) {
	auto smoothLightOption = Game.getClientInstance()->getOptions()->getOptionByTranslatedName("options.smoothlighting");
	if (smoothLightOption)
		smoothLightningSetting = (bool*)&smoothLightOption->valueI;

	if (smoothLightningSetting) {
		if (!gotSmoothInfo) {
			gotSmoothInfo = true;
			wasSmooth = *smoothLightningSetting;
		}
		*smoothLightningSetting = 0;
	}
}

void Xray::onDisable() {
	if (Game.getLocalPlayer()) {
		Game.getLevelRenderer()->reloadChunks();

		if (smoothLightningSetting && gotSmoothInfo) {
			*smoothLightningSetting = wasSmooth;
		}
		gotSmoothInfo = false;
	}
}