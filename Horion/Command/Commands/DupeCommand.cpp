#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "<count> [offhand: bool]") {
	registerAlias("d");
}

DupeCommand::~DupeCommand() {
}

bool DupeCommand::execute(std::vector<std::string>* args) {
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	auto transactionManager = Game.getTransactionManager();

	int selectedSlot = supplies->selectedHotbarSlot;
	ItemStack* item = inv->getItemStack(selectedSlot);

	int count = item->count;
	bool offhand = false;

	if (args->size() > 1)
		item->count = assertInt(args->at(1));
	if (args->size() > 2)
		offhand = args->at(2) == "true";

	if (!offhand) {
		int slot = inv->getFirstEmptySlot();

		transactionManager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
		transactionManager->addAction(InventoryAction(slot, nullptr, item));

		inv->addItemToFirstEmptySlot(item);
	} else {
		transactionManager->addAction(InventoryAction(0, nullptr, item, ContainerID::Offhand));
		Game.getLocalPlayer()->setOffhandSlot(item);
		transactionManager->addAction(InventoryAction(0, item, nullptr, ContainerID::Offhand, InventorySource::Type::NotImplemented));
		item->count = count + 1;
		Game.getLocalPlayer()->setOffhandSlot(item);
		item->count -= 1;
	}

	clientMessageF("%sSuccessfully duplicated the item!", GREEN);
	return true;
}