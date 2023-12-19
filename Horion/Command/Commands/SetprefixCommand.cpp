#include "SetprefixCommand.h"

SetprefixCommand::SetprefixCommand() : IMCCommand("setprefix", "Set the prefix for horion commands", "<prefix>") {
	registerAlias("prefix");
}

SetprefixCommand::~SetprefixCommand() {
}

bool SetprefixCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);
	assertTrue(args->at(1).length() == 1);
	char prefix = args->at(1).at(0);
	if (prefix == '/') {
		clientMessageF("Your prefix cannot be '/'");
	} else {
		cmdMgr->prefix = prefix;
		clientMessageF("Set prefix to %s%c", GRAY, prefix);
	}
	return true;
}