#include "AutoRespawn.h"

AutoRespawn::AutoRespawn() : IModule(0, Category::PLAYER, "Respawn immediately!") {
}

AutoRespawn::~AutoRespawn() {
}

const char* AutoRespawn::getModuleName() {
	return "AutoRespawn";
}

void AutoRespawn::onTick(GameMode* gm) {
	if (Game.getLocalPlayer()->getDeathTime() == 1) {
		RespawnPacket p;
		p.actorRuntimeId = Game.getLocalPlayer()->getRuntimeId();
		p.respawnState = RespawnPacket::RespawnState::ClientReadyToSpawn;
		Game.getLoopbackPacketSender()->send(&p);
	}
}