#include "RenameCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

RenameCommand::RenameCommand() : IMCCommand("rename", "Rename held item", "<name>") {
}

RenameCommand::~RenameCommand() {
}

std::string replaceLines(const std::string& s, const std::string& findS, const std::string& replaceS) {
	std::string result = s;
	auto pos = s.find(findS);
	if (pos == std::string::npos) {
		return result;
	}
	result.replace(pos, findS.length(), replaceS);
	return replaceLines(result, findS, replaceS);
}

bool RenameCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();
	ItemStack* item = Game.getLocalPlayer()->getSelectedItem();

	std::string name;

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}

	name = replaceLines(os.str(), "\\n", "\n");

	if (!item->item) {
		clientMessageF("%s%s", RED, "Error, no item held.");
		return false;
	}

	{
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
		manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
	}

	item->setCustomName(name);

	{
		manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
		inv->setItem(supplies->selectedHotbarSlot, *item);
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
	}

	clientMessageF("%s%s", GREEN, "Successfully Renamed!");
	return true;
}