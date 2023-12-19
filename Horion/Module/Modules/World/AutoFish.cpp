#include "AutoFish.h"

AutoFish::AutoFish() : IModule(0, Category::WORLD, "Go fishing automatically!") {
	registerBoolSetting("Auto Rod", &this->autoRod, this->autoRod);
}

AutoFish::~AutoFish() {
}

const char* AutoFish::getModuleName() {
	return "AutoFish";
}

void AutoFish::onEnable() {
	shouldFish = true;
}

void AutoFish::onTick(GameMode* gm) {
	LocalPlayer* plr = Game.getLocalPlayer();
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
	if (autoRod) {
		for (int i = 0; i < 9; i++) {
			if (inv->getItemStack(i)->isValid() && inv->getItemStack(i)->getItem()->name.find("fishing_rod") != std::string::npos) {
				supplies->selectedHotbarSlot = i;
				break;//gets the first rod rather than the last
			}
		}
	}
	static int Odelay = 0;
	if (reelRod) {
		gm->baseUseItem(*stack);
		reelRod = false;
		Odelay = 0;
	}
	if (++Odelay > 6) {
		if (shouldFish) {
			//for some reason mojang keeps changing the id of the fishing rod, so it would be better if we just looked for the name instead of the id
			if (stack->isValid() && stack->getItem()->name.find("fishing_rod") != std::string::npos) {
				if (stack->getDurability() <= 10) {
					clientMessageF("Error, Fishing Rod Durability is Low.");
					this->setEnabled(false);
				} else {
					shouldFish = false;
					gm->baseUseItem(*stack);
					gm->releaseUsingItem();
				}
			} else {
				clientMessageF("Error, Please Hold a Fishing Rod");
				this->setEnabled(false);
			}
		}
		Odelay = 0;
	}
}

void AutoFish::onDisable() {
	shouldFish = false;
}