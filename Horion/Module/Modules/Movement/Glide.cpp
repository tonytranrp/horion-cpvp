#include "Glide.h"

Glide::Glide() : IModule(0, Category::MOVEMENT, "Float down") {
	mode = (*new SettingEnum(this))
			   .addEntry(EnumEntry("Normal", 0))
			   .addEntry(EnumEntry("NoDown", 1));
	this->registerEnumSetting("Type", &mode, 0);
	this->registerFloatSetting("value", &this->glideMod, this->glideMod, -2, 1);
}

Glide::~Glide() {
}

const char* Glide::getModuleName() {
	if (isEnabled()) {
		static char modName[30];  // This is kinda ghetto rn, there should be a better way to make this...
		sprintf_s(modName, 30, "Glide [%.2f]", glideModEffective);
		return modName;
	} else
		return ("Glide");
}

void Glide::onTick(GameMode* gm) {
	glideModEffective = glideMod;
	GameSettingsInput* input = Game.getGameSettingsInput();

	if (mode.selected == 0) {
		if (Game.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBar.key))
				glideModEffective += 0.2f;
			if (GameData::isKeyDown(*input->sneak.key))
				glideModEffective -= 0.2f;
		}
		gm->player->stateVector->velocity.y = glideModEffective;
	}

	if (mode.selected == 1) {
		if (Game.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBar.key))
				glideModEffective += 0.2f;
			if (GameData::isKeyDown(*input->sneak.key))
				glideModEffective -= 0.2f;
		}
		gm->player->stateVector->velocity.y = 0;
	}
}

const char* Glide::getRawModuleName() {
	return "Glide";
}