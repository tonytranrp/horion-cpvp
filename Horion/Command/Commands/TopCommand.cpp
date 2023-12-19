#include "TopCommand.h"

TopCommand::TopCommand() : IMCCommand("top", "Teleports to the nearest pocket of air above you - highly recommend against using this inside the Nether.", "") {
}

TopCommand::~TopCommand() {
}

bool TopCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer());

	LocalPlayer* player = Game.getLocalPlayer();
	Vec3 playerPos = player->getEyePos();
	bool groundAbove = false;  //Checking if ground above us.
	Vec3 blockPos;

	for (int y(0); y < 256 - playerPos.y; ++y) {
		if (Game.getRegion()->getBlock(Vec3{playerPos.x, playerPos.y + y, playerPos.z})->toLegacy()->blockId) {
			groundAbove = true;
			blockPos = Vec3{playerPos.x, playerPos.y + y, playerPos.z};
			break;
		}
	}

	if (groundAbove) {
		for (int y(0); y < 256 - blockPos.y; ++y) {  //This time we're going through loop again, but continuing where we left off to find open air pocket.
			if ((Game.getRegion()->getBlock(Vec3{blockPos.x, blockPos.y + y, blockPos.z})->toLegacy()->blockId == 0) && (Game.getRegion()->getBlock(Vec3{blockPos.x, blockPos.y + y + 1, blockPos.z})->toLegacy()->blockId == 0)) {
				player->setPos(Vec3{blockPos.x, blockPos.y + y + 1, blockPos.z});
				clientMessageF("Whoosh!");
				return true;
			}
		}
		clientMessageF("There is no open space above you!");
		return true;
	} else {
		clientMessageF("There are no blocks above you!");
		return true;
	}
}