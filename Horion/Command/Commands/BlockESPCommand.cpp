#include "BlockESPCommand.h"

BlockESPCommand::BlockESPCommand() : IMCCommand("blockesp", "Choose blocks displayed in BlockESP", "<add/remove/list/clear> <BlockName/ID/all> [Hex Color (#FFFFFF) ]") {

}

bool BlockESPCommand::execute(std::vector<std::string>* args) {
#ifdef HORION_DEBUG
	static BlockESP* mod = moduleMgr->getModule<BlockESP>("BlockESP");
	
	assertTrue(args->size() >= 2);
	std::string command = args->at(1);
	auto getBlockName = [this, args]() {
		assertTrue(args->size() >= 3);
		std::string blockname = args->at(2);

		int blockid = 0;
		try {
			blockid = std::stoi(args->at(2));
			auto item = Game.getItemRegistry()->getItemFromId(blockid);
			if (!item) {
				clientMessageF("Block id is not valid.");
				return std::string();
			}
			blockname = item->name;
		} catch (const std::invalid_argument&) {
		}

		if (blockname != "all") {  // check that this item/block exists
			auto item = Game.getItemRegistry()->lookUpByName(blockname);
			if (!item) {
				clientMessageF("Block name does not exist.");
				return std::string();
			}
		}
		return blockname;
	};
	

	if (command == "add") {
		std::string blockname = getBlockName();
		if (blockname.empty()) return true;

		//try to remove the block if it exists
		auto blockI = mod->requestedBlocks.end();
		for (auto it = mod->requestedBlocks.begin(); it != mod->requestedBlocks.end(); it++)
			if (it->name == blockname) {
				blockI = it;
				break;
			}

		if (blockI != mod->requestedBlocks.end()) 
			mod->requestedBlocks.erase(blockI);


		if (blockname == "all") {
			clientMessageF("I don't think adding all blocks is a good idea...");
			return true;
		}
		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
		if (args->size() > 3) {
			const char* color = args->at(3).c_str();
			if (*color == '#') color++;
			auto getDigits = [&color]() -> uint8_t {
				if (*color == '\0') return 255;
				uint16_t res = 0;
				if (*color >= '0' && *color <= '9')
					res += *color - '0';
				else if (*color >= 'A' && *color <= 'F')
					res += *color - 'A' + 0xA;
				else if (*color >= 'a' && *color <= 'f')
					res += *color - 'a' + 0xA;

				color++;
				if (*color == '\0') return 255;
				res *= 16;

				if (*color >= '0' && *color <= '9')
					res += *color - '0';
				else if (*color >= 'A' && *color <= 'F')
					res += *color - 'A' + 0xA;
				else if (*color >= 'a' && *color <= 'f')
					res += *color - 'a' + 0xA;

				color++;
				return (uint8_t)res;
			};
			r = getDigits();
			g = getDigits();
			b = getDigits();
		}

		BlockESP::RequestedBlock newBlock;
		newBlock.name = blockname;
		newBlock.color = MC_Color(r, g, b, 255);
		mod->requestedBlocks.emplace(newBlock);
		mod->updateRequestedBlocks();
		clientMessageF("Added %s%s%s in the list!", GRAY, blockname.c_str(), RESET);
	} else if (command == "remove") {
		std::string blockname = getBlockName();
		if (blockname.empty()) return true;
		auto blockI = mod->requestedBlocks.end();
		for (auto it = mod->requestedBlocks.begin(); it != mod->requestedBlocks.end(); it++)
			if (it->name == blockname) {
				blockI = it;
				break;
			}

		if (blockI == mod->requestedBlocks.end()) {
			clientMessageF("This block is not in the list!");
			return true;
		}

		mod->requestedBlocks.erase(blockI);
		mod->updateRequestedBlocks();
		clientMessageF("Removed %s%s%s from the list!", GRAY, blockname.c_str(), RESET);
	} else if (command == "clear") {
		mod->requestedBlocks.clear();
		mod->updateRequestedBlocks();
	} else if (command == "list") {
		std::string msg;
		for (auto& block : mod->requestedBlocks) {
			msg += "ID: " +  std::to_string(block.id) + " Name: " + block.name + "\n";
		}
		if (msg.empty())
			msg = "No blocks are in the list";
		else
			msg.erase(msg.end() - 1);
		Game.getGuiData()->displayClientMessage(&msg);
	}

	return true;
#else
	return false;
#endif
}