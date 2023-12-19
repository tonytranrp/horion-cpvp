#include "NameSpoofCommand.h"

NameSpoofCommand::NameSpoofCommand() : IMCCommand("namespoof", "Spoof your name on realms/bds", "<set/reset> <name>") {
	registerAlias("ns");
	registerAlias("fakename");
	registerAlias("fn");
}

bool NameSpoofCommand::execute(std::vector<std::string>* args) {

	auto arg1 = args->at(1); 
	if ((arg1 == "set") && args->size() > 2) {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 2)
				os << " ";
			os << args->at(i);
		}
		std::string name = os.str();
		clientMessageF("Set fakename to %s%s%s, please reconnect!", GRAY, name, RESET);
		Game.setFakeName(std::move(name));
		return true;
	} else if (args->at(1) == "reset") {
		Game.resetFakeName();
		clientMessageF("Reset fakename!");
		return true;
	}
	return false;
}