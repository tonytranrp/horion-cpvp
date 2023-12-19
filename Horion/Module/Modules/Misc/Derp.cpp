#include "Derp.h"

Derp::Derp() : IModule(0, Category::MISC, "lol you stupid") {
	this->registerBoolSetting("ihaveastroke", &this->epicStroke, this->epicStroke);
	this->registerBoolSetting("packet mode", &this->packetMode, this->packetMode);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onTick(GameMode* gm) {
	if (packetMode) {
		MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
		if (epicStroke) {
			p.pitch = (float)(rand() % 360);
			p.yaw = (float)(rand() % 360);
		} else {
			p.pitch = (float)(counter % 360);
			p.yaw = (float)(counter % 360);
		}
		Game.getLoopbackPacketSender()->sendToServer(&p);
	} else {
		if (epicStroke) {
			gm->player->viewAnglesPtr->viewAngles.x = (float)(rand() % 360);

			// FIXME: no bodyyaw
			//gm->player->bodyYaw = (float)(rand() % 360);
		} else {
			gm->player->viewAnglesPtr->viewAngles.x = (float)(counter % 360);
			//gm->player->bodyYaw = (float)(counter % 360);
		}
	}

	if (counter < 360)
		counter++;
	else
		counter = 0;
}