#include "FriendListCommand.h"

FriendListCommand::FriendListCommand() : IMCCommand("friend", "Add/Remove friendly players", "<add/remove>") {
	registerAlias("friendlist");
	registerAlias("wl");
	registerAlias("whitelist");
	registerAlias("f");
}

FriendListCommand::~FriendListCommand() {
}

bool FriendListCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 3);
	std::string subcommand = args->at(1);
	std::transform(subcommand.begin(), subcommand.end(), subcommand.begin(), ::tolower);
	std::string searchedName = args->at(2);                                                     // Friend to add/remove
	std::transform(searchedName.begin(), searchedName.end(), searchedName.begin(), ::tolower);  // tolower

	std::string playerName;
	Game.forEachEntity([&](Entity* ent) {
		std::string currentEntityName(ent->getNameTag().c_str());

		std::transform(currentEntityName.begin(), currentEntityName.end(), currentEntityName.begin(), ::tolower);  // tolower

		if (ent == 0)
			return;

		if (ent == Game.getLocalPlayer())  // Skip Local player
			return;

		if (currentEntityName.find(searchedName) == std::string::npos)  // Continue if name not found
			return;

		playerName = ent->getNameTag().c_str();
	});

	if (playerName.size() <= 1) {
		clientMessageF("Couldn't find player: %s%s%s!", GRAY, searchedName.c_str(), RESET);
		return true;
	}
	if (subcommand == "add") {
		FriendList::addPlayerToList(playerName);
		clientMessageF("%s%s%s is now your friend!", GRAY, playerName.c_str(), RESET);
		return true;
	} else if (subcommand == "remove") {
		if (FriendList::removePlayer(searchedName)) {
			clientMessageF("%s%s%s has been removed from your friendlist!", GRAY, searchedName.c_str(), RESET);
			return true;
		} else {
			clientMessageF("%s%s%s was not in your friendlist!", GRAY, searchedName.c_str(), RESET);
			return true;
		}
	}
	return true;
}