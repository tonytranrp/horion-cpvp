#include "AutoPot.h"

#include "../../../../Utils/Logger.h"

AutoPot::AutoPot() : IModule(0, Category::COMBAT, "Automatically throws instant health pots!") {
	registerIntSetting("Health", &health, 10, 1, 20);
	registerIntSetting("Pitch", &pitch, -90, -90, 90);
}

AutoPot::~AutoPot() {
}

const char* AutoPot::getModuleName() {
	return "AutoPot";
}

std::string AutoPot::getModeText() {
	return "HP: " + std::to_string(health);
}

bool AutoPot::hasPot() {
	auto supplies = Game.getSupplies();
	auto inv = Game.getInventory();
	for (int i = 0; i < 9; i++) {
		if (inv->getItemStack(i)->isSplashPotionOfInstantHealth()) {
			return true;
		}
	}
	return false;
}
void AutoPot::throwPot() {
	auto plr = Game.getLocalPlayer();
	auto supplies = Game.getSupplies();
	auto inv = Game.getInventory();

	Game.getGameMode()->baseUseItem(*inv->getItemStack(supplies->selectedHotbarSlot));
	rotate = false;
	potTimer = 20;
	throwCoolDown = plr->ticksAlive + 25;
	supplies->selectedHotbarSlot = prevSlot;
	prevSlot = 99;
}

void AutoPot::onTick(GameMode* gm) {

	auto plr = Game.getLocalPlayer();
	if (!plr) return;

	if (plr->ticksAlive == throwCoolDown) {
		shouldThrow = true;
	}

	auto supplies = plr->getSupplies();
	auto inv = Game.getInventory();
	int32_t currHealth = static_cast<int32_t>(std::round(plr->getHealth()));

	for (int i = 0; i < 9; i++) {
		if ((currHealth < health) && inv->getItemStack(i)->isSplashPotionOfInstantHealth() && shouldThrow) {
			if (prevSlot == 99) {
				prevSlot = supplies->selectedHotbarSlot;
			}
			supplies->selectedHotbarSlot = i;
			rotate = true;
		}
	}
}