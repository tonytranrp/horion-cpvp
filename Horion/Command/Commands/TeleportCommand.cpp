#include "TeleportCommand.h"

TeleportCommand::TeleportCommand() :
	IMCCommand("tp", "Teleports to coordinates",
	"<float: x> <float: y> <float: z> [lerpMode: instant/smooth] [lerpRate: int]") {

	this->registerAlias("teleport");
	this->registerAlias("setpos");
}

bool TeleportCommand::execute(std::vector<std::string>* args) {

	auto lp = Game.getLocalPlayer();
	this->assertTrue(lp);

	size_t argSize = args->size();
	this->assertTrue(argSize >= 4);

	Vec3 pos{};
	pos.x = this->assertFloat((*args)[1]);
	pos.y = this->assertFloat((*args)[2]);
	pos.y += (lp->getPos()->y - lp->aabbShape->aabb.lower.y);
	pos.z = this->assertFloat((*args)[3]);

	// smooth lerp mode might be useful for getting around realm ACs that try and detect single tick pos deltas
	if ((argSize >= 5) && ((*args)[4] == "smooth")) {

		int32_t lerpRate = 1;
		if (argSize >= 6) {
			lerpRate = this->assertInt((*args)[5]);
			if (lerpRate < 1) {
				this->clientMessageF("%sLerp rate must be at least 1!", RED);
				return false;
			}
		}
		lp->lerpTo(pos, Vec2(lp->viewAnglesPtr->viewAngles.x, lp->viewAnglesPtr->viewAngles.y), lerpRate);  // rot parameter seems to do nothing
	}
	else {
		lp->setPos(pos);
	}

	this->clientMessageF("%sTeleported!", GREEN);
	return true;
}