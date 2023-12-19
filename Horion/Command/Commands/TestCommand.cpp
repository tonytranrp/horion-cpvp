#include "TestCommand.h"

#include "../../../SDK/Tag.h"
#include "../../../SDK/Mojangson.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	std::string name = ent->getNameTag().c_str();
	logF("---------------");
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

void showAimedBlockInfo() {
	Level* pointingStruct = Game.getLevel();
	Block* block = Game.getRegion()->getBlock(pointingStruct->getHitResult().block);
	auto id = block->toLegacy()->blockId;
	std::string name = block->toLegacy()->getRawNameStr().c_str();
	logF("---------------");
	logF("Block Name: %s", name);
	logF("Block ID: %lld", id);
	logF("---------------");
}

bool TestCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);

	std::string s;
	for (int i = 1; i < args->size(); ++i) {
		if (i != 1)
			s += ' ';
		s += args->at(i);
	}
	
	try {
		std::unique_ptr<Tag> tag = Mojangson::Parse(s);
		
		std::string tagStr = Mojangson::FromTag(tag.get(), false);

		Game.getGuiData()->displayClientMessage(&tagStr);
	} catch (Mojangson::Exception& e) {
		logF("%s", e.what());
		return true;
	}
	logF("Success.");

	return true;
}