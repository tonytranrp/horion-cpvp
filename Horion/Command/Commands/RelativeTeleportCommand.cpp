#include "RelativeTeleportCommand.h"

RelativeTeleportCommand::RelativeTeleportCommand() : IMCCommand("relativeteleport", "Teleports to coordinates relative to the Player", "<X> <Y> <Z>") {
	registerAlias("reltp");
	registerAlias("rtp");
	registerAlias("vclip");
	registerAlias("hclip");
}

RelativeTeleportCommand::~RelativeTeleportCommand() {
}

bool RelativeTeleportCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer());
	bool clip = args->at(0).substr(1) == "clip";
	assertTrue(args->size() == (clip ? 2: 4));

	Vec3 pos{Game.getLocalPlayer()->getEyePos()};

	if (clip) { // (V/H)Clip
		if (args->at(0).at(0) == 'h') {
			Vec2 move = MoveUtils::GetMove(assertFloat(args->at(1)), Game.getLocalPlayer()->viewAnglesPtr->viewAngles.y);
			pos.x += move.x;
			pos.z += move.y;
		} else if (args->at(0).at(0) == 'v') {
			pos.y += assertFloat(args->at(1));
		}
	} else { // RelativeTP
		pos.x += assertFloat(args->at(1));
		pos.y += assertFloat(args->at(2));
		pos.z += assertFloat(args->at(3));
	}

	Game.getLocalPlayer()->setPos(pos);
	clientMessageF("%sTeleported!", GREEN);
	return true;
}