#include "ItemIDCommand.h"
using namespace std;
ItemIDCommand::ItemIDCommand() : IMCCommand("id", "Get a item id by holding it or get a block id by looking at a block", "<block/item>") {
}

ItemIDCommand::~ItemIDCommand() {
	registerAlias("identify");
}

bool ItemIDCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "item" && Game.getLocalPlayer()->getSelectedItemId() ) {
		int id = Game.getLocalPlayer()->getSelectedItemId();
		std::string name = Game.getLocalPlayer()->getSelectedItem()->getItem()->name;

		clientMessageF("Item Name: %s", name.c_str());
		Utils::setClipboardText(name + " id = " + std::to_string(id));
		clientMessageF("Item ID: %d", id);

	}

	else if (args->at(1) == "block") {
		Level* pointingStruct = Game.getLevel();
		Block* block = Game.getRegion()->getBlock(pointingStruct->getHitResult().block);
		int id = block->toLegacy()->blockId;
		std::string name = block->toLegacy()->getRawNameStr().c_str();

		clientMessageF("Block Name: %s", name.c_str());
		Utils::setClipboardText(name + " id = " + std::to_string(id));
		clientMessageF("Block ID: %d", id);

	}

	else return false;
	return true;
}