#include "StackableItem.h"

StackableItem::StackableItem() : IModule(0, Category::PLAYER, "Stack items you normally wouldn't be able to stack (e. g. swords)") {
}

StackableItem::~StackableItem() {
}

const char* StackableItem::getModuleName() {
	return ("StackableItem");
}

void StackableItem::onTick(GameMode* gm) {
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* a = Game.getInventory();
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = a->getItemStack(i);
		if (stack && stack->isValid()) {
			Item* item = *stack->item;
			item->setStackedByData(true);
			item->setMaxStackSize(64);
		}
	}
}