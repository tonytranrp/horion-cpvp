#include "FastEat.h"

FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instant") {
registerIntSetting("Duration", &this->DurationSpeed, this->DurationSpeed, 1, 32);
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

void FastEat::onTick(GameMode* gm) {
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL)
		if ((*stack->item)->isFood() || (*stack->item)->itemId == 426) {  //potion id = 426
			(*stack->item)->setMaxUseDuration(DurationSpeed);
		}
	}
}

void FastEat::onDisable() {
	if (!Game.getLocalPlayer())
		return;
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL)
		if ((*stack->item)->isFood() || (*stack->item)->itemId == 426) {
			(*stack->item)->setMaxUseDuration(32);
		}
	}
}
