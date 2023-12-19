#include "TriggerBot.h"

#include "../../../../Utils/Target.h"

TriggerBot::TriggerBot() : IModule(0, Category::COMBAT, "Attacks entities you're looking at") {
	registerBoolSetting("Include Mobs", &this->includeMobs, this->includeMobs);
	this->registerIntSetting("Delay", &this->delay, this->delay, 0, 20);
}

TriggerBot::~TriggerBot() {
}

const char* TriggerBot::getModuleName() {
	return ("TriggerBot");
}
void TriggerBot::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	Entity* target = Game.getLevel()->getLookingAtEntity();

	Odelay++;
	if (target != 0 && Odelay >= delay) {
		if (!Target::isValidTarget(target, this->includeMobs, false))
			return;

		localPlayer->swing();
		gm->attack(target);

		Odelay = 0;
	}
}