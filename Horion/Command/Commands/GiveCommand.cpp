#include "GiveCommand.h"

#include "../../../SDK/Mojangson.h"
#include "../../../Utils/Utils.h"

GiveCommand::GiveCommand() : IMCCommand("give", "spawn items", "<itemName> <count> [itemData] [NBT]") {
}

GiveCommand::~GiveCommand() {
}

bool GiveCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 2);

	int itemId = 0;
	char count = static_cast<char>(assertInt(args->at(2)));
	char itemData = 0;
	if (args->size() > 3)
		itemData = static_cast<char>(assertInt(args->at(3)));
	try {
		itemId = std::stoi(args->at(1));
	} catch (const std::invalid_argument&) {
	}

	Inventory* inv = Game.getInventory();
	ItemStack* yot = nullptr;
	auto transactionManager = Game.getTransactionManager();

	if (itemId == 0) {
		Item* cStack = Game.getItemRegistry()->lookUpByName(args->at(1));
		if (!cStack) {
			clientMessageF("%sInvalid item name!", RED);
			return true;
		}
		yot = new ItemStack(*cStack, count, itemData);
	} else {
		Item* cStack = Game.getItemRegistry()->getItemFromId(itemId);
		if (!cStack) {
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		yot = new ItemStack(*cStack, count, itemData);
	}

	if (yot)
		yot->count = count;

	int slot = inv->getFirstEmptySlot();

	if (args->size() > 4) {
		std::string tag = Utils::getClipboardText();
		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START && tag.back() == MojangsonToken::COMPOUND_END) {
			try {
				yot->setUserData(std::move(Mojangson::Parse(tag)));
			} catch (Mojangson::Exception& e) {
				clientMessageF("%sError reading NBT tag: %s", RED, e.what());
				return true;
			}
		}
	}

	transactionManager->addAction(InventoryAction(slot, nullptr, yot));
	transactionManager->addAction(InventoryAction(0, yot, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));

	inv->addItemToFirstEmptySlot(yot);

	clientMessageF("%sSuccessfully given item!", GREEN);
	return true;
}