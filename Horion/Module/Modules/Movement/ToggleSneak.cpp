#include "ToggleSneak.h"

ToggleSneak::ToggleSneak() : IModule(0x0, Category::MOVEMENT, "Automatically sneak without holding the key") {
}

ToggleSneak::~ToggleSneak() {
}

const char* ToggleSneak::getModuleName() {
	return ("ToggleSneak");
}

void ToggleSneak::onTick(GameMode* gm) {
	GameSettingsInput* input = Game.getGameSettingsInput();
	if (GameData::isKeyDown(*input->sneak.key) && !hasPressed) {
		isSneaking = !isSneaking;
		hasPressed = true;
	} else if (!GameData::isKeyDown(*input->sneak.key)) {
		hasPressed = false;
	}
	Game.getMoveInputHandler()->isSneakDown = isSneaking;
}