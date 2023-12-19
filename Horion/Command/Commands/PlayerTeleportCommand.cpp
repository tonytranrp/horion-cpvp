#include "PlayerTeleportCommand.h"

#include "../../../Utils/Utils.h"

PlayerTeleportCommand::PlayerTeleportCommand() : IMCCommand("playertp", "Teleports to players coordinates", "<NameOfThePlayer>") {
}

PlayerTeleportCommand::~PlayerTeleportCommand() {
}

bool PlayerTeleportCommand::execute(std::vector<std::string>* args) {
	//assertTrue(Game.getLocalPlayer());
	//assertTrue(args->size() > 1);  // .playertp <player>
	//std::string nameOfPlayer = args->at(1);
	//assertTrue(!nameOfPlayer.empty());
	//std::string nameOfPlayerLower = std::string(nameOfPlayer);
	//std::transform(nameOfPlayerLower.begin(), nameOfPlayerLower.end(), nameOfPlayerLower.begin(), ::tolower);
	//nameOfPlayerLower = Utils::sanitize(nameOfPlayerLower);

	//EntityList* entList = Game.getEntityList();
	//size_t listSize = entList->getListSize();
	//Vec3 pos;

	//if (listSize > 5000) {
	//	return true;
	//}
	//std::string playerName;
	////Loop through all our players and retrieve their information
	//for (size_t i = 0; i < listSize; i++) {
	//	Entity* currentEntity = entList->get(i);

	//	std::string name(currentEntity->getNameTag().getText());

	//	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	//	if (currentEntity == 0)
	//		break;

	//	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
	//		continue;

	//	if (name.find(nameOfPlayerLower) == std::string::npos)
	//		continue;

	//	pos = *currentEntity->getPos();
	//	playerName = currentEntity->getNameTag().getText();
	//	break;
	//}
	//if (pos.iszero()) {
	//	clientMessageF("Couldn't find player: %s!", nameOfPlayer.c_str());
	//	return true;
	//}
	//Game.getLocalPlayer()->setPos(pos);
	//clientMessageF("Teleported to %s", playerName.c_str());
	//return true;
	return false;
}