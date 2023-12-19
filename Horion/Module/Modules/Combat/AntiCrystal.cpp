#include "AntiCrystal.h"

AntiCrystal::AntiCrystal() : IModule(0x0, Category::COMBAT, "Reduces the crystal damage by varying amounts") {
	registerFloatSetting("Player -Y Level", &this->posY, this->posY, 0.4f, 1.61f);
}

AntiCrystal::~AntiCrystal() {
}

const char* AntiCrystal::getModuleName() {
	return ("AntiCrystal");
}

std::string AntiCrystal::getModeText() {
	char YLevelChar[50];
	sprintf_s(YLevelChar, "Y Pos: %.1f", posY);
	return YLevelChar;
}

void AntiCrystal::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* InputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		InputPacket->pos.y -= posY;
	} else if (packet->getId() == PacketID::MovePlayer) {
		MovePlayerPacket* MovePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		MovePacket->Position.y -= posY;
	}
}
