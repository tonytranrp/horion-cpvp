#include "BruhCommand.h"

BruhCommand::BruhCommand() : IMCCommand("bruh", "bruh moment", "bruh") {
}

BruhCommand::~BruhCommand() {
}

bool BruhCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer());

	clientMessageF("bruh moment");
	return true;
}