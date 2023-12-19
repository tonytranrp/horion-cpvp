#include "DeviceIDCommand.h"

DeviceIDCommand::DeviceIDCommand() :
	IMCCommand("deviceid", "Get another player's device ID!", "<playerName: string>") {}





bool DeviceIDCommand::execute(std::vector<std::string>* args) {

	auto lp = Game.getLocalPlayer();
	this->assertTrue(lp);

	size_t argLength = args->size();
	if (argLength <= 1) {
		this->clientMessageF("%sYour device ID is: %s", GREEN, lp->deviceId.c_str());
		return true;
	}

	std::string const& playerNameArg = (*args)[1];

	bool foundPlayer = false;
	Game.forEachEntity([&](Entity* actor) {
		if (foundPlayer) return;
		if (actor->isPlayer()) {
			const auto& player = *reinterpret_cast<Player*>(actor);
			if (strcmp(playerNameArg.c_str(), player.playerName.c_str()) == 0) {
				this->clientMessageF("%s%s's device ID is: %s", GREEN, player.playerName.c_str(), player.deviceId.c_str());
				foundPlayer = true;
			}
		}
	});

	if (foundPlayer) return true;
	else {
		this->clientMessageF(
			"%sNo player with the name %s was found in the player list! Make sure the player is in render distance.",
			RED, playerNameArg.c_str());
	}
	
	return false;
}