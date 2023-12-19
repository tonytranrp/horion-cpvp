#include "Jetpack.h"

#include "../../../../Utils/Logger.h"

Jetpack::Jetpack() : IModule('F', Category::MOVEMENT, "Fly around like you had a Jetpack!") {
	registerFloatSetting("speed", &this->speedMod, 1, 0.2f, 3.f);
	this->registerBoolSetting("Bypass", &this->isBypass, this->isBypass);
}

Jetpack::~Jetpack() {
}

bool Jetpack::isFlashMode() {
	return true;
}

const char* Jetpack::getModuleName() {
	return ("Jetpack");
}

std::string Jetpack::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speedMod);
	return speedChar;
}

void Jetpack::onTick(GameMode* gm) {
	float calcYaw = (gm->player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	float calcPitch = (gm->player->viewAnglesPtr->viewAngles.x) * -(PI / 180);

	if (!isBypass) {
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
		moveVec.y = sin(calcPitch) * speedMod;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

		gm->player->lerpMotion(moveVec);
	} else {
		delay++;

		if (delay >= 5) {
			Vec3 pos = *Game.getLocalPlayer()->getPos();
			MovePlayerPacket a(Game.getLocalPlayer(), pos);
			Game.getLoopbackPacketSender()->sendToServer(&a);
			pos.y += 0.35f;
			a = MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getLoopbackPacketSender()->sendToServer(&a);

			gm->player->stateVector->velocity.y = 0.465f;
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

			gm->player->stateVector->velocity.x = moveVec.x;
			gm->player->stateVector->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			Game.getLocalPlayer()->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			gm->player->stateVector->velocity.y -= 0.15f;
			delay = 0;
		}
	}
}