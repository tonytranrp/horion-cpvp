#include "BowSpam.h"

BowSpam::BowSpam() : IModule(0, Category::COMBAT, "Spam arrows") {
	this->registerIntSetting("Pullback Delay", &this->Pdelay, this->Pdelay, 3, 32);
	registerBoolSetting("Facing Entity", &PointingAtPlayer, PointingAtPlayer);
}

BowSpam::~BowSpam() {
}

const char* BowSpam::getModuleName() {
	return "BowSpam";
}

void BowSpam::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;

	auto Player = Game.getLocalPlayer();

	static int Odelay = 0;
	if (Player->getSelectedItem()->isValid()) {
		if (Player->getSelectedItem()->getItem()->itemId == 300 || Player->getSelectedItem()->getItem()->itemId == 575 && Pdelay >= 12) {
			auto target = Game.getLevel()->getLookingAtEntity();

			if (Game.getLevel()->isLookingAtEntity() && target->getEntityTypeId() == 63 && PointingAtPlayer || !PointingAtPlayer) {
				if (Player->remainingTicksUsingItem <= 0) {
					Game.getGameMode()->baseUseItem(*Player->getSelectedItem());
				}
				if (++Odelay > Pdelay) {
					Game.getGameMode()->releaseUsingItem();
					Odelay = 0;
				}
			}
		}
	}
}
