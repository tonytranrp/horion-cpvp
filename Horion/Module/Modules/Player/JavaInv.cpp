#include "JavaInv.h"

JavaInv::JavaInv() : IModule(0, Category::PLAYER, "Java Inventory Keybinds") {
	registerBoolSetting("Custom Binds", &this->usePlayerKeybinds, this->usePlayerKeybinds);
	registerBoolSetting("Item Animation", &this->animated, this->animated);
}

JavaInv::~JavaInv() {
}

const char* JavaInv::getModuleName() {
	return ("JavaInv");
}

void JavaInv::onTick(GameMode* gm) {
	GameSettingsInput* input = Game.getGameSettingsInput();
	key1 = usePlayerKeybinds ? *input->hotbar1.key : 49;
	key2 = usePlayerKeybinds ? *input->hotbar2.key : 50;
	key3 = usePlayerKeybinds ? *input->hotbar3.key : 51;
	key4 = usePlayerKeybinds ? *input->hotbar4.key : 52;
	key5 = usePlayerKeybinds ? *input->hotbar5.key : 53;
	key6 = usePlayerKeybinds ? *input->hotbar6.key : 54;
	key7 = usePlayerKeybinds ? *input->hotbar7.key : 55;
	key8 = usePlayerKeybinds ? *input->hotbar8.key : 56;
	key9 = usePlayerKeybinds ? *input->hotbar9.key : 57;

	std::vector<int> keyList = {key1, key2, key3, key4, key5, key6, key7, key8, key9};
	for (int key : keyList) {
		if (key == -99 || key == -98 || key == -97 || key == -95 || key == -94)
			continue;
		if (GameData::isKeyDown(key)) {
			if (key == key1) slot = 0;
			if (key == key2) slot = 1;
			if (key == key3) slot = 2;
			if (key == key4) slot = 3;
			if (key == key5) slot = 4;
			if (key == key6) slot = 5;
			if (key == key7) slot = 6;
			if (key == key8) slot = 7;
			if (key == key9) slot = 8;

			if (timer == 0 && !GameData::canUseMoveKeys())
				shouldSwitch = true;
		}
	}
	if (timer > 0)
		timer--;
}

void JavaInv::ScreenController_tick(ScreenController* c) {
	if (shouldSwitch && c->isInventory()) {
		if (strcmp(screenName.c_str(), "hotbar_items") == 0 || strcmp(screenName.c_str(), "inventory_items") == 0 || strcmp(screenName.c_str(), "container_items") == 0 || strcmp(screenName.c_str(), "armor_items") == 0 || strcmp(screenName.c_str(), "offhand_items") == 0) {
			ItemStack* hoveredItem = c->_getItemStack(std::string(screenName), hoverSlot);
			ItemStack* hotbarItem = c->_getItemStack(std::string("hotbar_items"), slot);
			if (hoveredItem->item && hotbarItem->item) {
				if (animated)
					c->handleSwap("hotbar_items", slot, screenName, hoverSlot);
				else {
					c->handleTakeAll("hotbar_items", slot);
					c->handlePlaceAll(screenName, hoverSlot);
					c->handlePlaceAll("hotbar_items", slot);
				}
			} else if (hoveredItem->item && !hotbarItem->item) {
				if (animated)
					c->sendFlyingItem(hoveredItem, screenName, hoverSlot, "hotbar_items", slot);
				c->handleTakeAll(screenName, hoverSlot);
				c->handlePlaceAll("hotbar_items", slot);
			} else if (!hoveredItem->item && hotbarItem->item) {
				if (animated)
					c->sendFlyingItem(hotbarItem, "hotbar_items", slot, screenName, hoverSlot);
				c->handleTakeAll("hotbar_items", slot);
				c->handlePlaceAll(screenName, hoverSlot);
			}
			timer = 3;
			shouldSwitch = false;
		} else
			shouldSwitch = false;
	}
}