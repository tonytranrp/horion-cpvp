#include "PathCommand.h"

#include "../../Module/ModuleManager.h"
#include "../../path/goals/JoeGoalXYZ.h"
#include "../../path/goals/JoeGoalXZ.h"
#include "../../path/goals/JoeGoalY.h"

PathCommand::PathCommand() : IMCCommand("path", "Joe path", "<y|xz|xyz> [args]") {
	this->registerAlias("joe");
}
PathCommand::~PathCommand() {
}
bool PathCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);
	static auto mod = moduleMgr->getModule<FollowPathModule>("FollowPath");
	if (mod->isEnabled()) {
		clientMessageF("Joe is already enabled, disable joe to use this command");
		return true;
	}

	auto cmd = args->at(1);
	if (cmd == "y") {
		assertTrue(args->size() > 2);
		int yLevel = assertInt(args->at(2));
		assertTrue(yLevel > -64 && yLevel < 321);

		mod->goal = std::make_unique<JoeGoalY>((float)yLevel);
		mod->setEnabled(true);

		clientMessageF("Starting search...");
		return true;
	}
	if (cmd == "xz") {
		assertTrue(args->size() > 3);
		int x = assertInt(args->at(2));
		int z = assertInt(args->at(3));

		mod->goal = std::make_unique<JoeGoalXZ>(Vec3i(x, 0, z));
		mod->setEnabled(true);

		clientMessageF("Starting search...");
		return true;
	}
	if (cmd == "xyz") {
		assertTrue(args->size() > 4);
		int x = assertInt(args->at(2));
		int y = assertInt(args->at(3));
		int z = assertInt(args->at(4));

		mod->goal = std::make_unique<JoeGoalXYZ>(Vec3i(x, y, z));
		mod->setEnabled(true);

		clientMessageF("Starting search...");
		return true;
	}
	if (cmd == "p" || cmd == "player") {
		std::string nameOfPlayer = args->at(2);
		assertTrue(!nameOfPlayer.empty());
		std::string nameOfPlayerLower = std::string(nameOfPlayer);
		std::transform(nameOfPlayerLower.begin(), nameOfPlayerLower.end(), nameOfPlayerLower.begin(), ::tolower);
		nameOfPlayerLower = Utils::sanitize(nameOfPlayerLower);

		Vec3 pos{};
		Game.forEachEntity([&](Entity* e) {
			if (e == Game.getLocalPlayer())
				return;
			std::string name(e->getNameTag().c_str());
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (name.find(nameOfPlayerLower) == std::string::npos)
				return;

			pos = *e->getPos();
		});

		if (pos.iszero()) {
			clientMessageF("%s Player \"%s\" could not be found!", GOLD, nameOfPlayer.c_str());
			return true;
		}

		Vec3i endNode((int)floorf(pos.x), (int)roundf(pos.y), (int)floorf(pos.z));
		mod->goal = std::make_unique<JoeGoalXYZ>(endNode);
		mod->setEnabled(true);
		clientMessageF("Starting search...");

		return true;
	}

	return false;
}