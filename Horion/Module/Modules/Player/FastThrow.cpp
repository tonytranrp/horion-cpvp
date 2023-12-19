#include "FastThrow.h"

FastThrow::FastThrow() : IModule(0, Category::PLAYER, "Throw certain items fast") {
	registerFloatSetting("delay", &this->delay, this->delay, 0.f, 0.1f);
}

FastThrow::~FastThrow() {
}

const char* FastThrow::getModuleName() {
	return "FastThrow";
}

void FastThrow::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
	if (stack->isValid() && stack->getItem()->isThrowable() && GameData::isRightClickDown() && GameData::canUseMoveKeys()) {
		Odelay++;
		if (Odelay > delay * 20) {
			gm->baseUseItem(*stack); 
			Odelay = 0;
		}
	} else {
		Odelay = 0;
	}
}