#include "UnlockAchievementsCommand.h"
#include "../../Module/ModuleManager.h"

UnlockAchievementsCommand::UnlockAchievementsCommand() : IMCCommand(
	"unlockachievements", "Unlocks all achievements in a local world.", "") {}


bool UnlockAchievementsCommand::execute(std::vector<std::string>* args) {

	auto lp = Game.getLocalPlayer();
	this->assertTrue(lp);

	if (!Game.getRakNetConnector()->isOnAServer()) {
		lp->unlockAchievments();
		this->clientMessageF("%sSuccessfully unlocked all achievements! Waiting for Xbox confirmation...", GREEN);
		return true;
	}
	this->clientMessageF("%sFailed to unlock achievements! Make sure you are in a local world.", RED);
	return false;
}