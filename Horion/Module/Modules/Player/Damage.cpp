#include "Damage.h"

Damage::Damage() : IModule(0, Category::PLAYER, "Damage Yourself") {
}

Damage::~Damage() {
}

const char* Damage::getModuleName() {
	return ("Damage");
}

void Damage::onDisable() {
	this->done = false;
}

void Damage::onSendPacket(Packet *packet) {
	if (this->done) return;

	if (packet->getId() == PacketID::MovePlayer) {
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		movePacket->Position.y = Game.getLocalPlayer()->getPos()->y + 4;
		this->done = true;
		this->setEnabled(false);
	}

	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->pos = Vec3(Game.getLocalPlayer()->getPos()->x, Game.getLocalPlayer()->getPos()->y + 4, Game.getLocalPlayer()->getPos()->z);
		this->done = true;
		this->setEnabled(false);
	}
}

