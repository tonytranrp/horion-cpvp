#include "SignEditCommand.h"

#include "../../Module/ModuleManager.h"

SignEditCommand::SignEditCommand() : IMCCommand("SignEdit", "Edit SignEdit text", "") {
	registerAlias("sign");
}

SignEditCommand::~SignEditCommand() {
}

bool SignEditCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer());

	auto SignEditMod = moduleMgr->getModule<SignEdit>("SignEdit");

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	std::string text = os.str();
	SignEditMod->SignText = text;
	clientMessageF("%sSignEdit text set to %s%s%s!", GREEN, GRAY, text.c_str(), GREEN);
	return true;
}