#include "Inventory.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"
void Inventory::dropSlot(int slot, bool deleteItems, bool dropAll, bool randomize) {
	// FillingContainer::dropSlot
	using drop_t = void(__fastcall*)(Inventory*, int, bool, bool, bool);
	static drop_t func = reinterpret_cast<drop_t>(FindSignature("85 D2 0F 88 ?? ?? ?? ?? 48 89 5C 24 ?? 55 56 57 41 54"));
	if (func )
		func(this, slot, deleteItems, dropAll, randomize);
}
void Inventory::dropAll(int slot) {
	// FillingContainer::dropAll
	using dropAll_t = void(__fastcall*)(Inventory*, int, int, char);
	static dropAll_t func = reinterpret_cast<dropAll_t>(FindSignature("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 80 7C 24 60 ?? 41 0F B6 F1 8B ?? 48 8B F9 75 72"));
	if (func )
		func(this, slot, 0, 0);
}
bool Inventory::isFull() {
	int fullslots = 0;
	for (int i = 0; i < 36; i++) {
		if (this->getItemStack(i)->item)
			fullslots++;
	}
	if (fullslots == 36) return true;
	return false;
}

void ScreenController::handleAutoPlace(uintptr_t a1, std::string name, int slot) {
	using ScreenController__autoPlace = __int64(__fastcall*)(ScreenController*, uintptr_t, std::string, int);
	static ScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ScreenController__autoPlace>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 45 8B E1 4D 8B F0"));

	if (autoPlaceFunc )
		autoPlaceFunc(this, a1, name, slot);
}

void ScreenController::handleSwap(std::string name1, int slot1, std::string name2, int slot2) {
	using ScreenController__swap = __int64(__fastcall*)(ScreenController*, std::string, int, std::string, int);
	static ScreenController__swap swapFunc = reinterpret_cast<ScreenController__swap>(FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 41 54 41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 49 8B E9 45 8B F0 4C 8B FA 48 8B F1"));

	if (swapFunc )
		swapFunc(this, name1, slot1, name2, slot2);
}

void ScreenController::handleTakeAll(std::string name, int slot) {
	using ScreenController__takeAll = __int64(__fastcall*)(ScreenController*, std::string, int);
	static ScreenController__takeAll takeAllFunc = reinterpret_cast<ScreenController__takeAll>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 41 8B E8 4C 8B F2 48 8B F1 48 8B 01 48 8B 80"));

	if (takeAllFunc )
		takeAllFunc(this, name, slot);
}

void ScreenController::handlePlaceAll(std::string name, int slot) {
	using ScreenController__placeAll = __int64(__fastcall*)(ScreenController*, std::string, int);
	static ScreenController__placeAll placeAllFunc = reinterpret_cast<ScreenController__placeAll>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B E8 4C 8B F2 48 8B F1 48 8B 01 48 8B 80 ? ? ? ? ? ? ? ? ? ? ? ? 0F 84 ? ? ? ? 83 BE 3C"));

	if (placeAllFunc )
		placeAllFunc(this, name, slot);
}

void ScreenController::sendFlyingItem(ItemStack* item, std::string name1, int slot1, std::string name2, int slot2) {
	using ScreenController__flyingItem = __int64(__fastcall*)(ScreenController*, ItemStack*, std::string, int, std::string, int);
	static ScreenController__flyingItem flyingItemFunc = reinterpret_cast<ScreenController__flyingItem>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 8B F1 49 8B F8"));

	if (flyingItemFunc )
		flyingItemFunc(this, item, name1, slot1, name2, slot2);
}

ItemStack* ScreenController::getContainerItem(int slot) {
	return this->_getItemStack(std::string("container_items"), slot);
}

void ScreenController::takeContainerItem(int slot) {
	this->handleAutoPlace(0x7FFFFFFF, "container_items", slot);
}

void Inventory::moveItem(int from, int to = -1) {
	InventoryTransactionManager* manager = Game.getTransactionManager();

	if (to < 0) {
		to = getFirstEmptySlot();
	}

	ItemStack* item1 = getItemStack(from);
	ItemStack* item2 = getItemStack(to);

	if (!item1->item) return;

	if (!item2->item) {
		InventoryAction first(from, item1, nullptr);
		InventoryAction second(to, nullptr, item1);

		manager->addAction(first);
		manager->addAction(second);

		ItemStack item1Copy{*item1};

		*item2 = std::move(item1Copy);
		*item1 = ItemStack::getEmptyItemStack();

		manager->addAction(second);
	}
	else {
		InventoryAction first(from, item1, nullptr);
		InventoryAction second(to, item2, item1);
		InventoryAction third(from, nullptr, item2);

		manager->addAction(first);
		manager->addAction(second);
		manager->addAction(third);

		ItemStack item1Copy{*item1}, item2Copy{*item2};

		*item2 = std::move(item1Copy);
		*item1 = std::move(item2Copy);

		manager->addAction(third);
	}
}

void Inventory::swapSlots(int from, int to) {
	InventoryTransactionManager* manager = Game.getTransactionManager();

	ItemStack* i1 = getItemStack(from);
	ItemStack* i2 = getItemStack(to);

	ItemStack i1Copy{*i1}, i2Copy{*i2};

	InventoryAction first(from, i1, nullptr);
	InventoryAction second(to, i2, i1);
	InventoryAction third(from, nullptr, i2);

	manager->addAction(first);
	manager->addAction(second);
	manager->addAction(third);

	*i2 = std::move(i1Copy);
	*i1 = std::move(i2Copy);

	manager->addAction(third);
}
