#include "AutoEat.h"


AutoEat::AutoEat() : IModule(0, Category::WORLD, "Eat food automatically!") {
	this->registerIntSetting("Hunger Bar", &this->hunger, this->hunger, 0, 19);
}

AutoEat::~AutoEat() {
}

const char* AutoEat::getModuleName() {
	return "AutoEat";
}

std::string AutoEat::getModeText() {
	return "Hunger: " + std::to_string(hunger);
}

void AutoEat::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;

	auto Player = Game.getLocalPlayer();
	auto PlayerHunger = Game.getLocalPlayer()->getAttribute(AttributeID::PlayerHunger)->currentValue;
	
	if (PlayerHunger <= hunger) {
		if (Player->getSelectedItem()->isValid() && Player->getSelectedItem()->getItem()->isFood()) {
			if (Player->remainingTicksUsingItem <= 0) {
				Game.getGameMode()->baseUseItem(*Player->getSelectedItem());
			}
		}
	}
}