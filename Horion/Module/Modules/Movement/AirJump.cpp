#include "AirJump.h"

AirJump::AirJump() : IModule('G', Category::MOVEMENT, "Jump even you're not on the ground") {
	registerBoolSetting("Legacy", &legacyMode, legacyMode);
}

AirJump::~AirJump() {
}

const char* AirJump::getModuleName() {
	return ("AirJump");
}

std::string AirJump::getModeText() {
	if (legacyMode)
		return "Legacy";
	else
		return "Normal";
}

void AirJump::onTick(GameMode* gm) {
	if (legacyMode) {
		gm->player->setOnGround(true);
		return;
	}
	GameSettingsInput* input = Game.getGameSettingsInput();

	if (!input)
		return;

	if (GameData::isKeyDown(*input->spaceBar.key) && hasJumped == 0) {
		gm->player->setOnGround(true);
		hasJumped = 1;
	} else if (!GameData::isKeyDown(*input->spaceBar.key)) {
		hasJumped = 0;
	}
}