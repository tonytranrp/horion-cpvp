#include "SetOffhandCommand.h"

#include "../../../SDK/Mojangson.h"
#include "../../../Utils/Utils.h"

SetOffhandCommand::SetOffhandCommand() : IMCCommand("setoffhand", "Spawn items in ur offhand", "<itemName> <count> <itemData>") {
	registerAlias("soh");
}

SetOffhandCommand::~SetOffhandCommand() {
}

bool SetOffhandCommand::execute(std::vector<std::string>* args) {
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
	ItemStack* item = nullptr;
	ItemStack* altItem = nullptr;
	auto transactionManager = Game.getTransactionManager();

	if (itemId == 0) {
		auto cStack = Game.getItemRegistry()->lookUpByName(args->at(1));
		if (!cStack) {
			clientMessageF("%sInvalid item name!", RED);
			return true;
		}
		item = new ItemStack(*cStack, count, itemData);
	} else {
		Item* cStack = Game.getItemRegistry()->getItemFromId(itemId);
		if (!cStack) {
			clientMessageF("%sInvalid item ID!", RED);
			return true;
		}
		item = new ItemStack(*cStack, count, itemData);
	}

	if (item)
		item->count = count;

	if (args->size() > 4) {
		std::string tag = Utils::getClipboardText();
		if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START && tag.back() == MojangsonToken::COMPOUND_END) {
			try {
				item->setUserData(std::move(Mojangson::Parse(tag)));
			} catch (Mojangson::Exception&) {
			}
		}
	}

	transactionManager->addAction(InventoryAction(0, nullptr, item, ContainerID::Offhand));
	Game.getLocalPlayer()->setOffhandSlot(item);
	transactionManager->addAction(InventoryAction(0, item, nullptr, ContainerID::Offhand, InventorySource::Type::NotImplemented));
	item->count = count + 1;
	Game.getLocalPlayer()->setOffhandSlot(item);

	clientMessageF("%sSuccessfully set item to offhand!", GREEN);
	return true;
}