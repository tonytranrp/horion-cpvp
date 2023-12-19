#include "SetLoreCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

SetLoreCommand::SetLoreCommand() : IMCCommand("setlore", "Adds lore to held item", "<lore>") {
}

SetLoreCommand::~SetLoreCommand() {
}

std::string replaceNewLines(const std::string& s, const std::string& findS, const std::string& replaceS) {
	std::string result = s;
	auto pos = s.find(findS);
	if (pos == std::string::npos) {
		return result;
	}
	result.replace(pos, findS.length(), replaceS);
	return replaceNewLines(result, findS, replaceS);
}

bool SetLoreCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();
	ItemStack* item = Game.getLocalPlayer()->getSelectedItem();

	std::string lore;

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}

	lore = replaceNewLines(os.str(), "\\n", "\n");

	if (!item->item) {
		clientMessageF("%s%s", RED, "Error, no item held.");
		return false;
	}

	{
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
		manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
	}

	item->setCustomLore(lore);

	{
		manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
		inv->setItem(supplies->selectedHotbarSlot, *item);
		manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
	}

	clientMessageF("%s%s", GREEN, "Successfully Set Item Lore!");
	return true;
}