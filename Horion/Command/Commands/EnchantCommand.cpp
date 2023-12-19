#include "EnchantCommand.h"

#include "../../../Utils/Utils.h"

EnchantCommand::EnchantCommand() :
	IMCCommand("enchant", "Enchants items",
		"\n1) <add> <enchantment: string> [level: int] [overrideDuplicates: bool]\n2) <remove> <enchantment: string>") {
}

bool EnchantCommand::execute(std::vector<std::string>* args) {

	size_t argSize = args->size();
	assertTrue(argSize >= 3);
	assertTrue(args->at(1) == "add" || args->at(1) == "remove");

	auto supplies = Game.getSupplies();
	auto inv = Game.getInventory();
	auto manager = Game.getTransactionManager();
	auto item = inv->getItemStack(supplies->selectedHotbarSlot);

	if (!item || !item->isValid()) {
		this->clientMessageF("%sSelected item is empty!", RED);
		return true;
	}

	EnchantType enchantId = EnchantType::INVALID;
	int enchantLevel = 32767;

	if (args->at(2) != "all") {
		try {
			// convert string to back to lower case
			std::string data = args->at(2);
			std::transform(data.begin(), data.end(), data.begin(), ::tolower);

			auto convertedString = EnchantUtils::IdMap.find(data);
			if (convertedString != EnchantUtils::IdMap.end()) {
				enchantId = static_cast<EnchantType>(convertedString->second);
			}
			else {
				enchantId = static_cast<EnchantType>(assertInt(args->at(2)));
			}
		}
		catch (...) {
			this->clientMessageF("Exception caught while trying to get enchant string!");
			enchantId = static_cast<EnchantType>(assertInt(args->at(2)));
		}
	}

	manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
	manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));

	if (args->at(1) == "add") {

		if (argSize >= 4) {
			enchantLevel = assertInt(args->at(3));
		}

		bool overrideDuplicates = true;
		if (argSize >= 5) {
			overrideDuplicates = args->at(4) != "false";
		}

		if (args->at(2) == "all") {
			for (int8_t i = 0; i < static_cast<int8_t>(EnchantType::COUNT); i++) {
				EnchantUtils::applyUnfilteredEnchant(*item, {static_cast<EnchantType>(i), enchantLevel}, overrideDuplicates);
			}

			this->clientMessageF(
				"%sSuccessfully applied all enchants of level %d to the selected item!", GREEN, enchantLevel);
		} 
		else {
			bool result = EnchantUtils::applyUnfilteredEnchant(*item, {enchantId, enchantLevel}, overrideDuplicates);
			if (result) {
				this->clientMessageF(
					"%sSuccessfully applied the enchant of id %d and level %d to the selected item!",
					GREEN, enchantId, enchantLevel);
			}
			else {
				this->clientMessageF(
					"%sFailed to apply the enchant of id %d and level %d to the selected item!",
					RED, enchantId, enchantLevel);
			}
		}
	} else if (args->at(1) == "remove") {

		if (args->at(2) == "all") {
			EnchantUtils::removeAllEnchants(*item);
			this->clientMessageF(
				"%sSuccessfully removed all enchants from the selected item!", GREEN);
		}
		else {
			bool result = EnchantUtils::removeEnchant(*item, enchantId);
			if (result) {
				this->clientMessageF(
					"%sSuccessfully removed all enchants of id %d from the selected item!", GREEN, enchantId);
			}
			else {
				this->clientMessageF(
					"%sFailed to remove all enchants of id %d from the selected item!", RED, enchantId);
			}
		}
	}

	manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
	manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
	manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));

	return true;
}