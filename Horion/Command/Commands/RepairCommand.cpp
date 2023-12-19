#include "RepairCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

RepairCommand::RepairCommand() : IMCCommand("repair", "Repair held item", "") {
}

RepairCommand::~RepairCommand() {
}

bool RepairCommand::execute(std::vector<std::string>* args) {
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();
	ItemStack* item = Game.getLocalPlayer()->getSelectedItem();

	if (!item->isValid()) {
		clientMessageF("%s%s", RED, "Error, no item held.");
		return false;
	}

	{
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
		manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
	}

	item->getItem()->setDamageValue(*item, (short)0);

	{
		manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
		inv->setItem(supplies->selectedHotbarSlot, *item);
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
	}

	clientMessageF("%s%s", GREEN, "Successfully Repaired!");
	return true;
}